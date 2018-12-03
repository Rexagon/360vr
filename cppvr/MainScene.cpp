#include "MainScene.h"

#include <thread>
#include <GL/glew.h>

void MainScene::onInit()
{
	initManagers();

	m_vrManager->connect();

	glEnable(GL_CULL_FACE);

	m_carpet = std::make_unique<Carpet>(getCore());
	m_skybox = std::make_unique<Skybox>(getCore());
	m_headSet = std::make_unique<HeadSet>(getCore());

	m_receivingEnabled = true;
	m_streamingThread = std::make_unique<std::thread>(std::thread([this]() {
		try {
			m_videoStream.init("rtmp://rtuitlab.ru/stream/test");
			m_isConnected = true;

			m_videoStream.startReceiving(m_receivingEnabled);
		}
		catch (const std::runtime_error& e) {
			printf("Error in video stream thread: %s", e.what());
		}
	}));
}

void MainScene::onClose()
{
	m_receivingEnabled = false;
	if (m_streamingThread->joinable()) {
		m_streamingThread->join();
	}
}

void MainScene::onUpdate(const float dt)
{
	if (m_isConnected && m_videoStream.hasData()) {
		m_skybox->updateTexture(&m_videoStream);
	}

	m_vrManager->update();

	m_headSet->update(dt);

	drawScene(m_headSet->bindEye(vr::Eye_Left));
	drawScene(m_headSet->bindEye(vr::Eye_Right));
	m_headSet->submit();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	const auto& windowSize = m_windowManager->getWindow().getSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	m_headSet->drawDebug();
}

void MainScene::initManagers()
{
	m_vrManager = getCore().get<ej::VRManager>();
	m_vrManager->connect();

	m_inputManager = getCore().get<ej::InputManager>();

	m_windowManager = getCore().get<ej::WindowManager>();
}

void MainScene::drawScene(const ej::Camera& camera) const
{
	glClear(GL_DEPTH_BUFFER_BIT);

	m_skybox->draw(camera, m_headSet->getTransform());
	m_carpet->draw(camera);
}