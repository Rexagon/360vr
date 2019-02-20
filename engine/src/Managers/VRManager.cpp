#include "Managers/VRManager.h"

#include <thread>

using namespace ej;

glm::mat4 toGLM(const vr::HmdMatrix34_t& mat)
{
	return glm::mat4(
		mat.m[0][0], mat.m[1][0], mat.m[2][0], 0.0f,
		mat.m[0][1], mat.m[1][1], mat.m[2][1], 0.0f,
		mat.m[0][2], mat.m[1][2], mat.m[2][2], 0.0f,
		mat.m[0][3], mat.m[1][3], mat.m[2][3], 1.0f
	);
}

VRManager::VRManager(const Core& core) :
	BaseManager(core), m_system(nullptr), m_compositor(nullptr), m_renderModels(nullptr),
	m_isHmdConnected(false), m_hmdDeviceIndex(-1), m_trackedDevicePoses(), m_trackedDeviceMatrices(), m_isInitialized(false) 
{
}

VRManager::~VRManager()
{
	if (!m_isInitialized)
		return;

	vr::VR_Shutdown();
}

void VRManager::connect()
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

	m_isInitialized = true;

	updateControllersInfo();
}

void VRManager::update()
{
	if (!m_isInitialized) {
		return;
	}

	m_compositor->WaitGetPoses(&m_trackedDevicePoses[0], vr::k_unMaxTrackedDeviceCount, nullptr, 0);

	vr::VREvent_t event;
	while (m_system->PollNextEvent(&event, sizeof(event))) {
		processEvent(event);
	}

	float secondsSinceLastVsync;
	m_system->GetTimeSinceLastVsync(&secondsSinceLastVsync, nullptr);

	const auto displayFrequency = m_system->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_DisplayFrequency_Float);
	const auto frameDuration = 1.f / displayFrequency;
	const float vsyncToPhotons = m_system->GetFloatTrackedDeviceProperty(vr::k_unTrackedDeviceIndex_Hmd, vr::Prop_SecondsFromVsyncToPhotons_Float);

	const float predictedSecondsFromNow = frameDuration * 2.0f - secondsSinceLastVsync + vsyncToPhotons;

	//printf("%f\n", predictedSecondsFromNow);

	m_system->GetDeviceToAbsoluteTrackingPose(vr::TrackingUniverseStanding, predictedSecondsFromNow, &m_trackedDevicePoses[0], DEVICE_COUNT);

	for (VRDeviceIndex i = 0; i < DEVICE_COUNT; ++i) {
		vr::TrackedDevicePose_t& devicePose = m_trackedDevicePoses[i];

		switch (m_system->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_HMD:
		case vr::TrackedDeviceClass_Controller:
		{
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
	if (!m_isInitialized) {
		return 0;
	}

	return m_controllerDevicesIndices.size();
}

const std::vector<VRDeviceIndex>& VRManager::getControllerIndices() const
{
	return m_controllerDevicesIndices;
}

glm::mat4 VRManager::getEyeProjectionMatrix(vr::EVREye eye, glm::vec2 range) const
{
	if (!m_isInitialized) {
		return glm::mat4(1.0f);
	}

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
	if (!m_isInitialized) {
		return glm::mat4(1.0f);
	}

	return toGLM(m_system->GetEyeToHeadTransform(eye));
}

glm::vec3 VRManager::getDevicePosition(VRDeviceIndex device) const
{
	if (!m_isInitialized) {
		return glm::vec3();
	}

	const auto& m = m_trackedDeviceMatrices[device];
	glm::vec3 result;
	result.x = m[3][0];
	result.y = m[3][1];
	result.z = m[3][2];
	return result;
}

glm::quat VRManager::getDeviceRotation(VRDeviceIndex device) const
{
	if (!m_isInitialized) {
		return glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
	}

	const auto& m = m_trackedDeviceMatrices[device];

	glm::quat rotation;
	rotation.w = sqrt(fmax(0.0f, 1.0f + m[0][0] + m[1][1] + m[2][2])) / 2.0f;
	rotation.x = sqrt(fmax(0.0f, 1.0f + m[0][0] - m[1][1] - m[2][2])) / 2.0f;
	rotation.y = sqrt(fmax(0.0f, 1.0f - m[0][0] + m[1][1] - m[2][2])) / 2.0f;
	rotation.z = sqrt(fmax(0.0f, 1.0f - m[0][0] - m[1][1] + m[2][2])) / 2.0f;

	rotation.x = copysign(rotation.x, m[1][2] - m[2][1]);
	rotation.y = copysign(rotation.y, m[2][0] - m[0][2]);
	rotation.z = copysign(rotation.z, m[0][1] - m[1][0]);

	return rotation;
}

glm::mat4 VRManager::getDeviceTransformation(VRDeviceIndex device) const
{
	if (!m_isInitialized) {
		return glm::mat4(1.0f);
	}

	return m_trackedDeviceMatrices[device];
}

vr::ETrackedControllerRole ej::VRManager::getControllerRole(VRDeviceIndex device)
{
	if (!m_isInitialized) {
		return vr::ETrackedControllerRole::TrackedControllerRole_Invalid;
	}

	return m_system->GetControllerRoleForTrackedDeviceIndex(device);
}

std::string ej::VRManager::getDeviceRenderModelName(VRDeviceIndex device)
{
	if (!m_isInitialized) {
		return {};
	}

	char temp[128];
	const auto len = m_system->GetStringTrackedDeviceProperty(device, vr::Prop_RenderModelName_String, temp, 128);
	if (len == 0) {
		return {};
	}

	std::string result;
	result.resize(len - 1);
	std::memcpy(result.data(), temp, len - 1);

	return result;
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
	if (!m_isHmdConnected) {
		return glm::mat4(1.0f);
	}

	return m_trackedDeviceMatrices[m_hmdDeviceIndex];
}

glm::uvec2 VRManager::getRenderTargetSize() const
{
	if (!m_isInitialized) {
		return glm::uvec2(0.0f, 0.0f);
	}

	uint32_t width, height;
	m_system->GetRecommendedRenderTargetSize(&width, &height);
	return glm::uvec2(width, height);
}

bool VRManager::isInitialized() const
{
	return m_isInitialized;
}

bool VRManager::isHmdConnected() const
{
	return m_isHmdConnected;
}

vr::IVRCompositor* VRManager::getCompositorInterface() const
{
	return m_compositor;
}

vr::IVRRenderModels* VRManager::getRenderModelsInterface() const
{
	return m_renderModels;
}

bool VRManager::checkHmdPresent()
{
	return vr::VR_IsHmdPresent();
}

void VRManager::processEvent(const vr::VREvent_t& event)
{
	if (!m_isInitialized) {
		return;
	}

	switch (event.eventType) {
	case vr::VREvent_TrackedDeviceActivated:
	case vr::VREvent_TrackedDeviceDeactivated:
		updateControllersInfo();
		break;

	case vr::VREvent_ButtonPress:
		if (event.trackedDeviceIndex >= 0 && event.trackedDeviceIndex < DEVICE_COUNT) {
			m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, true);
		}
		break;

	case vr::VREvent_ButtonUnpress:
		if (event.trackedDeviceIndex >= 0 && event.trackedDeviceIndex < DEVICE_COUNT) {
			m_currentButtonsState[event.trackedDeviceIndex].set(event.data.controller.button, false);
		}
		break;
	}
}

void VRManager::updateControllersInfo()
{
	if (!m_isInitialized) {
		return;
	}

	m_isHmdConnected = false;
	m_controllerDevicesIndices.clear();

	for (uint32_t i = 0; i < vr::k_unMaxTrackedDeviceCount; i++)
	{
		switch (vr::VRSystem()->GetTrackedDeviceClass(i))
		{
		case vr::TrackedDeviceClass_Controller:
			m_controllerDevicesIndices.push_back(i);
			printf("Controllers index: %d\n", i);
			break;

		case vr::TrackedDeviceClass_HMD:
			m_isHmdConnected = true;
			m_hmdDeviceIndex = i;
			printf("HMD index: %d\n", i);
			break;

		case vr::TrackedDeviceClass_Invalid:
			break;

		default: break;
		}
	}
}
