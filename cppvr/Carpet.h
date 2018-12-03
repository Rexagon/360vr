#pragma once
#include "Camera.h"
#include "Mesh.h"
#include <memory>
#include "Shader.h"

#include "Core.h"
#include "Texture.h"
#include "Transform.h"

class Carpet
{
public:
	explicit Carpet(const ej::Core& core);

	void draw(const ej::Camera& camera, const ej::Transform& cameraTransform) const;

private:
	ej::Mesh m_mesh;

	std::shared_ptr<ej::Shader> m_shader;
	std::shared_ptr<ej::Texture> m_texture;
};
