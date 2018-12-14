#pragma once

#include "Core.h"
#include "Mesh.h"
#include "Shader.h"

#include "Texture.h"
#include "Camera.h"
#include "Transform.h"

class Skybox
{
public:
	Skybox(const ej::Core& core);
	~Skybox();

	void draw(const ej::Camera& camera, const ej::Transform& transform) const;

	void setTexture(ej::Texture::ptr texture);
	const ej::Texture* getTexture() const;

	bool isInitialized() const;

private:
	bool m_isInitialized;

	ej::Mesh m_cube;
	ej::Texture::ptr m_texture;
	ej::Shader::ptr m_shader;
};
