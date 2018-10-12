#include "VirtualRealityManager.h"

VirtualRealityManager::VirtualRealityManager() :
	m_system(nullptr), m_renderModels(nullptr)
{
}

VirtualRealityManager::~VirtualRealityManager()
{
	if (m_system == nullptr)
		return;

	vr::VR_Shutdown();
}

bool VirtualRealityManager::connect()
{
	if (!vr::VR_IsHmdPresent())
		throw std::runtime_error("HMD is not present");

	// IVRSystem initialization
	vr::HmdError error;
	m_system = vr::VR_Init(&error, vr::EVRApplicationType::VRApplication_Scene);

	if (m_system == nullptr) {
		throw std::runtime_error("IVRSystem initialization error: " + std::to_string(error));
	}

	// IVRRenderModels initializations
	vr::EVRInitError interfaceInitError;
	m_renderModels = static_cast<vr::IVRRenderModels*>(vr::VR_GetGenericInterface(vr::IVRRenderModels_Version, &interfaceInitError));
	if (m_renderModels == nullptr) {
		throw std::runtime_error("IVRRenderModels initialization error: " + std::to_string(interfaceInitError));
	}

	return true;
}
