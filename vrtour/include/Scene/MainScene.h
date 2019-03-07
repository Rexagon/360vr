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
#include "Scene/SteamVRObject.h"
#include "Rendering/SkyboxMaterial.h"
#include "Rendering/TextureStreamer.h"

namespace app
{
	class MainScene final : public ej::Scene
	{
		struct Target final
		{
			explicit Target(const ej::Core& core) :
				streamer(core)
			{}

			ej::Texture* target = nullptr;
			TextureStreamer streamer;
		};

	public:
		void onInit() override;

		void onUpdate(float dt) override;

	private:
		void drawScene();

		ej::Texture* createVideoTarget(const glm::vec3& position);
		void createSkyBox();
		void createCamera();

		ej::VRManager* m_vrManager = nullptr;
		ej::InputManager* m_inputManager = nullptr;
		ej::WindowManager* m_windowManager = nullptr;
		ej::RenderingManager* m_renderingManager = nullptr;

		std::vector<std::unique_ptr<Video>> m_videos;
		std::unordered_map<std::string, Target> m_targets;

		std::pair<ej::MeshEntity, std::unique_ptr<SkyBoxMaterial>> m_skyBox{};

		std::unique_ptr<HeadSet> m_headSet;
		std::unordered_map<ej::VRDeviceIndex, std::unique_ptr<SteamVRObject>> m_controllers;

		std::unique_ptr<DebugCamera> m_debugCamera;
	};
}
