#pragma once

#include <OpenVR/openvr.h>

class VirtualRealityManager
{
public:
	VirtualRealityManager();
	~VirtualRealityManager();

	bool connect();

private:
	vr::IVRSystem* m_system;
	vr::IVRRenderModels* m_renderModels;
};