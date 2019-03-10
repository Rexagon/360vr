#include "Scene/MainScene.h"

#include <fstream>

#include <json.hpp>
#include <Managers/MeshManager.h>
#include <Managers/FontManager.h>
#include <Managers/TextureManager.h>

#include "Managers/VideoManager.h"
#include "Rendering/SkyboxMaterial.h"
#include "Rendering/WireFrameMaterial.h"

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

	//m_stateGraph.setTransitionSpeed(0.05f);

	// Initialize managers
	m_inputManager = core.get<ej::InputManager>();
	m_windowManager = core.get<ej::WindowManager>();
	m_renderingManager = core.get<ej::RenderingManager>();
	m_renderingManager->synchronize();

	core.get<VideoManager>()->init();

	m_forwardRenderer = m_renderingManager->createRenderer<ej::ForwardRenderer>();

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

	// Create test meshes
	createWireFrame();

	// Load data from config
	try {
		json config;
		std::ifstream file("config.json");
		file >> config;

		// Read all targets
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

			m_stateGraph.addState(target, texture, std::move(textureStreamer), std::move(video));
		}

		// Read all transitions
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

			glm::vec3 position;

			const auto positionData = transition.find("position");
			if (positionData->is_array()) {
				size_t component = 0;
				for (auto positionIt = positionData->begin(); positionIt != positionData->end(); ++positionIt) {
					if (positionIt.value().is_number()) {
						position[component] = positionIt.value().get<float>();
					}

					if (++component > 3) {
						break;
					}
				}
			}

			m_stateGraph.addConnection(begin, end, position);
		}

		// Read first state
		const auto firstState = config.find("begin");
		if (firstState == config.end() ||
			!firstState.value().is_string()) 
		{
			throw std::runtime_error("First state was not set");
		}

		m_stateGraph.setCurrentState(firstState.value().get<std::string>());
	}
	catch (const std::runtime_error& e) {
		printf("Config parsing error: %s\n", e.what());
	}

	updateControllers();
	updateMarkers();
}

void app::MainScene::onUpdate(const float dt)
{
	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
		return;
	}

	updateTransition(dt);

	for (auto& it : m_markers) {
		it->material->setColor(0.9f, 0.1f, 0.1f);
	}

	if (m_debugCamera != nullptr) {
		m_debugCamera->update(dt);

		drawDesktop();
	}

	if (m_vrManager != nullptr) {
		m_vrManager->update();
		m_headSet->update(dt);

		if (m_vrManager->wasDevicesInfoUpdated()) {
			updateControllers();
		}

		handleControllersRaycast();

		drawVR();
	}
}

void app::MainScene::drawDesktop()
{
	m_renderingManager->setCurrentFrameBuffer(nullptr);

	const auto& windowSize = m_windowManager->getWindow().getSize();
	m_renderingManager->setViewport(0, 0, windowSize.x, windowSize.y);

	m_forwardRenderer->setCameraEntity(m_debugCamera->getCameraEntity());

	drawScene();
}

void app::MainScene::drawVR()
{
	for (size_t i = 0; i < 2; ++i) {
		const auto eye = static_cast<vr::EVREye>(i);
		m_headSet->bindEye(eye);
		m_forwardRenderer->setCameraEntity(m_headSet->getCameraEntity(eye));

		// Push all controller models to renderer
		for (const auto& index : m_vrManager->getControllerIndices()) {
			const auto it = m_controllers.find(index);
			if (it == m_controllers.end()) {
				continue;
			}

			auto* entity = it->second->getMeshEntity();

			entity->getTransform().setTransformationMatrix(
				m_vrManager->getDeviceTransformation(index));

			m_forwardRenderer->push(entity);
		}

		// Finally draw scene
		drawScene();
	}

	m_headSet->submit();
}

