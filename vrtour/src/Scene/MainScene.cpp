#include "Scene/MainScene.h"

#include <fstream>

#include <json.hpp>
#include <Managers/MeshManager.h>
#include <Managers/FontManager.h>
#include <Managers/TextureManager.h>

#include "Managers/VideoManager.h"
#include "Rendering/SkyboxMaterial.h"

using json = nlohmann::json;

app::MainScene::Target::Target(ej::Texture* texture, 
	std::unique_ptr<TextureStreamer> streamer,
	std::unique_ptr<Video> video) :
	m_texture(texture), m_streamer(std::move(streamer)), 
	m_video(std::move(video))
{
}

void app::MainScene::Target::write() const
{
	if (m_texture == nullptr ||
		m_streamer == nullptr ||
		m_video == nullptr)
	{
		return;
	}

	m_streamer->write(m_texture, m_video->getVideoStream());
}

ej::Texture* app::MainScene::Target::getTexture() const
{
	return m_texture;
}

void app::MainScene::onInit()
{
	const auto& core = getCore();

	// Initialize managers
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	core.get<VideoManager>()->init();
	m_renderingManager->init();

	// Create player
	createCamera();

	if (ej::VRManager::checkHmdPresent()) {
		try {
			m_vrManager = core.get<ej::VRManager>();
			m_vrManager->init();

			m_headSet = std::make_unique<HeadSet>(core);
		}
		catch(const std::runtime_error& e) {
			m_vrManager = nullptr;
			printf("%s\n", e.what());
		}
	}

	// Enable VSync only on desktop
	m_windowManager->getWindow().setVerticalSyncEnabled(m_vrManager == nullptr);

	// Create scene structure
	createSkyBox();

	// Create test mesh
	auto mesh = core.get<ej::MeshManager>()->bind("test_mesh", "meshes/uv_sphere.obj").get("test_mesh");
	auto material = std::make_unique<SimpleMeshMaterial>(core);
	ej::MeshEntity entity{ mesh, material.get() };

	entity.getTransform()
		.scale(0.5f)
		.move(0.0f, 0.0f, -5.0f);

	material->setColor(0.9f, 0.1f, 0.1f);

	m_entities.emplace_back(entity, std::move(material));

	// Create video targets
	try {
		json config;
		std::ifstream file("config.json");
		file >> config;

		const auto urlsList = config.find("urls");
		if (urlsList == config.end() || 
			!urlsList.value().is_object() || 
			urlsList.value().empty()) 
		{
			throw std::runtime_error("Unable to find urls");
		}

		for (auto it = urlsList->begin(); it != urlsList->end(); ++it) {
			const auto& target = it.key();
			const auto& url = it.value().get<std::string>();

			printf("Found video url: %s\n", url.c_str());

			auto video = std::make_unique<Video>(core, url);
			video->init();

			const auto textureName = "target_" + target;
			const auto texture = core.get<ej::TextureManager>()->bind(textureName, [](const ej::Core& core) {
				auto texture = std::make_unique<ej::Texture>(core);
				texture->init(1920, 1080, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
				return texture;
			}).get(textureName);

			auto textureStreamer = std::make_unique<TextureStreamer>(core);
			textureStreamer->init();

			m_targets.try_emplace(target, 
				Target{ texture, std::move(textureStreamer), std::move(video) });
		}

		const auto transitionsList = config.find("transitions");
		if (transitionsList == config.end() ||
			!transitionsList.value().is_array() ||
			transitionsList.value().empty())
		{
			throw std::runtime_error("Unable to find transitions");
		}

		for (auto it = transitionsList->begin(); it != transitionsList->end(); ++it) {
			const auto& transition = it.value();
			if (!transition.is_object() ||
				transition.find("begin") == transition.end() ||
				transition.find("end") == transition.end()) {
				continue;
			}

			const auto begin = transition.find("begin").value().get<std::string>();
			const auto end = transition.find("end").value().get<std::string>();

			m_transitions.try_emplace(begin, end);
		}
	}
	catch (const json::exception & e) {
		printf("Error parsing video source: %s\n", e.what());
	}
	catch (const std::runtime_error& e) {
		printf("Video source not provided\n");
	}

	if (!m_transitions.empty()) {
		prepareTransition(
			m_transitions.begin()->first, 
			m_transitions.begin()->second);
	}
}

void app::MainScene::onUpdate(const float dt)
{
	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
		return;
	}

	if (m_transitionPair.first != nullptr) {
		m_transitionPair.first->write();
	}

	if (m_transitionPair.second != nullptr) {
		m_transitionPair.second->write();
	}

	if (m_inputManager->getKeyDown(ej::Key::T)) {
		const auto begin = m_currentTransition.first;
		const auto end = m_currentTransition.second;
		
		prepareTransition(begin, end);
		m_currentTransition.first = end;
		m_currentTransition.second = begin;

		m_skyBox.second->startTransition();		
	}

	m_skyBox.second->update(dt);

	if (m_debugCamera != nullptr) {
		m_debugCamera->update(dt);

		m_renderingManager->getState()->setCurrentFrameBuffer(nullptr);

		const auto& windowSize = m_windowManager->getWindow().getSize();
		m_renderingManager->getState()->setViewport(0, 0, windowSize.x, windowSize.y);

		m_renderingManager->getForwardRenderer()->setCameraEntity(m_debugCamera->getCameraEntity());

		drawScene();
	}

	if (m_vrManager != nullptr) {
		m_vrManager->update();
		m_headSet->update(dt);

		for (const auto& index : m_vrManager->getControllerIndices()) {
			const auto name = m_vrManager->getDeviceRenderModelName(index);
			m_controllers.try_emplace(index, std::make_unique<SteamVRObject>(getCore(), name));
		}

		for (size_t i = 0; i < 2; ++i) {
			const auto eye = static_cast<vr::EVREye>(i);
			m_headSet->bindEye(eye);
			m_renderingManager
				->getForwardRenderer()
				->setCameraEntity(m_headSet->getCameraEntity(eye));

			// Push all controller models to renderer
			for (const auto& index : m_vrManager->getControllerIndices()) {
				const auto it = m_controllers.find(index);
				if (it == m_controllers.end()) {
					continue;
				}

				auto* entity = it->second->getMeshEntity();

				entity->getTransform().setTransformationMatrix(
					m_vrManager->getDeviceTransformation(index));

				m_renderingManager->getForwardRenderer()->push(entity);
			}

			// Finally draw scene
			drawScene();
		}

		m_headSet->submit();
	}
}

