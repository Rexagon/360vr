#pragma once

#include <memory>

#include <Scene/MeshEntity.h>
#include <Managers/InputManager.h>
#include <Managers/SceneManager.h>
#include <Managers/WindowManager.h>
#include <Managers/RenderingManager.h>

#include "Video/Video.h"
#include "Scene/DebugCamera.h"
#include "Rendering/TextureStreamer.h"

class MainScene : public ej::Scene
{
	struct VideoData
	{
		VideoData(Video::ptr video, ej::Texture* target, TextureStreamer::ptr streamer) :
			video(video), target(target), streamer(streamer)
		{}

		Video::ptr video;
		ej::Texture* target;
		TextureStreamer::ptr streamer;
	};

public:
	void onInit() override;

	void onUpdate(float dt) override;

private:
	void drawScene();

	ej::Texture* createVideoTarget(const glm::vec3& position);
	ej::Texture* createSkyBox();
	void createCamera();

	ej::InputManager::ptr m_inputManager;
	ej::WindowManager::ptr m_windowManager;
	ej::RenderingManager::ptr m_renderingManager;

	std::vector<VideoData> m_videos;
	
	std::vector<ej::MeshEntity::ptr> m_meshes;

	std::unique_ptr<DebugCamera> m_debugCamera;
};
