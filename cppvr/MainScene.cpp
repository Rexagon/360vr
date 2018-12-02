#include "MainScene.h"

#include <thread>
#include <GL/glew.h>
#include "WindowManager.h"

void MainScene::onInit()
{
	const auto& windowSize = getLocator().get<ej::WindowManager>()->getWindow().getSize();

	glEnable(GL_CULL_FACE);

	m_camera.setAspect(static_cast<float>(windowSize.x) / windowSize.y);

	m_inputManager = getLocator().get<ej::InputManager>();

	m_receivingVideo = true;

	m_skybox = std::make_unique<Skybox>(getLocator());
	m_streamingThread = std::make_unique<std::thread>(std::thread([this]() {
		try {
			m_videoStream.init("rtmp://rtuitlab.ru/stream/test");
			m_skyboxState = NeedInitialize;

			m_videoStream.startReceiving(m_receivingVideo);
		}
		catch (const std::runtime_error& e) {
			printf("Error in video stream thread: %s", e.what());
		}
	}));
}

void MainScene::onClose()
{
	m_receivingVideo = false;
	if (m_streamingThread->joinable()) {
		m_streamingThread->join();
	}
}

void MainScene::onUpdate(const float dt)
{
	m_camera.updateProjection();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_skyboxState == NeedInitialize) {
		m_skybox->init(m_videoStream.getSize());
		m_skyboxState = Initialized;
	}

	if (m_skyboxState == Initialized) {
		if (m_videoStream.hasData()) {
			m_skybox->updateTexture(&m_videoStream);
		}
	}

	if (dt < 0.5f) {
		glm::vec3 vec;
		bool moved = false;

		if (m_inputManager->getKey(ej::Key::A)) {
			vec -= glm::vec3(0.0f, 1.0f, 0.0f);
			moved = true;
		}
		if (m_inputManager->getKey(ej::Key::D)) {
			vec += glm::vec3(0.0f, 1.0f, 0.0f);
			moved = true;
		}

		if (moved) {
			m_cameraTransform.rotate(vec * dt * 30.0f);
		}
	}

	m_camera.updateView(m_cameraTransform.getRotationMatrix());

	m_skybox->draw(&m_camera);
}