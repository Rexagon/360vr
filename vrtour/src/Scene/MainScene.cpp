#include "Scene/MainScene.h"

#include <fstream>

#include <json.hpp>
#include <Managers/MeshManager.h>
#include <Managers/FontManager.h>
#include <Managers/TextureManager.h>

#include "Managers/VideoManager.h"
#include "Rendering/SkyboxMaterial.h"
#include "Rendering/SimpleMeshMaterial.h"

using json = nlohmann::json;

void MainScene::onInit()
{
	const auto& core = getCore();

	// Initialize managers
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
	m_renderingManager = core.get<ej::RenderingManager>();

	m_windowManager->getWindow().setVerticalSyncEnabled(true);

	core.get<VideoManager>()->init();
	m_renderingManager->init();

	// Create player
	createCamera();

	if (ej::VRManager::checkHmdPresent()) {
		m_vrManager = core.get<ej::VRManager>();
		m_vrManager->init();

		m_headSet = std::make_unique<HeadSet>(core);
	}

	// Create scene structure
	auto skyBoxTarget = createSkyBox();

	// Load video config
	json config;
	try {
		std::ifstream file("config.json");
		file >> config;

		const auto it = config.find("urls");
		if (it == config.end() || !it.value().is_object() || it.value().empty()) {
			throw std::exception();
		}

		const auto videoCount = static_cast<float>(it.value().size() - 1);

		const glm::vec3 step{ 0.0f, 2.2f, 0.0 };
		const auto offset = -step * videoCount * 0.5f + glm::vec3(0.0f, 0.0f, -2.0f);
		auto position = offset;

		auto first = true;

		for (const auto& url : it.value()) {
			printf("Found video url: %s\n", url.get<std::string>().data());

			auto video = std::make_unique<Video>(core, url.get<std::string>());
			video->init();

			if (first) {
				m_videos.emplace_back(std::move(video), skyBoxTarget,
					std::make_unique<TextureStreamer>());

				first = false;
			}
			else {
				m_videos.emplace_back(std::move(video), createVideoTarget(position),
					std::make_unique<TextureStreamer>());

				position += step;
			}
		}
	}
	catch (const std::exception & e) {
		printf("Video source not provided\n");
	}
}

void MainScene::onUpdate(const float dt)
{
	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
		return;
	}

	for (auto& data : m_videos) {
		data.streamer->write(data.target, data.video->getVideoStream());
	}

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

		for (size_t i = 0; i < 2; ++i) {
			const auto eye = static_cast<vr::EVREye>(i);
			m_headSet->bindEye(eye);
			m_renderingManager
				->getForwardRenderer()
				->setCameraEntity(m_headSet->getCameraEntity(eye));

			drawScene();
		}

		m_headSet->submit();
	}
}

void MainScene::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	auto renderer = m_renderingManager->getForwardRenderer();
	for (auto& entity : m_entities) {
		const auto mesh = &entity.first;
		renderer->push(mesh);
	}
	renderer->draw();
}

ej::Texture* MainScene::createVideoTarget(const glm::vec3& position)
{
	static size_t targetCount = 0;

	const auto mesh = getCore().get<ej::MeshManager>()->bind("carpet_mesh", []() {
		return ej::MeshGeometry::createPlane(glm::vec2(1.1f, 1.0f));
	}).get("carpet_mesh");

	auto material = std::make_unique<SimpleMeshMaterial>(getCore());

	const auto textureName = "carpet_" + std::to_string(targetCount++);

	const auto texture = getCore().get<ej::TextureManager>()
		->bind(textureName, "textures/carpet.jpg").get(textureName);

	material->setDiffuseTexture(texture);
	material->setTextureFlipped(true);

	ej::MeshEntity entity(mesh, material.get());
	entity.getTransform().setPosition(position);
	entity.getTransform().setRotation(90.0f, 0.0f, 0.0f);
	entity.getTransform().setScale(1.4f, 1.0f, 1.0f);

	m_entities.emplace_back(entity, std::move(material));

	return texture;
}

ej::Texture* MainScene::createSkyBox()
{
	const auto mesh = getCore().get<ej::MeshManager>()->bind("skybox_mesh", []() {
		return ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
			ej::MeshGeometry::SIMPLE_VERTEX);
	}).get("skybox_mesh");

	auto material = std::make_unique<SkyBoxMaterial>(getCore());

	const auto texture = getCore().get<ej::TextureManager>()
		->bind("loading", "textures/loading.jpg").get("loading");

	material->setSkyTexture(texture);

	ej::MeshEntity entity(mesh, material.get());
	m_entities.emplace_back(std::move(entity), std::move(material));

	return texture;
}

void MainScene::createCamera()
{
	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getCameraEntity()->getTransform().setPosition(0.0f, 1.0f, 0.0f);
	m_debugCamera->setRotationSpeed(-0.2f);
}
