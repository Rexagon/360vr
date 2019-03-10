#pragma once

#include <memory>

#include <Scene/MeshEntity.h>
#include <Managers/VRManager.h>
#include <Managers/InputManager.h>
#include <Managers/SceneManager.h>
#include <Managers/WindowManager.h>
#include <Managers/RenderingManager.h>

#include <Rendering/ForwardRenderer.h>

#include "Scene/StateGraph.h"
#include "Scene/CollisionWorld.h"

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
		void createWireFrame();
		void createMarker();
		void createCamera();

		void updateTransition(float dt);

		ej::VRManager* m_vrManager = nullptr;
		ej::InputManager* m_inputManager = nullptr;
		ej::WindowManager* m_windowManager = nullptr;
		ej::RenderingManager* m_renderingManager = nullptr;

		StateGraph<Target> m_stateGraph;

		SkyBoxMaterial* m_skyBoxMaterial = nullptr;
		std::vector<std::pair<ej::MeshEntity, std::unique_ptr<ej::Material>>> m_entities;

		std::unordered_map<ej::VRDeviceIndex, std::unique_ptr<SteamVRObject>> m_controllers;

		std::unique_ptr<HeadSet> m_headSet;
		std::unique_ptr<DebugCamera> m_debugCamera;

		CollisionWorld m_collisionWorld;

		ej::ForwardRenderer* m_forwardRenderer = nullptr;
	};
}
