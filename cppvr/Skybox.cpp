#include "Skybox.h"

#include <iostream>

#include "ShaderManager.h"
#include "TextureManager.h"
#include "Transform.h"

Skybox::Skybox(const ej::Core & core) :
	m_isInitialized(false), m_texture(nullptr), m_shader(nullptr)
{
	m_renderingManager = core.get<ej::RenderingManager>();

	core.get<ej::TextureManager>()->bind("loading",
		ej::TextureManager::FromFile("textures/loading.jpg"));

	m_texture = core.get<ej::TextureManager>()->get("loading");

	m_shader = core.get<ej::ShaderManager>()->bind("skybox",
		ej::ShaderManager::FromFile("shaders/skybox.vert"),
		ej::ShaderManager::FromFile("shaders/skybox.frag"))->get("skybox");

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");

	m_renderingManager->setCurrentShader(m_shader.get());
	m_shader->setUniform("uProjection", glm::mat4());
	m_shader->setUniform("uRotation", glm::mat4());
	m_shader->setUniform("uDiffuseTexture", 0);
	m_renderingManager->setCurrentShader(nullptr);

	m_cube.init(ej::MeshGeometry::createCube(glm::vec3(1.0f, 1.0f, 1.0f),
		ej::MeshGeometry::TEXTURED_VERTEX));
}

Skybox::~Skybox()
{
}

void Skybox::draw(const ej::Camera& camera, const ej::Transform& transform) const
{
	m_renderingManager->setFaceCullingSide(GL_FRONT);

	if (m_texture) {
		m_texture->bind(0);
	}

	m_renderingManager->setCurrentShader(m_shader.get());

	m_shader->setUniform("uProjection", camera.getProjectionMatrix());
	m_shader->setUniform("uRotation", transform.getRotationMatrixInverse());

	m_cube.draw();
}

void Skybox::setTexture(ej::Texture::ptr texture)
{
	m_texture = texture;
}

const ej::Texture* Skybox::getTexture() const
{
	return m_texture.get();
}

bool Skybox::isInitialized() const
{
	return m_isInitialized;
}
