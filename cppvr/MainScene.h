#pragma once

#include <memory>
#include <mutex>

#include "VRManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "WindowManager.h"

#include "Skybox.h"
#include "Camera.h"
#include "Transform.h"
#include "FrameBuffer.h"
#include "Carpet.h"

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
	void initManagers();
	void drawScene(ej::Camera& camera, ej::FrameBuffer& target) const;
	void drawHmd();

	std::unique_ptr<ej::Camera> m_eyeCameras[2];
	ej::FrameBuffer m_eyeFrameBuffers[2];

	std::shared_ptr<ej::VRManager> m_vrManager;
	std::shared_ptr<ej::InputManager> m_inputManager;
	std::shared_ptr<ej::WindowManager> m_windowManager;

	ej::Transform m_cameraTransform;

	std::unique_ptr<Carpet> m_carpet;
	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<std::thread> m_streamingThread;

	std::shared_ptr<ej::Shader> m_screenShader;
	ej::Mesh m_screenQuad;
	
	VideoStream m_videoStream;
	bool m_receivingVideo = false;
};
