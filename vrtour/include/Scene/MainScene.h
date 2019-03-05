#pragma once

#include <memory>

#include <Scene/MeshEntity.h>
#include <Managers/VRManager.h>
#include <Managers/InputManager.h>
#include <Managers/SceneManager.h>
#include <Managers/WindowManager.h>
#include <Managers/RenderingManager.h>

#include "Video/Video.h"
#include "Scene/HeadSet.h"
#include "Scene/DebugCamera.h"
#include "Rendering/TextureStreamer.h"

class MainScene : public ej::Scene
{
	struct VideoData
	{
		VideoData(std::unique_ptr<Video> video, ej::Texture* target, std::unique_ptr<TextureStreamer> streamer) :
			video(std::move(video)), target(target), streamer(std::move(streamer))
		{}

		std::unique_ptr<Video> video;
		ej::Texture* target;
		std::unique_ptr<TextureStreamer> streamer;
	};

public:
	void onInit() override;

	void onUpdate(float dt) override;

private:
	void drawScene();

	ej::Texture* createVideoTarget(const glm::vec3& position);
	ej::Texture* createSkyBox();
	void createCamera();

	ej::VRManager* m_vrManager{ nullptr };
	ej::InputManager* m_inputManager{ nullptr };
	ej::WindowManager* m_windowManager{ nullptr };
	ej::RenderingManager* m_renderingManager{ nullptr };

	std::vector<VideoData> m_videos;

	std::vector<std::pair<ej::MeshEntity, std::unique_ptr<ej::Material>>> m_entities;

	std::unique_ptr<HeadSet> m_headSet;
	std::unique_ptr<DebugCamera> m_debugCamera;
};
