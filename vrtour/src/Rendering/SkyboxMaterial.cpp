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

void app::SkyBoxMaterial::update(const float dt)
{
	if (!m_isPaused) {
		m_transition += dt;

		if (m_transition >= 1.0f) {
			finishTransition();
		}
	}
}

void app::SkyBoxMaterial::startTransition()
{
	m_isPaused = false;
}

void app::SkyBoxMaterial::finishTransition()
{
	m_transition = 0.0f;
	m_skyTexture = m_nextSkyTexture;
	m_nextSkyTexture = nullptr;
	m_isPaused = true;
}

void app::SkyBoxMaterial::pauseTransition()
{
	m_isPaused = true;
}

void app::SkyBoxMaterial::resetTransition()
{
	m_transition = 0.0f;
	m_isPaused = true;
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

float app::SkyBoxMaterial::getTransition() const
{
	return m_transition;
}

void app::SkyBoxMaterial::setTransitionSpeed(const float speed)
{
	if (m_transitionSpeed <= 0.0f) {
		return;
	}

	m_transitionSpeed = speed;
}

float app::SkyBoxMaterial::getTransitionSpeed() const
{
	return m_transitionSpeed;
}
