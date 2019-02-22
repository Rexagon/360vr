#include "Scene/MainScene.h"

#include <fstream>

#include <json.hpp>

#include <Managers/MeshManager.h>
#include <Managers/FontManager.h>
#include <Managers/TextureManager.h>

#include "Rendering/SkyboxMaterial.h"
#include "Rendering/SimpleMeshMaterial.h"

using json = nlohmann::json;

void MainScene::onInit()
{
	// Initialize managers
	m_videoManager = getCore().get<VideoManager>();
	m_inputManager = getCore().get<ej::InputManager>();
	m_windowManager = getCore().get<ej::WindowManager>();
	m_renderingManager = getCore().get<ej::RenderingManager>();

	m_windowManager->getWindow().setVerticalSyncEnabled(true);

	m_videoManager->init();
	m_renderingManager->init();

	// Create scene structure
	createCarpet();
	createSkybox();
	createCamera();

	// Create UI
	m_rectangleWidget = std::make_shared<RectangleWidget>(getCore());
	m_rectangleWidget->setSize(glm::vec2(100.0f, 50.0f));

	auto font = getCore().get<ej::FontManager>()->bind("font", "fonts/segoeui.ttf")->get("font");

	m_textWidget = std::make_shared<TextWidget>(getCore());
	m_textWidget->setFont(font);

	// Load video config
	json config;
	try {
		std::ifstream file("config.json");
		file >> config;

		const auto it = config.find("url");
		if (it == config.end()) {
			throw std::exception();
		}

		printf("%s\n", it.value().get<std::string>().data());

		m_video = std::make_unique<Video>(it.value().get<std::string>());
		m_video->init();
		m_videoManager->setCurrentVideo(m_video);

		m_textureStreamer = std::make_unique<TextureStreamer>();
	}
	catch (const std::exception& e) {
		printf("Video source not provided\n");
	}
}

void MainScene::onUpdate(const float dt)
{
	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
	}

	if (m_videoTarget != nullptr && m_textureStreamer != nullptr && 
		m_video != nullptr && m_video->hasVideoData()) 
	{
		m_textureStreamer->write(m_videoTarget, m_video.get());
	}

	m_textWidget->setText("current delta time: " + std::to_string(dt));
	m_textWidget->update(dt);

	m_debugCamera->update(dt);

	drawScene();
}

void MainScene::drawScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_renderingManager->getState()->setCurrentFrameBuffer(nullptr);

	const auto& windowSize = m_windowManager->getWindow().getSize();
	m_renderingManager->getState()->setViewport(0, 0, windowSize.x, windowSize.y);

	auto renderer = m_renderingManager->getForwardRenderer();
	for (auto& mesh : m_meshes) {
		renderer->push(mesh.get());
	}
	renderer->draw();

	m_renderingManager->getUIRenderer()->push(m_rectangleWidget->getMeshEntity().get());
	m_renderingManager->getUIRenderer()->push(m_textWidget->getMeshEntity().get());
	m_renderingManager->getUIRenderer()->draw();
}

void MainScene::createCarpet()
{
	auto mesh = getCore().get<ej::MeshManager>()->bind("carpet_mesh", []() {
		return ej::MeshGeometry::createPlane(glm::vec2(1.1f, 1.0f), 1, 1);
	})->get("carpet_mesh");

	auto material = std::make_shared<SimpleMeshMaterial>(getCore());

	material->setDiffuseTexture(getCore().get<ej::TextureManager>()->bind("carpet",
		ej::TextureManager::FromFile("textures/carpet.jpg"))->get("carpet"));
	material->setTextureFlipped(true);

	auto entity = std::make_shared<ej::MeshEntity>(mesh, material);
	entity->getTransform().setPosition(0.0f, 1.0f, -2.0f);
	entity->getTransform().setRotation(90.0f, 0.0f, 0.0f);
	entity->getTransform().setScale(1.4f, 1.0f, 1.0f);

	m_videoTarget = material->getDiffuseTexture();

	m_meshes.push_back(entity);
}

void MainScene::createSkybox()
{
	auto mesh = getCore().get<ej::MeshManager>()->bind("skybox_mesh", []() {
		return ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
			ej::MeshGeometry::SIMPLE_VERTEX);
	})->get("skybox_mesh");

	auto material = std::make_shared<SkyboxMaterial>(getCore());

	material->setSkyTexture(getCore().get<ej::TextureManager>()->bind("loading",
		ej::TextureManager::FromFile("textures/loading.jpg"))->get("loading"));

	auto entity = std::make_shared<ej::MeshEntity>(mesh, material);

	m_meshes.push_back(entity);
}

void MainScene::createCamera()
{
	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getCameraEntity()->getTransform().setPosition(0.0f, 1.0f, 0.0f);
	m_debugCamera->setRotationSpeed(-0.2f);
	m_renderingManager->getForwardRenderer()->setCameraEntity(m_debugCamera->getCameraEntity());
}
