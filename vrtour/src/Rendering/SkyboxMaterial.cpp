#include "Rendering/SkyboxMaterial.h"

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

	m_renderingManager->getState()->setCurrentShader(m_shader);
	m_shader->setUniform("uSkyTexture", 2);
}

void app::SkyBoxMaterial::bind()
{
	auto state = m_renderingManager->getState();

	state->setFaceCullingSide(GL_FRONT);
	state->bindTexture(m_skyTexture, 2);

	state->setCurrentShader(m_shader);

	m_shader->setUniform("uHasTexture", static_cast<int>(m_skyTexture != nullptr));
}

void app::SkyBoxMaterial::setSkyTexture(ej::Texture* texture)
{
	m_skyTexture = texture;
}

ej::Texture* app::SkyBoxMaterial::getSkyTexture() const
{
	return m_skyTexture;
}
