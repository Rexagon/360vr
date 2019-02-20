#pragma once

#include <string>

#include <OpenVR/openvr.h>

#include <Managers/MeshManager.h>
#include <Managers/RenderingManager.h>

class SteamVRObject : ej::PointerDefs<SteamVRObject>
{
public:
	SteamVRObject(const ej::Core& core, const std::string& name);
	~SteamVRObject();

	ej::MeshEntity::ptr getMeshEntity() const;

private:
	void tryLoad();

	ej::MeshManager::ptr m_meshManager;
	ej::RenderingManager::ptr m_renderingManager;

	bool m_isInitialized;
	std::string m_name;

	ej::MeshEntity::ptr m_meshEntity;

	vr::RenderModel_t* m_renderModel;
	vr::IVRRenderModels* m_iVRRenderModels;
};
