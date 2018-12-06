#include "Carpet.h"
#include "TextureManager.h"
#include "ShaderManager.h"

Carpet::Carpet(const ej::Core& core)
{
	core.get<ej::TextureManager>()->bind("carpet", 
		ej::TextureManager::FromFile("textures/carpet.jpg"));
	m_texture = core.get<ej::TextureManager>()->get("carpet");

	m_shader = core.get<ej::ShaderManager>()->bind("carpet",
		ej::ShaderManager::FromFile("shaders/carpet.vert"), 
		ej::ShaderManager::FromFile("shaders/carpet.frag"))->get("carpet");

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");
	m_shader->setAttribute(2, "vNormal");

	glUseProgram(m_shader->getHandle());
	m_shader->setUniform("uDiffuseTexture", 0);
	glUseProgram(0);

	m_mesh.init(ej::MeshGeometry::createPlane(glm::vec2(1.4f, 1.0f), 1, 1));
}

void Carpet::draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const ej::Transform& transform) const
{
	m_texture->bind(0);

	glCullFace(GL_BACK);

	glUseProgram(m_shader->getHandle());
	m_shader->setUniform("uViewProjection", camera.getViewProjectionMatrix());
	m_shader->setUniform("uTransform", transform.getGlobalTransformationMatrix());
	m_shader->setUniform("uCameraPosition", cameraTransform.getGlobalPosition());
	m_mesh.draw();
	
	glUseProgram(0);

	glBindTexture(GL_TEXTURE_2D, 0);
}
