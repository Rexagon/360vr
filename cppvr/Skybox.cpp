#include "Skybox.h"

#include <iostream>

#include "ShaderManager.h"

Skybox::Skybox(const ej::ManagerLocator & locator) :
	m_isInitialized(false), m_size(), m_dataSize(0), 
	m_texture(0), m_currentPBO(0)
{
	locator.get<ej::ShaderManager>()->bind("quad",
		ej::ShaderManager::FromFile("quad.vert"), 
		ej::ShaderManager::FromFile("quad.frag"));

	m_shader = locator.get<ej::ShaderManager>()->get("quad");
	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");

	m_shader->setUniform("diffuseTexture", 0);

	m_quad.init(ej::MeshGeometry::createQuad(glm::vec2(0.5f, 0.5f),
		ej::MeshGeometry::TEXTURED_VERTEX));
}

Skybox::~Skybox()
{
	if (m_isInitialized) {
		glDeleteTextures(1, &m_texture);
		glDeleteBuffers(2, m_PBOs);
	}
}

void Skybox::init(const glm::ivec2 & size)
{
	if (m_isInitialized) {
		return;
	}

	m_size = size;
	m_dataSize = size.x * size.y * 3;

	printf("X: %d, Y: %d\n", size.x, size.y);

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, size.x, size.y, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glGenBuffers(2, m_PBOs);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[0]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, m_dataSize, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[1]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, m_dataSize, nullptr, GL_STREAM_DRAW);
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	m_isInitialized = true;
}

void Skybox::draw() const
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glUseProgram(m_shader->getHandle());
	m_quad.draw();
	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

void Skybox::updateTexture(VideoStream* videoStream)
{
	if (videoStream == nullptr) {
		return;
	}

	m_currentPBO = (m_currentPBO + 1) % 2;
	const auto nextPBO = (m_currentPBO + 1) % 2;

	glBindTexture(GL_TEXTURE_2D, m_texture);

	// Nth PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[m_currentPBO]);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_size.x, m_size.y, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

	// N+1 PBO
	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_PBOs[nextPBO]);
	glBufferData(GL_PIXEL_UNPACK_BUFFER, m_dataSize, nullptr, GL_STREAM_DRAW);

	const auto ptr = static_cast<GLubyte*>(glMapBuffer(GL_PIXEL_UNPACK_BUFFER, GL_WRITE_ONLY));
	if (ptr) {
		videoStream->sendCurrentData(ptr, m_dataSize);
		//std::memset(ptr, rand() % 255, m_dataSize);
		glUnmapBuffer(GL_PIXEL_UNPACK_BUFFER);
	}

	glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
}

bool Skybox::isInitialized() const
{
	return m_isInitialized;
}
