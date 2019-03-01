#pragma once

#include <Resources/Mesh.h>
#include <Scene/Transform.h>
#include <Rendering/FrameBuffer.h>

#include <Managers/VRManager.h>
#include <Managers/RenderingManager.h>

class HeadSet final : public ej::PointerDefs<HeadSet>
{
public:
	explicit HeadSet(const ej::Core& core);

	void update(float dt);
	   
	void bindEye(vr::EVREye eye);
	void submit();

	const ej::Transform& getTransform() const;
	ej::CameraEntity::ptr getCameraEntity(vr::EVREye eye) const;

private:
	ej::VRManager::ptr m_vrManager;
	ej::RenderingManager::ptr m_renderingManager;

	ej::Transform m_transform;

	ej::CameraEntity::ptr m_cameraEntities[2];

	ej::FrameBuffer m_eyeBuffers[2];
};