void app::MainScene::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	for (auto& it : m_entities) {
		m_forwardRenderer->push(&it->meshEntity);
	}

	for (auto& it : m_markers) {
		m_forwardRenderer->push(&it->meshEntity);
	}

	m_renderingManager->draw();
}

void app::MainScene::createSkyBox()
{
	const auto meshName = "skybox";

	auto sceneEntity = std::make_unique<SceneEntity>();

	const auto mesh = getCore().get<ej::MeshManager>()->bind(meshName, [](const ej::Core& core) {
		auto mesh = std::make_unique<ej::Mesh>(core);
		mesh->init(ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
			ej::MeshGeometry::SIMPLE_VERTEX));
		return mesh;
	}).get(meshName);
	sceneEntity->meshEntity.setMesh(mesh);

	auto material = std::make_unique<SkyBoxMaterial>(getCore());
	m_skyBoxMaterial = material.get();
	sceneEntity->meshEntity.setMaterial(material.get());

	sceneEntity->material = std::move(material);

	m_entities.emplace_back(std::move(sceneEntity));
}

void app::MainScene::createWireFrame()
{
	const auto meshName = "wireframe";

	auto sceneEntity = std::make_unique<SceneEntity>();

	const auto mesh = getCore().get<ej::MeshManager>()->bind(meshName, [](const ej::Core& core) {
		auto mesh = std::make_unique<ej::Mesh>(core);

		ej::MeshGeometry meshGeometry(ej::MeshGeometry::SIMPLE_VERTEX, GL_LINES);

		const glm::vec2 halfSize{50.0f, 50.0f};
		const glm::ivec2 segments{ 100, 100 };

		unsigned int index = 0;

		const size_t positionCount = (segments.x + 1) * 2 + (segments.y + 1) * 2;
		meshGeometry.positions.reserve(positionCount);
		meshGeometry.indices.reserve(positionCount);

		for (auto i = 0; i <= segments.x; ++i) {
			const auto x = -halfSize.x + 2.0f * halfSize.x * static_cast<float>(i) / segments.x;
			meshGeometry.positions.emplace_back(x, 0.0f,-halfSize.y);
			meshGeometry.positions.emplace_back(x, 0.0f, halfSize.y);
			meshGeometry.indices.emplace_back(index++);
			meshGeometry.indices.emplace_back(index++);
		}

		for (auto i = 0; i <= segments.y; ++i) {
			const auto z = -halfSize.y + 2.0f * halfSize.y * static_cast<float>(i) / segments.y;
			meshGeometry.positions.emplace_back(-halfSize.x, 0.0f, z);
			meshGeometry.positions.emplace_back( halfSize.x, 0.0f, z);
			meshGeometry.indices.emplace_back(index++);
			meshGeometry.indices.emplace_back(index++);
		}

		mesh->init(meshGeometry);

		return mesh;
	}).get(meshName);
	sceneEntity->meshEntity.setMesh(mesh);

	auto material = std::make_unique<WireFrameMaterial>(getCore());
	material->setColor(0.1f, 0.6f, 0.6f);
	sceneEntity->meshEntity.setMaterial(material.get());

	sceneEntity->material = std::move(material);

	m_entities.emplace_back(std::move(sceneEntity));
}

void app::MainScene::createMarker(const std::string& name, const glm::vec3& position)
{
	const auto meshName = "marker";

	auto marker = std::make_unique<Marker>();
	marker->stateName = name;

	const auto mesh = getCore().get<ej::MeshManager>()->bind(meshName, "meshes/uv_sphere.obj").get(meshName);
	marker->meshEntity.setMesh(mesh);

	marker->material = std::make_unique<SimpleMeshMaterial>(getCore());
	marker->meshEntity.setMaterial(marker->material.get());

	marker->meshEntity.getTransform()
		.scale(0.5f)
		.move(position);

	m_collisionWorld.add(position, 0.5f, marker.get());

	m_markers.emplace_back(std::move(marker));
}

