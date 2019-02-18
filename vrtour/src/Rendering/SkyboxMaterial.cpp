#include "Rendering/SkyboxMaterial.h"

#include <Managers/RenderingManager.h>

SkyboxMaterial::SkyboxMaterial(const ej::Core& core) :
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

	m_renderingManager->getState()->setCurrentShader(m_shader.get());
	m_shader->setUniform("uSkyTexture", 2);
}

void SkyboxMaterial::bind()
{
	auto state = m_renderingManager->getState();

	state->setFaceCullingSide(GL_FRONT);
	state->bindTexture(m_skyTexture.get(), 2);

	state->setCurrentShader(m_shader.get());

	m_shader->setUniform("uHasTexture", static_cast<int>(m_skyTexture != nullptr));
}

void SkyboxMaterial::setSkyTexture(ej::Texture::ptr texture)
{
	m_skyTexture = texture;
}

ej::Texture* SkyboxMaterial::getTexture() const
{
	return m_skyTexture.get();
}
