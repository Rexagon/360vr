#include "MainScene.h"

#include <thread>
#include <GL/glew.h>

void MainScene::onInit()
{
	initManagers();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_carpet = std::make_unique<Carpet>(getCore());
	m_skybox = std::make_unique<Skybox>(getCore());
	m_headSet = std::make_unique<HeadSet>(getCore());

	m_viveController = std::make_unique<SteamVRObject>(getCore(), 
		"vr_controller_vive_1_5");
	m_oculusLeftController = std::make_unique<SteamVRObject>(getCore(),
		"oculus_cv1_controller_left");
	m_oculusRightController = std::make_unique<SteamVRObject>(getCore(),
		"oculus_cv1_controller_right");

	m_receivingEnabled = false;
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
	m_chairManager->close();
}

void MainScene::onUpdate(const float dt)
{
	if (m_isConnected && m_videoStream.hasData()) {
		m_skybox->updateTexture(&m_videoStream);
	}

	m_vrManager->update();

	m_headSet->update(dt);

	m_carpetTransform.setRotation(m_rotationManager->getState());

	drawScene(vr::Eye_Left);
	drawScene(vr::Eye_Right);
	m_headSet->submit();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	const auto& windowSize = m_windowManager->getWindow().getSize();
	glViewport(0, 0, windowSize.x, windowSize.y);

	m_headSet->drawDebug();
}

void MainScene::initManagers()
{
	m_vrManager = getCore().get<ej::VRManager>();
	m_inputManager = getCore().get<ej::InputManager>();
	m_windowManager = getCore().get<ej::WindowManager>();
	m_rotationManager = getCore().get<RotationManager>();
	m_chairManager = getCore().get<ChairManager>();

	try {
		m_vrManager->connect();
	}
	catch (const std::runtime_error& e) {
		printf("ERROR: %s\n", e.what());
	}

	m_rotationManager->start();

	try {
		m_chairManager->init(5);
	}
	catch (const std::runtime_error& e) {
		printf("ERROR: %s\n", e.what());
	}
}

void MainScene::drawScene(vr::EVREye eye) const
{
	const auto& camera = m_headSet->bindEye(eye);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_skybox->getTexture()->bind(3);

	m_carpet->draw(camera, m_headSet->getEyeTransform(eye), m_carpetTransform);

	if (m_vrManager->getControllerCount() > 0) {
		for (const auto& index : m_vrManager->getControllerIndices()) {
			SteamVRObject* model = nullptr;

			switch (m_vrManager->getControllerRole(index)) {
			case vr::TrackedControllerRole_OptOut:
				model = m_viveController.get();
				break;
			case vr::TrackedControllerRole_LeftHand:
				model = m_oculusLeftController.get();
				break;
			case vr::TrackedControllerRole_RightHand:
				model = m_oculusRightController.get();
				break;
			default:
				break;
			}

			if (model) {
				model->draw(camera, m_headSet->getEyeTransform(eye), m_vrManager->getDeviceTransformation(index));
			}
		}
	}

	m_skybox->draw(camera, m_headSet->getTransform());
}