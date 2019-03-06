#pragma once

#include <array>
#include <vector>
#include <bitset>

#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/quaternion.hpp>
#include <OpenVR/openvr.h>

#include "Core/BaseManager.h"
#include "Resources/MeshGeometry.h"

namespace ej
{
	using VRButton = vr::EVRButtonId;
	using VRDeviceIndex = vr::TrackedDeviceIndex_t;

	/**
	 * \brief Give access to SteamVR API
	 */
	class VRManager final : public BaseManager
	{
	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit VRManager(const Core& core);

		/**
		 * \brief Destructor
		 * 
		 * Shutdown SteamVR
		 */
		~VRManager();

		/**
		 * \brief Initialize SteamVR
		 * 
		 * Must be called before accessing any state
		 */
		void init();

		/**
		 * \brief Update devices
		 * 
		 * This function will block current thread until synchronization
		 * point. 
		 * See SteamVR documentation.
		 */
		void update();

		/**
		 * \brief Check initialization status
		 * \return true if initialized
		 */
		bool isInitialized() const;

		/**
		 * \brief Get active controller count
		 * \return Active controller count
		 */
		size_t getControllerCount() const;

		/**
		 * \brief Get active controllers
		 * \return Const reference to active controllers
		 */
		const std::vector<VRDeviceIndex>& getControllerIndices() const;

		/**
		 * \brief Get active tracking camera count
		 * \return Active tracking camera count
		 */
		size_t getTrackerCount() const;

		/**
		 * \brief Get active tracking cameras
		 * \return Const reference to active tracking cameras
		 */
		const std::vector<VRDeviceIndex>& getTrackerIndices() const;

		/**
		 * \brief Get eye projection
		 * \param eye Left or Right eye
		 * \param range Depth range for projection
		 * \return Projection matrix. Identity matrix if not initialized
		 */
		glm::mat4 getEyeProjectionMatrix(vr::EVREye eye, const glm::vec2& range) const;

		/**
		 * \brief Get eye transformation relative to head
		 * \param eye Left or Right eye
		 * \return Transformation matrix. Identity matrix if not initialized
		 */
		glm::mat4 getEyeToHeadTransform(vr::EVREye eye) const;

		/**
		 * \brief Get device position
		 * \param device Device index
		 * \return Device position. Zero vector if not initialized or device
		 * index is out of range
		 */
		glm::vec3 getDevicePosition(VRDeviceIndex device) const;

		/**
		 * \brief Get device rotation
		 * \param device Device index
		 * \return Device rotation. Identity quaternion if not initialized or
		 * device index is out of range
		 */
		glm::quat getDeviceRotation(VRDeviceIndex device) const;

		/**
		 * \brief Get device transformation
		 * \param device Device index
		 * \return Device transformation matrix. Identity matrix if not
		 * initialized or device index is out of range
		 */
		glm::mat4 getDeviceTransformation(VRDeviceIndex device) const;

		/**
		 * \brief Get controller role
		 * \param device Controller device index
		 * \return Controller role. vr::TrackedControllerRole_Invalid if
		 * not initialized or device index is out of range
		 */
		vr::ETrackedControllerRole getControllerRole(VRDeviceIndex device) const;

		/**
		 * \brief Get device render model name
		 * \param device Device index
		 * \return Render model name. Empty string if not initialized or
		 * device index is out of range
		 */
		std::string getDeviceRenderModelName(VRDeviceIndex device) const;

		/**
		 * \brief Get head mounted device position
		 * \return Position. Zero vector if not initialized
		 */
		glm::vec3 getHmdPosition() const;

		/**
		 * \brief Get head mounted device rotation
		 * \return Rotation. Identity quaternion if not initialized
		 */
		glm::quat getHmdRotation() const;

		/**
		 * \brief Get head mounted device transformation
		 * \return Transformation matrix. Identity matrix if not initialized
		 */
		glm::mat4 getHmdTransformation() const;

		/**
		 * \brief Get eye render target size
		 * \return Render target size
		 */
		glm::uvec2 getRenderTargetSize() const;

		/**
		 * \brief Check head mounted device connection status
		 * \return true if connected
		 */
		bool isHmdConnected() const;

		/**
		 * \brief Get SteamVR compositor interface
		 * \return Compositor interface
		 */
		vr::IVRCompositor* getCompositorInterface() const;

		/**
		 * \brief Get SteamVR render models interface
		 * \return Render models interface
		 */
		vr::IVRRenderModels* getRenderModelsInterface() const;

		/**
		 * \brief Check if system has VR
		 * \return true if it has
		 */
		static bool checkHmdPresent();

	private:
		void processEvent(const vr::VREvent_t & event);
		void updateControllersInfo();

		static const size_t DEVICE_COUNT = vr::k_unMaxTrackedDeviceCount;

		bool m_isInitialized = false;

		vr::IVRSystem* m_system = nullptr;
		vr::IVRCompositor* m_compositor = nullptr;
		vr::IVRRenderModels* m_renderModels = nullptr;

		bool m_isHmdConnected = false;
		VRDeviceIndex m_hmdDeviceIndex = -1;
		std::vector<VRDeviceIndex> m_controllerDevicesIndices;
		std::vector<VRDeviceIndex> m_trackerDevicesIndices;

		std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_currentButtonsState{};
		std::array<std::bitset<vr::k_EButton_Max>, DEVICE_COUNT> m_lastButtonsState{};

		std::array<vr::TrackedDevicePose_t, DEVICE_COUNT> m_trackedDevicePoses{};
		std::array<glm::mat4, DEVICE_COUNT> m_trackedDeviceMatrices{};
	};
}
