#include "MainScene.h"

#include <iostream>
#include <vector>

#include "ManagerLocator.h"

static const int DATA_WIDTH = 100;
static const int DATA_HEIGHT = 100;
static const int DATA_SIZE = 30000;

MainScene::MainScene() :
	m_currentPBO(0)
{
	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, DATA_WIDTH, DATA_HEIGHT, 0,
		GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glGenBuffers(2, m_PBOs);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);
}

MainScene::~MainScene()
{
	glDeleteTextures(1, &m_texture);
	glDeleteBuffers(2, m_PBOs);
}

void MainScene::onInit()
{
	getLocator().get<ej::ShaderManager>()->bind("quad", 
		ej::ShaderManager::FromFile("quad.vert"), ej::ShaderManager::FromFile("quad.frag"));

	m_shader = getLocator().get<ej::ShaderManager>()->get("quad");
	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");

	m_shader->setUniform("diffuseTexture", 0);

	m_quad.init(ej::MeshGeometry::createQuad(glm::vec2(0.5f, 0.5f),
		ej::MeshGeometry::TEXTURED_VERTEX));
}

void MainScene::onClose()
{
}

void MainScene::onUpdate(const float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_currentPBO = (m_currentPBO + 1) % 2;
	unsigned int nextPBO = (m_currentPBO + 1) % 2;

	glBindTexture(GL_TEXTURE_2D, m_texture);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[m_currentPBO]);

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, DATA_WIDTH, DATA_HEIGHT, GL_BGRA, GL_UNSIGNED_BYTE, 0);

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[nextPBO]);

	glBufferData(GL_PIXEL_UNPACK_BUFFER, DATA_SIZE, 0, GL_STREAM_DRAW);

	GLubyte* ptr = (GLubyte*)glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY);
	if (ptr)
	{
		updatePixels(ptr);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	// Draw texture quad
	

	glUseProgram(m_shader->getHandle());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	
	m_quad.draw();
}

void MainScene::updatePixels(GLubyte * dst)
{
	std::memset(dst, 100, DATA_SIZE);
}
