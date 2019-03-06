#pragma once

#include <string>

#include <OpenVR/openvr.h>

#include <Managers/MeshManager.h>
#include <Managers/TextureManager.h>
#include <Managers/RenderingManager.h>
#include "Rendering/SimpleMeshMaterial.h"

namespace app
{
	class SteamVRObject final
	{
	public:
		SteamVRObject(const ej::Core& core, const std::string& name);
		~SteamVRObject();

		ej::MeshEntity* getMeshEntity();

	private:
		void ensureLoaded();
		bool tryLoadMesh();
		bool tryLoadTexture();

		ej::MeshManager* m_meshManager = nullptr;
		ej::TextureManager* m_textureManager = nullptr;
		ej::RenderingManager* m_renderingManager = nullptr;

		bool m_isInitialized = false;
		std::string m_name;

		ej::MeshEntity m_meshEntity{ nullptr, nullptr };

		ej::Mesh m_mesh;
		std::unique_ptr<ej::Texture> m_texture;
		SimpleMeshMaterial m_material;

		vr::RenderModel_t* m_renderModel = nullptr;
		vr::IVRRenderModels* m_iVRRenderModels = nullptr;

		const ej::Core& m_core;
	};
}
