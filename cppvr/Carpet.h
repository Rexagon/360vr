#pragma once
#include "Camera.h"
#include "Mesh.h"
#include <memory>
#include "Shader.h"

#include "ManagerLocator.h"
#include "Texture.h"

class Carpet
{
public:
	explicit Carpet(const ej::ManagerLocator& locator);

	void draw(ej::Camera& camera) const;

private:
	ej::Mesh m_mesh;

	std::shared_ptr<ej::Shader> m_shader;
	std::shared_ptr<ej::Texture> m_texture;
};
