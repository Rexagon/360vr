#pragma once

#include <mutex>
#include <memory>
#include <atomic>

#include "VRManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "WindowManager.h"

#include "Skybox.h"
#include "Carpet.h"
#include "HeadSet.h"
#include "SteamVRObject.h"
#include "RotationManager.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	void initManagers();
	void drawScene(vr::EVREye eye) const;

	ej::VRManager::ptr m_vrManager;
	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	RotationManager::ptr m_rotationManager;

	ej::Transform m_carpetTransform;

	std::unique_ptr<Carpet> m_carpet;
	std::unique_ptr<Skybox> m_skybox;
	std::unique_ptr<HeadSet> m_headSet;

	std::unique_ptr<SteamVRObject> m_viveController;
	std::unique_ptr<SteamVRObject> m_oculusLeftController;
	std::unique_ptr<SteamVRObject> m_oculusRightController;

	std::unique_ptr<std::thread> m_streamingThread;

	VideoStream m_videoStream;
	std::atomic_bool m_isConnected;
	std::atomic_bool m_receivingEnabled;
};
