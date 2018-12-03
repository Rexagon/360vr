#pragma once

#include "Core.h"
#include "Mesh.h"
#include "Shader.h"

#include "VideoStream.h"
#include "Texture.h"
#include "Camera.h"
#include "Transform.h"

class Skybox
{
public:
	Skybox(const ej::Core& core);
	~Skybox();

	void init(const glm::ivec2& size);

	void draw(const ej::Camera& camera, const ej::Transform& transform) const;

	void updateTexture(VideoStream* videoStream);

	bool isInitialized() const;

private:
	bool m_isInitialized;

	glm::ivec2 m_size;
	int m_dataSize;

	ej::Mesh m_quad;

	std::shared_ptr<ej::Texture> m_texture;
	std::shared_ptr<ej::Shader> m_shader;

	GLuint m_PBOs[2];
	unsigned int m_currentPBO;
};
