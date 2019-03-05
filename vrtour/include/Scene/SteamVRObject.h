#pragma once

#include <string>

#include <OpenVR/openvr.h>

#include <Managers/MeshManager.h>
#include <Managers/RenderingManager.h>

class SteamVRObject final
{
public:
	SteamVRObject(const ej::Core& core, const std::string& name);
	~SteamVRObject();

	ej::MeshEntity* getMeshEntity();

private:
	void tryLoad();

	ej::MeshManager* m_meshManager{nullptr};
	ej::RenderingManager* m_renderingManager{nullptr};

	bool m_isInitialized{false};
	std::string m_name;

	ej::MeshEntity m_meshEntity{nullptr, nullptr};

	vr::RenderModel_t* m_renderModel{nullptr};
	vr::IVRRenderModels* m_iVRRenderModels{nullptr};
};
