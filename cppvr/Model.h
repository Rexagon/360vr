#pragma once

#include <memory>

#include "Mesh.h"
#include "Camera.h"
#include "Shader.h"
#include "Texture.h"
#include "Transform.h"

#include  "RenderingManager.h"

class Model : public ej::PointerDefs<Model>
{
public:
	explicit Model(const ej::Core& core);

	void draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const ej::Transform& transform) const;

	void setTexture(ej::Texture::ptr texture);
	ej::Texture* getTexture() const;

private:
	ej::RenderingManager::ptr m_renderingManager;

	ej::Mesh m_mesh;
	ej::Shader::ptr m_shader;
	ej::Texture::ptr m_texture;
};
