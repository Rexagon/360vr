#pragma once

#include <memory>
#include <unordered_map>

#include "VRManager.h"
#include "InputManager.h"
#include "SceneManager.h"
#include "VideoManager.h"
#include "WindowManager.h"
#include "RenderingManager.h"

#include "Model.h"
#include "Skybox.h"
#include "HeadSet.h"
#include "DebugCamera.h"
#include "SteamVRObject.h"
#include "TextureStreamer.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	void initManagers();
	void drawScene(const ej::Camera& camera, const ej::Transform& cameraTransform) const;

	VideoManager::ptr m_videoManager;
	ej::VRManager::ptr m_vrManager;
	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	ej::RenderingManager::ptr m_renderingManager;

	Video::ptr m_video;
	TextureStreamer::ptr m_textureStreamer;

	ej::Transform m_meshTransform;

	std::unique_ptr<Model> m_model;
	std::unique_ptr<Skybox> m_skybox;

	std::unique_ptr<HeadSet> m_headSet;
	std::unique_ptr<DebugCamera> m_debugCamera;

	std::unordered_map<std::string, std::unique_ptr<SteamVRObject>> m_controllers;
};
