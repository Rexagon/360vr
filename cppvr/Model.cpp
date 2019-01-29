#include "Model.h"
#include "TextureManager.h"
#include "ShaderManager.h"

Model::Model(const ej::Core& core)
{
	m_renderingManager = core.get<ej::RenderingManager>();

	core.get<ej::TextureManager>()->bind("carpet", 
		ej::TextureManager::FromFile("textures/carpet.jpg"));
	m_texture = core.get<ej::TextureManager>()->get("carpet");

	m_shader = core.get<ej::ShaderManager>()->bind("model",
		ej::ShaderManager::FromFile("shaders/model.vert"), 
		ej::ShaderManager::FromFile("shaders/model.frag"))->get("model");

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");
	m_shader->setAttribute(2, "vNormal");

	m_renderingManager->setCurrentShader(m_shader.get());
	m_shader->setUniform("uDiffuseTexture", 0);
	m_renderingManager->setCurrentShader(nullptr);

	m_mesh.init(ej::MeshGeometry::createPlane(glm::vec2(1.4f, 1.0f), 1, 1));
}

void Model::draw(const ej::Camera& camera, const ej::Transform& cameraTransform, const ej::Transform& transform) const
{
	m_renderingManager->setFaceCullingSide(GL_BACK);

	m_texture->bind(0);

	m_renderingManager->setCurrentShader(m_shader.get());

	m_shader->setUniform("uViewProjection", camera.getViewProjectionMatrix());
	m_shader->setUniform("uTransform", transform.getGlobalTransformationMatrix());
	m_shader->setUniform("uCameraPosition", cameraTransform.getGlobalPosition());

	m_mesh.draw();
}

void Model::setTexture(ej::Texture::ptr texture)
{
	m_texture = texture;
}

ej::Texture* Model::getTexture() const
{
	return m_texture.get();
}
