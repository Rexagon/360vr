#pragma once

#include "Core.h"

#include "Camera.h"
#include "Shader.h"
#include "FrameBuffer.h"
#include "VRManager.h"
#include "Transform.h"
#include "Mesh.h"

class HeadSet
{
public:
	HeadSet(const ej::Core& core);

	void update(float dt);
	   
	const ej::Camera& bindEye(vr::EVREye eye);
	void submit();
	void drawDebug();

	const ej::Transform& getTransform() const;

private:
	ej::VRManager::ptr m_vrManager;

	ej::Camera::uptr m_eyeCameras[2];
	ej::FrameBuffer m_eyeBuffers[2];

	ej::Transform m_transform;

	// For desktop rendering
	ej::Mesh m_screenQuad;
	ej::Shader::ptr m_screenQuadShader;
};
