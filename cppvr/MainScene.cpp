#include "MainScene.h"

#include <thread>
#include <GL/glew.h>

#include <iostream>

void MainScene::onInit()
{
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
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_skyboxState == NeedInitialize) {
		m_skybox->init(m_videoStream.getSize());
		m_skyboxState = Initialized;
	}

	if (m_skyboxState == Initialized) {
		if (m_videoStream.hasData()) {
			m_skybox->updateTexture(&m_videoStream);
		}

		m_skybox->draw();
	}
}