#pragma once

#include <memory>
#include <mutex>

#include "InputManager.h"
#include "SceneManager.h"

#include "Skybox.h"
#include "Camera.h"
#include "Transform.h"

class MainScene : public ej::Scene
{
	enum SkyboxState
	{
		Waiting,
		NeedInitialize,
		Initialized
	} m_skyboxState = Waiting;

public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	std::shared_ptr<ej::InputManager> m_inputManager;

	ej::Camera m_camera;
	ej::Transform m_cameraTransform;

	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<std::thread> m_streamingThread;

	VideoStream m_videoStream;
	bool m_receivingVideo = false;
};