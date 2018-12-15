#include "MainScene.h"

#include <thread>
#include <GL/glew.h>

void MainScene::onInit()
{
	initManagers();

	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	m_model = std::make_unique<Model>(getCore());
	m_skybox = std::make_unique<Skybox>(getCore());

	m_debugCamera = std::make_unique<DebugCamera>(getCore());
	m_debugCamera->getTransform().setPosition(0.0f, 1.0f, 0.0f);

	if (m_vrManager->isInitialized()) {
		m_headSet = std::make_unique<HeadSet>(getCore());
	}
	else {
		m_windowManager->getWindow().setVerticalSyncEnabled(true);
	}

	m_viveController = std::make_unique<SteamVRObject>(getCore(), 
		"vr_controller_vive_1_5");
	m_oculusLeftController = std::make_unique<SteamVRObject>(getCore(),
		"oculus_cv1_controller_left");
	m_oculusRightController = std::make_unique<SteamVRObject>(getCore(),
		"oculus_cv1_controller_right");
}

void MainScene::onClose()
{
}

void MainScene::onUpdate(const float dt)
{
	m_vrManager->update();
	
	if (m_vrManager->isHmdConnected()) {
		m_headSet->update(dt);

		for (size_t i = 0; i < 2; ++i) {
			const auto eye = static_cast<vr::EVREye>(i);
			drawScene(m_headSet->bindEye(eye), m_headSet->getEyeTransform(eye));
		}
		m_headSet->submit();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		const auto& windowSize = m_windowManager->getWindow().getSize();
		glViewport(0, 0, windowSize.x, windowSize.y);
		
		m_headSet->drawDebug();
	}
	else {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		const auto& windowSize = m_windowManager->getWindow().getSize();
		glViewport(0, 0, windowSize.x, windowSize.y);

		m_debugCamera->update(dt);

		drawScene(m_debugCamera->getCamera(), m_debugCamera->getTransform());
	}

	if (m_inputManager->getKeyDown(ej::Key::Escape)) {
		getCore().get<ej::SceneManager>()->removeScene();
	}
}

void MainScene::initManagers()
{
	m_vrManager = getCore().get<ej::VRManager>();
	m_inputManager = getCore().get<ej::InputManager>();
	m_windowManager = getCore().get<ej::WindowManager>();

	try {
		//m_vrManager->connect();
	}
	catch (const std::runtime_error& e) {
		printf("ERROR: %s\n", e.what());
	}
}

void MainScene::drawScene(const ej::Camera& camera, const ej::Transform& cameraTransform) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_skybox->draw(camera, cameraTransform);
	m_skybox->getTexture()->bind(3);

	m_model->draw(camera, cameraTransform, m_meshTransform);

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
				model->draw(camera, cameraTransform, m_vrManager->getDeviceTransformation(index));
			}
		}
	}
}