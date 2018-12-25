#include "MainScene.h"

#include <GL/glew.h>

#include "Core.h"

void MainScene::onInit()
{
	initManagers();

	m_renderingManager->apply();

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
}

void MainScene::onClose()
{
}

void MainScene::onUpdate(const float dt)
{
	m_vrManager->update();
	
	if (m_vrManager->isHmdConnected()) {
		m_headSet->update(dt);

		for (const auto& index : m_vrManager->getControllerIndices()) {
			const auto name = m_vrManager->getDeviceRenderModelName(index);
			m_controllers.try_emplace(name, std::make_unique<SteamVRObject>(getCore(), name));
		}

		for (size_t i = 0; i < 2; ++i) {
			const auto eye = static_cast<vr::EVREye>(i);
			drawScene(m_headSet->bindEye(eye), m_headSet->getEyeTransform(eye));
		}
		m_headSet->submit();

		m_renderingManager->setCurrentFrameBuffer(nullptr);
		const auto& windowSize = m_windowManager->getWindow().getSize();
		m_renderingManager->setViewport(0, 0, windowSize.x, windowSize.y);
		
		m_headSet->drawDebug();
	}
	else {
		m_renderingManager->setCurrentFrameBuffer(nullptr);
		const auto& windowSize = m_windowManager->getWindow().getSize();
		m_renderingManager->setViewport(0, 0, windowSize.x, windowSize.y);

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
	m_renderingManager = getCore().get<ej::RenderingManager>();

	try {
		//if (m_vrManager->checkHmdPresent()) {
		//	m_vrManager->connect();
		//}
	}
	catch (const std::runtime_error& e) {
		printf("ERROR: %s\n", e.what());
	}
}

void MainScene::drawScene(const ej::Camera& camera, const ej::Transform& cameraTransform) const
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_skybox->getTexture()->bind(3);

	m_model->draw(camera, cameraTransform, m_meshTransform);

	if (m_vrManager->getControllerCount() > 0) {
		for (const auto& index : m_vrManager->getControllerIndices()) {
			if (m_vrManager->getControllerRole(index) == vr::TrackedControllerRole_Invalid) {
				continue;
			}

			SteamVRObject* model = nullptr;

			const auto name = m_vrManager->getDeviceRenderModelName(index);
			const auto it = m_controllers.find(name);
			if (it != m_controllers.end()) {
				model = it->second.get();
			}

			if (model) {
				model->draw(camera, cameraTransform, m_vrManager->getDeviceTransformation(index));
			}
		}
	}

	if (m_vrManager->isHmdConnected()) {
		m_skybox->draw(camera, m_headSet->getTransform());
	}
	else {
		m_skybox->draw(camera, cameraTransform);
	}
}