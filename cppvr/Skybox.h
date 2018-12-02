#pragma once

#include "ManagerLocator.h"
#include "Mesh.h"
#include "Shader.h"

#include "VideoStream.h"

class Skybox
{
public:
	Skybox(const ej::ManagerLocator& locator);
	~Skybox();

	void init(const glm::ivec2& size);

	void draw() const;

	void updateTexture(VideoStream* videoStream);

	bool isInitialized() const;

private:
	bool m_isInitialized;

	glm::ivec2 m_size;
	int m_dataSize;

	ej::Mesh m_quad;

	GLuint m_texture;
	std::shared_ptr<ej::Shader> m_shader;

	GLuint m_PBOs[2];
	unsigned int m_currentPBO;
};