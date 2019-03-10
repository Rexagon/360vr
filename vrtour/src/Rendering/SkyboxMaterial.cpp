#include "Rendering/SkyboxMaterial.h"

#include <Rendering/ForwardRenderer.h>
#include <Managers/RenderingManager.h>

app::SkyBoxMaterial::SkyBoxMaterial(const ej::Core& core) :
	Material(core)
{
	const std::string shaderName = "skybox";

	m_shader = m_shaderManager->get(shaderName);
	if (m_shader == nullptr) {
		m_shader = m_shaderManager->bind(shaderName,
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".vert"),
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".frag"))->get(shaderName);
	}

	m_shader->setAttribute(0, "vPosition");

	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uSkyTexture", 2);
	m_shader->setUniform("uNextSkyTexture", 3);

	m_renderingParameters = ej::ForwardRenderer::createRenderingParameters();
	m_renderingParameters.faceCullingSide = GL_FRONT;
}

void app::SkyBoxMaterial::bind()
{
	m_renderingManager->bindTexture(m_skyTexture, 2);
	m_renderingManager->bindTexture(m_nextSkyTexture, 3);

	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uTransition", m_transition);
	m_shader->setUniform("uHasTexture", static_cast<int>(m_skyTexture != nullptr));
	m_shader->setUniform("uHasNextTexture", static_cast<int>(m_nextSkyTexture != nullptr));
}

void app::SkyBoxMaterial::setSkyTexture(ej::Texture* texture)
{
	m_skyTexture = texture;
}

ej::Texture* app::SkyBoxMaterial::getSkyTexture() const
{
	return m_skyTexture;
}

void app::SkyBoxMaterial::setNextSkyTexture(ej::Texture* texture)
{
	m_nextSkyTexture = texture;
}

ej::Texture* app::SkyBoxMaterial::getNextSkyTexture() const
{
	return m_nextSkyTexture;
}

void app::SkyBoxMaterial::setTransition(const float transition)
{
	m_transition = transition;
}

float app::SkyBoxMaterial::getTransition() const
{
	return m_transition;
}
