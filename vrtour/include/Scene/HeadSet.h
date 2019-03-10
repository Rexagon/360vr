#pragma once

#include <Scene/Transform.h>
#include <Scene/CameraEntity.h>
#include <Managers/VRManager.h>
#include <Rendering/FrameBuffer.h>

namespace ej
{
	class RenderingManager;
}

namespace app
{
	class HeadSet final
	{
	public:
		explicit HeadSet(const ej::Core& core);

		void update(float dt);

		void bindEye(vr::EVREye eye);
		void submit();

		const ej::Transform& getTransform() const;
		ej::CameraEntity* getCameraEntity(vr::EVREye eye);

	private:
		ej::VRManager* m_vrManager = nullptr;
		ej::RenderingManager* m_renderingManager = nullptr;

		ej::Transform m_transform;

		ej::Camera m_cameras[2]{};
		ej::CameraEntity m_cameraEntities[2];

		ej::FrameBuffer m_eyeBuffers[2];
	};
}