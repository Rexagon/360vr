#include "VRManager.h"

using namespace ej;

glm::mat4 toGLM(const vr::HmdMatrix34_t& m)
{
	glm::mat4 result(1.0f);

	result[0][0] = m.m[0][0];
	result[1][0] = m.m[1][0];
	result[2][0] = m.m[2][0];

	result[0][1] = m.m[0][1];
	result[1][1] = m.m[1][1];
	result[2][1] = m.m[2][1];

	result[0][2] = m.m[0][2];
	result[1][2] = m.m[1][2];
	result[2][2] = m.m[2][2];

	result[0][3] = m.m[0][3];
	result[1][3] = m.m[1][3];
	result[2][3] = m.m[2][3];

	return result;
}

VRManager::VRManager(const Core& core) :
	BaseManager(core), m_system(nullptr), m_compositor(nullptr), m_renderModels(nullptr),
	m_isHmdConnected(false), m_hmdDeviceIndex(-1)
{
}

VRManager::~VRManager()
{
	if (m_system == nullptr)
		return;

	vr::VR_Shutdown();
}

bool VRManager::connect()
{
	// IVRSystem initialization
	vr::HmdError error;
	m_system = VR_Init(&error, vr::EVRApplicationType::VRApplication_Scene);

	if (m_system == nullptr) {
		throw std::runtime_error("IVRSystem initialization error: " + std::to_string(error));
	}

	// IVRRenderModels initializations
	vr::EVRInitError interfaceInitError;
	m_renderModels = static_cast<vr::IVRRenderModels*>(VR_GetGenericInterface(vr::IVRRenderModels_Version, &interfaceInitError));
	if (m_renderModels == nullptr) {
		throw std::runtime_error("IVRRenderModels initialization error: " + std::to_string(interfaceInitError));
	}

	m_compositor = vr::VRCompositor();

	updateControllersInfo();

	return true;
}

void VRManager::update()
{
	m_compositor->WaitGetPoses(&m_trackedDevicePoses[0], vr::k_unMaxTrackedDeviceCount, nullptr, 0);

	vr::VREvent_t event;
	while (m_system->PollNextEvent(&event, sizeof(event))) {
		processEvent(event);
	}

	float fSecondsSinceLastVsync;
	m_system->GetTimeSinceLastVsync(&fSecondsSinceLastVsync, nullptr);

	float fDisplayFrequency = m_system->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
	float fFrameDuration = 1.f / fDisplayFrequency;
	float fVsyncToPhotons = m_system->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);

	float fPredictedSecondsFromNow = fFrameDuration - fSecondsSinceLastVsync + fVsyncToPhotons;

	for (VRDeviceIndex i = 0; i < DEVICE_COUNT; ++i) {
		if (!m_system->IsTrackedDeviceConnected(i)) {
			continue;
		}

		vr::TrackedDevicePose_t& devicePose = m_trackedDevicePoses[i];

		switch (vr::VRSystem()->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_HMD:
		case vr::TrackedDeviceClass_Controller:
		{
			vr::VRSystem()->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, fPredictedSecondsFromNow, &devicePose, DEVICE_COUNT);

			m_trackedDeviceMatrices[i] = toGLM(devicePose.mDeviceToAbsoluteTracking);
		}
		break;

		default:
			break;
		}
	}
}

size_t VRManager::getControllerCount() const
{
	return m_controllerDevicesIndices.size();
}

std::vector<VRDeviceIndex> VRManager::getControllerIndices() const
{
	return m_controllerDevicesIndices;
}

glm::mat4 VRManager::getEyeProjectionMatrix(vr::EVREye eye, glm::vec2 range) const
{
	const auto& mat = m_system->GetProjectionMatrix(eye, range.x, range.y);

	return glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], mat.m[3][0],
		mat.m[0][1], mat.m[1][1], mat.m[2][1], mat.m[3][1],
		mat.m[0][2], mat.m[1][2], mat.m[2][2], mat.m[3][2],
		mat.m[0][3], mat.m[1][3], mat.m[2][3], mat.m[3][3]
	);
}

glm::mat4 VRManager::getEyeToHeadTransform(vr::EVREye eye) const
{
	return toGLM(m_system->GetEyeToHeadTransform(eye));
}

glm::vec3 VRManager::getDevicePosition(VRDeviceIndex device) const
{
	const auto& m = m_trackedDeviceMatrices[device];
	glm::vec3 result;
	result.x = m[0][3];
	result.y = m[1][3];
	result.z = m[2][3];
	return result;
}

glm::quat VRManager::getDeviceRotation(VRDeviceIndex device) const
{
	const auto& m = m_trackedDeviceMatrices[device];

	glm::quat rotation;
	rotation.w = sqrt(fmax(0.0f, 1.0f + m[0][0] + m[1][1] + m[2][2])) / 2.0f;
	rotation.x = sqrt(fmax(0.0f, 1.0f + m[0][0] - m[1][1] - m[2][2])) / 2.0f;
	rotation.y = sqrt(fmax(0.0f, 1.0f - m[0][0] + m[1][1] - m[2][2])) / 2.0f;
	rotation.z = sqrt(fmax(0.0f, 1.0f - m[0][0] - m[1][1] + m[2][2])) / 2.0f;

	rotation.x = copysign(rotation.x, m[2][1] - m[1][2]);
	rotation.y = copysign(rotation.y, m[0][2] - m[2][0]);
	rotation.z = copysign(rotation.z, m[1][0] - m[0][1]);

	return rotation;
}

glm::mat4 VRManager::getDeviceTransformation(VRDeviceIndex device) const
{
	return m_trackedDeviceMatrices[device];
}

glm::vec3 VRManager::getHmdPosition() const
{
	if (!m_isHmdConnected) {
		return glm::vec3();
	}

	return getDevicePosition(m_hmdDeviceIndex);
}

glm::quat VRManager::getHmdRotation() const
{
	if (!m_isHmdConnected) {
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	return getDeviceRotation(m_hmdDeviceIndex);
}

glm::mat4 VRManager::getHmdTransformation() const
{
	return m_trackedDeviceMatrices[m_hmdDeviceIndex];
}

glm::uvec2 VRManager::getRenderTargetSize() const
{
	uint32_t width, height;
	m_system->GetRecommendedRenderTargetSize(&width, &height);
	return glm::uvec2(width, height);
}

bool VRManager::isHmdConnected() const
{
	return m_isHmdConnected;
}

vr::IVRCompositor* VRManager::getCompositor() const
{
	return m_compositor;
}

bool VRManager::checkHmdPresent()
{
	return vr::VR_IsHmdPresent();
}

bool VRManager::processEvent(const vr::VREvent_t& event)
{
	switch (event.eventType) {
	case vr::VREvent_TrackedDeviceActivated:
	case vr::VREvent_TrackedDeviceDeactivated:
		updateControllersInfo();
		break;

	case vr::VREvent_ButtonPress:
		m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, true);
		break;

	case vr::VREvent_ButtonUnpress:
		m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, false);
		break;
	}

	return true;
}

void VRManager::updateControllersInfo()
{
	m_isHmdConnected = false;
	m_controllerDevicesIndices.clear();

	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (vr::VRSystem()->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_Controller:
			m_controllerDevicesIndices.push_back(i);
			break;

		case vr::TrackedDeviceClass_HMD:
			m_isHmdConnected = true;
			m_hmdDeviceIndex = i;
			break;

		case vr::TrackedDeviceClass_Invalid:
			break;

		default: break;
		}
	}
}
