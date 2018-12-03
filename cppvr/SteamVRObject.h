#pragma once

#include <string>

#include <OpenVR/openvr.h>

#include "Core.h"
#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Transform.h"

class SteamVRObject
{
public:
	SteamVRObject(const ej::Core& core, const std::string& name);
	~SteamVRObject();

	void draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const ej::Transform& transform);
	void draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const glm::mat4& transform);

private:
	void tryLoad();

	bool m_isInitialized;
	std::string m_name;

	ej::Mesh m_mesh;
	std::shared_ptr<ej::Shader> m_shader;

	vr::RenderModel_t* m_renderModel;
	vr::IVRRenderModels* m_iVRRenderModels;
};
