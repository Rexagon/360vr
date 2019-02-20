#include "Scene/MainScene.h"

#include <fstream>

#include <json.hpp>
#include <GL/glew.h>

#include <Core/Core.h>
#include <Managers/MeshManager.h>
#include <Managers/TextureManager.h>
#include <Rendering/SkyboxMaterial.h>

using json = nlohmann::json;

void MainScene::onInit()
{
	m_videoManager = getCore().get<VideoManager>();
	m_inputManager = getCore().get<ej::InputManager>();
	m_windowManager = getCore().get<ej::WindowManager>();
	m_renderingManager = getCore().get<ej::RenderingManager>();

	m_windowManager->getWindow().setVerticalSyncEnabled(true);

	m_videoManager->init();
	m_renderingManager->init();

	auto carpetMesh = getCore().get<ej::MeshManager>()->bind("carpet_mesh", []() {
		return ej::MeshGeometry::createPlane(glm::vec2(1.1f, 1.0f), 1, 1);
	})->get("carpet_mesh");

	m_videoTarget = std::make_shared<SimpleMeshMaterial>(getCore());

	m_videoTarget->setDiffuseTexture(getCore().get<ej::TextureManager>()->bind("carpet",
		ej::TextureManager::FromFile("textures/carpet.jpg"))->get("carpet"));
	m_videoTarget->setTextureFlipped(true);

	auto tvCarpet = std::make_shared<ej::MeshEntity>(carpetMesh, m_videoTarget);
	tvCarpet->getTransform().setPosition(0.0f, 1.0f, -2.0f);
	tvCarpet->getTransform().setRotation(90.0f, 0.0f, 0.0f);
	tvCarpet->getTransform().setScale(1.4f, 1.0f, 1.0f);

	m_meshes.push_back(tvCarpet);

	auto skyboxMesh = getCore().get<ej::MeshManager>()->bind("skybox_mesh", []() {
		return ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
		ej::MeshGeometry::SIMPLE_VERTEX);
	})->get("skybox_mesh");

	auto skyboxMaterial = std::make_shared<SkyboxMaterial>(getCore());

	skyboxMaterial->setSkyTexture(getCore().get<ej::TextureManager>()->bind("loading",
		ej::TextureManager::FromFile("textures/loading.jpg"))->get("loading"));

	auto skybox = std::make_shared<ej::MeshEntity>(skyboxMesh, skyboxMaterial);

	m_meshes.push_back(skybox);

	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getCameraEntity()->getTransform().setPosition(0.0f, 1.0f, 0.0f);
	m_renderingManager->getForwardRenderer()->setCameraEntity(m_debugCamera->getCameraEntity());

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

void MainScene::onClose()
{
}

void MainScene::onUpdate(const float dt)
{
	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
	}

	if (m_video != nullptr && m_textureStreamer != nullptr && m_video->hasVideoData()) {
		m_textureStreamer->write(m_videoTarget->getDiffuseTexture(), m_video.get());
	}

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
}
