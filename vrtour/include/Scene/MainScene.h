#pragma once

#include <tuple>
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
		class Target final
		{
		public:
			Target(ej::Texture* texture, 
				std::unique_ptr<TextureStreamer> streamer,
				std::unique_ptr<Video> video);

			void write() const;

			ej::Texture* getTexture() const;

		private:
			ej::Texture* m_texture = nullptr;
			std::unique_ptr<TextureStreamer> m_streamer;
			std::unique_ptr<Video> m_video;
		};

	public:
		void onInit() override;

		void onUpdate(float dt) override;

	private:
		void drawScene();

		void createSkyBox();
		void createCamera();

		void prepareTransition(const std::string& begin, const std::string& end);

		ej::VRManager* m_vrManager = nullptr;
		ej::InputManager* m_inputManager = nullptr;
		ej::WindowManager* m_windowManager = nullptr;
		ej::RenderingManager* m_renderingManager = nullptr;

		std::unordered_map<std::string, Target> m_targets;

		std::unordered_map<std::string, std::string> m_transitions;

		std::pair<std::string, std::string> m_currentTransition;
		std::pair<Target*, Target*> m_transitionPair{nullptr, nullptr};

		std::pair<ej::MeshEntity, std::unique_ptr<SkyBoxMaterial>> m_skyBox{};

		std::unique_ptr<HeadSet> m_headSet;
		std::unordered_map<ej::VRDeviceIndex, std::unique_ptr<SteamVRObject>> m_controllers;

		std::unique_ptr<DebugCamera> m_debugCamera;
	};
}
