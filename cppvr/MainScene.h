#pragma once

#include <GL/glew.h>

#include "SceneManager.h"
#include "ShaderManager.h"

#include "Mesh.h"

class MainScene : public ej::Scene
{
public:
	MainScene();
	~MainScene();

	void onInit() override;
	void onClose() override;

	void onUpdate(float dt) override;

private:
	void updatePixels(GLubyte* ptr);

	ej::Mesh m_quad;

	std::shared_ptr<ej::Shader> m_shader;
	GLuint m_texture;
	GLuint m_PBOs[2];
	unsigned int m_currentPBO;
};