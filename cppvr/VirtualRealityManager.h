#pragma once

#include <OpenVR/openvr.h>

#include "BaseManager.h"

namespace ej
{
	class VirtualRealityManager : public BaseManager
	{
	public:
		VirtualRealityManager(const ManagerLocator& locator);
		~VirtualRealityManager();

		bool checkHmdPresent();
		bool connect();

	private:
		vr::IVRSystem* m_system;
		vr::IVRRenderModels* m_renderModels;
	};
}