void app::MainScene::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderer = m_renderingManager->getForwardRenderer();
	for (auto& it : m_entities) {
		renderer->push(&it.first);
	}
	renderer->push(&m_skyBox.first);

	renderer->draw();
}

void app::MainScene::createSkyBox()
{
	const auto mesh = getCore().get<ej::MeshManager>()->bind("skybox_mesh", [](const ej::Core& core) {
		auto mesh = std::make_unique<ej::Mesh>(core);
		mesh->init(ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
			ej::MeshGeometry::SIMPLE_VERTEX));
		return mesh;
	}).get("skybox_mesh");

	auto material = std::make_unique<SkyBoxMaterial>(getCore());

	m_skyBox.first.setMesh(mesh);
	m_skyBox.first.setMaterial(material.get());
	m_skyBox.second = std::move(material);
}

void app::MainScene::createCamera()
{
	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getCameraEntity()->getTransform().setPosition(0.0f, 1.0f, 0.0f);
	m_debugCamera->setRotationSpeed(-0.2f);
}

void app::MainScene::prepareTransition(const std::string& begin, const std::string& end)
{
	m_currentTransition.first = begin;
	m_currentTransition.second = end;

	auto it = m_targets.find(begin);
	if (it != m_targets.end()) {
		auto* target = &it->second;
		m_transitionPair.first = target;
		m_skyBox.second->setSkyTexture(target->getTexture());
	}
	else {
		m_transitionPair.first = nullptr;
		m_skyBox.second->setSkyTexture(nullptr);
	}

	it = m_targets.find(end);
	if (it != m_targets.end()) {
		auto* target = &it->second;
		m_transitionPair.second = target;
		m_skyBox.second->setNextSkyTexture(target->getTexture());
	}
	else {
		m_transitionPair.second = nullptr;
		m_skyBox.second->setNextSkyTexture(nullptr);
	}
}
