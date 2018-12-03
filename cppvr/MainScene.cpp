#include "MainScene.h"

#include <thread>
#include <GL/glew.h>
#include "ShaderManager.h"

void MainScene::onInit()
{
	initManagers();

	m_vrManager->connect();

	glEnable(GL_CULL_FACE);
	
	m_receivingVideo = true;

	m_carpet = std::make_unique<Carpet>(getLocator());

	getLocator().get<ej::ShaderManager>()->bind("quad",
		ej::ShaderManager::FromFile("quad.vert"),
		ej::ShaderManager::FromFile("quad.frag"));
	m_screenShader = getLocator().get<ej::ShaderManager>()->get("quad");
	m_screenShader->setAttribute(0, "vPosition");
	m_screenShader->setAttribute(1, "vTexCoords");

	glUseProgram(m_screenShader->getHandle());
	m_screenShader->setUniform("uLeftEyeTexture", 0);
	m_screenShader->setUniform("uRightEyeTexture", 1);
	glUseProgram(0);

	m_screenQuad.init(ej::MeshGeometry::createQuad(glm::vec2(1.0f), ej::MeshGeometry::TEXTURED_VERTEX));

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

	const auto renderTargetSize = m_vrManager->getRenderTargetSize();
	for (size_t i = 0; i < 2; ++i) {
		m_eyeFrameBuffers[i].init(renderTargetSize.x, renderTargetSize.y, true);
	}

	m_eyeCameras[0] = std::make_unique<ej::Camera>(
		m_vrManager->getEyeProjectionMatrix(vr::EVREye::Eye_Left, glm::vec2(0.1f, 100.0f)));
	m_eyeCameras[1] = std::make_unique<ej::Camera>(
		m_vrManager->getEyeProjectionMatrix(vr::EVREye::Eye_Right, glm::vec2(0.1f, 100.0f)));

	ej::FrameBuffer::unbind();
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
	if (m_skyboxState == NeedInitialize) {
		m_skybox->init(m_videoStream.getSize());
		m_skyboxState = Initialized;
	}

	if (m_skyboxState == Initialized) {
		if (m_videoStream.hasData()) {
			m_skybox->updateTexture(&m_videoStream);
		}
	}

	m_vrManager->update();

	if (m_vrManager->isHmdConnected()) {
		m_cameraTransform.setPosition(m_vrManager->getHmdPosition());
		m_cameraTransform.setRotation(m_vrManager->getHmdRotation());
		
		m_eyeCameras[0]->updateView(m_cameraTransform.getTransformationMatrix() *
			m_vrManager->getEyeToHeadTransform(vr::Eye_Left));
		m_eyeCameras[1]->updateView(m_cameraTransform.getTransformationMatrix()  *
			m_vrManager->getEyeToHeadTransform(vr::Eye_Right));
	}

	for (size_t i = 0; i < 2; ++i) {
		drawScene(*m_eyeCameras[i], m_eyeFrameBuffers[i]);
	}
	ej::FrameBuffer::unbind();

	drawHmd();

	const auto& windowSize = m_windowManager->getWindow().getSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	m_eyeFrameBuffers[0].getColorTexture().bind(0);
	m_eyeFrameBuffers[1].getColorTexture().bind(1);
	glUseProgram(m_screenShader->getHandle());
	m_screenQuad.draw();
	glUseProgram(0);
}

void MainScene::initManagers()
{
	m_vrManager = getLocator().get<ej::VRManager>();
	m_vrManager->connect();

	m_inputManager = getLocator().get<ej::InputManager>();

	m_windowManager = getLocator().get<ej::WindowManager>();
}

void MainScene::drawScene(ej::Camera& camera, ej::FrameBuffer& target) const
{
	target.bind();
	glViewport(0, 0, target.getColorTexture().getSize().x, target.getColorTexture().getSize().y);

	glClear(GL_DEPTH_BUFFER_BIT);

	m_skybox->draw(camera, m_cameraTransform);
	m_carpet->draw(camera);
}

void MainScene::drawHmd()
{
	static vr::Texture_t leftEyeTexture = {
		reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeFrameBuffers[0].getColorTexture().getHandle())),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	static vr::Texture_t rightEyeBuffer = {
		reinterpret_cast<void*>(static_cast<uintptr_t>(m_eyeFrameBuffers[1].getColorTexture().getHandle())),
		vr::TextureType_OpenGL,
		vr::ColorSpace_Gamma
	};

	m_vrManager->getCompositor()->Submit(vr::Eye_Left, &leftEyeTexture);
	m_vrManager->getCompositor()->Submit(vr::Eye_Right, &rightEyeBuffer);
	glFlush();
}
