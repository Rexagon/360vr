#pragma once

#include <memory>

#include "InputManager.h"
#include "SceneManager.h"
#include "VideoManager.h"
#include "WindowManager.h"
#include "RenderingManager.h"

#include "HeadSet.h"
#include "MeshEntity.h"
#include "DebugCamera.h"
#include "TextureStreamer.h"
#include "SimpleMeshMaterial.h"

class MainScene : public ej::Scene
{
public:
	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	void drawScene();

	VideoManager::ptr m_videoManager;
	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	ej::RenderingManager::ptr m_renderingManager;

	Video::ptr m_video;
	TextureStreamer::ptr m_textureStreamer;
	SimpleMeshMaterial::ptr m_videoTarget;
	
	std::vector<ej::MeshEntity::ptr> m_meshes;

	std::unique_ptr<HeadSet> m_headSet;
	std::unique_ptr<DebugCamera> m_debugCamera;
};
