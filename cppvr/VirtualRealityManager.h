#pragma once

#include <OpenVR/openvr.h>

#include "BaseManager.h"

namespace core
{

class VirtualRealityManager : public BaseManager
{
public:
	VirtualRealityManager();
	~VirtualRealityManager();

	bool checkHmdPresent();
	bool connect();

private:
	vr::IVRSystem* m_system;
	vr::IVRRenderModels* m_renderModels;
};

}