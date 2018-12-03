#pragma once

#include <array>
#include <vector>
#include <bitset>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <OpenVR/openvr.h>

#include "BaseManager.h"

namespace ej
{
	using VRButton = vr::EVRButtonId;
	using VRDeviceIndex = vr::TrackedDeviceIndex_t;

	class VRManager : public BaseManager
	{
	public:
		explicit VRManager(const ManagerLocator& locator);
		~VRManager();

		bool connect();

		void update();

		size_t getControllerCount() const;
		std::vector<VRDeviceIndex> getControllerIndices() const;

		glm::mat4 getEyeProjectionMatrix(vr::EVREye eye, glm::vec2 range) const;
		glm::mat4 getEyeToHeadTransform(vr::EVREye eye) const;

		glm::vec3 getDevicePosition(VRDeviceIndex device) const;
		glm::quat getDeviceRotation(VRDeviceIndex device) const;
		glm::mat4 getDeviceTransformation(VRDeviceIndex device) const;

		glm::vec3 getHmdPosition() const;
		glm::quat getHmdRotation() const;
		glm::mat4 getHmdTransformation() const;

		glm::uvec2 getRenderTargetSize() const;

		bool isHmdConnected() const;

		vr::IVRCompositor* getCompositor() const;

		static bool checkHmdPresent();

	private:
		static const size_t DEVICE_COUNT = vr::k_unMaxTrackedDeviceCount;

		bool processEvent(const vr::VREvent_t & event);
		void updateControllersInfo();

		vr::IVRSystem* m_system;
		vr::IVRCompositor* m_compositor;
		vr::IVRRenderModels* m_renderModels;

		bool m_isHmdConnected;
		VRDeviceIndex m_hmdDeviceIndex;
		std::vector<VRDeviceIndex> m_controllerDevicesIndices;

		std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_currentButtonsState;
		std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_lastButtonsState;

		std::array<vr::TrackedDevicePose_t, DEVICE_COUNT> m_trackedDevicePoses;
		std::array<glm::mat4, DEVICE_COUNT> m_trackedDeviceMatrices;
	};
}