void app::MainScene::createLine(ej::Transform* parent)
{
	const auto meshName = "line";

	auto sceneEntity = std::make_unique<SceneEntity>();

	const auto mesh = getCore().get<ej::MeshManager>()->bind(meshName, [](const ej::Core & core) {
		auto mesh = std::make_unique<ej::Mesh>(core);
		ej::MeshGeometry meshGeometry{ ej::MeshGeometry::SIMPLE_VERTEX, GL_LINES };

		meshGeometry.positions = { {0.0f, 0.0f, 0.0f}, { 0.0, 0.0, -1.0f } };
		meshGeometry.indices = { 0, 1 };

		mesh->init(meshGeometry);

		return mesh;
	}).get(meshName);
	sceneEntity->meshEntity.setMesh(mesh);

	auto material = std::make_unique<WireFrameMaterial>(getCore());
	material->setColor(0.1f, 0.7f, 0.1f);
	sceneEntity->meshEntity.setMaterial(material.get());

	sceneEntity->material = std::move(material);

	sceneEntity->meshEntity.getTransform()
		.setParent(parent)
		.setScale(10.0f);

	m_entities.emplace_back(std::move(sceneEntity));
}

void app::MainScene::createCamera()
{
	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getCameraEntity()->getTransform().setPosition(0.0f, 1.0f, 0.0f);
	m_debugCamera->setRotationSpeed(-0.2f);
}

void app::MainScene::handleControllersRaycast()
{
	for (const auto& [controllerIndex, object] : m_controllers) {
		const auto& transform = object->getMeshEntity()->getTransform();

		void* data;
		if (m_collisionWorld.raycast(transform.getGlobalPosition(), glm::vec4(transform.getDirectionFront(), 1.0f), data)) {
			auto* marker = reinterpret_cast<Marker*>(data);
			marker->material->setColor(0.9f, 0.3f, 0.3f);

			if (m_vrManager->getButtonDown(controllerIndex, ej::VRButton::k_EButton_SteamVR_Touchpad)) {
				m_stateGraph.startTransition(marker->stateName);

				clearMarkers();
			}
		}
	}
}

void app::MainScene::updateMarkers()
{
	const auto& currentState = m_stateGraph.getCurrentState();

	clearMarkers();

	if (currentState == nullptr) {
		return;
	}

	for (const auto& state : currentState->connections) {
		createMarker(state.first->name, state.second);
	}
}

void app::MainScene::clearMarkers()
{
	m_collisionWorld.clear();
	m_markers.clear();
}

void app::MainScene::updateTransition(const float dt)
{
	m_stateGraph.update(dt);

	if (m_stateGraph.wasTransitionFinished()) {
		updateMarkers();
	}

	const auto& transition = m_stateGraph.getCurrentTransition();

	ej::Texture* textureBegin = nullptr;
	if (transition.begin != nullptr) {
		transition.begin->data.write();
		textureBegin = transition.begin->data.getTexture();
	}

	ej::Texture* textureEnd = nullptr;
	if (transition.end != nullptr) {
		transition.end->data.write();
		textureEnd = transition.end->data.getTexture();
	}

	m_skyBoxMaterial->setSkyTexture(textureBegin);
	m_skyBoxMaterial->setNextSkyTexture(textureEnd);
	m_skyBoxMaterial->setTransition(transition.value);
}

void app::MainScene::updateControllers()
{
	if (m_vrManager == nullptr || !m_vrManager->isInitialized()) {
		return;
	}

	for (const auto& index : m_vrManager->getControllerIndices()) {
		const auto name = m_vrManager->getDeviceRenderModelName(index);

		const auto it = m_controllers.find(index);
		if (it != m_controllers.end()) {
			continue;
		}

		auto object = std::make_unique<SteamVRObject>(getCore(), name);

		createLine(&object->getMeshEntity()->getTransform());
		m_controllers.emplace(index, std::move(object));
	}
}
