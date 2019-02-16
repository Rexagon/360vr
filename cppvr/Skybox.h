#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"

#include "RenderingManager.h"

class Skybox : public ej::PointerDefs<Skybox>
{
public:
	Skybox(const ej::Core& core);
	~Skybox();

	void draw(const ej::Camera& camera, const ej::Transform& transform) const;

	void setTexture(ej::Texture::ptr texture);
	ej::Texture* getTexture() const;

	ej::Shader* getShader() const;

	bool isInitialized() const;

private:
	ej::RenderingManager::ptr m_renderingManager;

	bool m_isInitialized;

	ej::Mesh m_cube;
	ej::Texture::ptr m_texture;
	ej::Shader::ptr m_shader;
};
