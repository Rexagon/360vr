#include "Rendering/WireFrameMaterial.h"

#include <Rendering/ForwardRenderer.h>
#include <Managers/RenderingManager.h>

app::WireFrameMaterial::WireFrameMaterial(const ej::Core& core) :
	Material(core)
{
	const std::string shaderName = "wireframe";

	m_shader = m_shaderManager->get(shaderName);
	if (m_shader == nullptr) {
		m_shader = m_shaderManager->bind(shaderName,
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".vert"),
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".frag"))->get(shaderName);
	}

	m_shader->setAttribute(0, "vPosition");

	m_renderingParameters = ej::ForwardRenderer::createRenderingParameters();
	m_renderingParameters.isFaceCullingEnabled = false;
	m_renderingParameters.polygonMode = GL_LINE;
	m_renderingParameters.isBlendingEnabled = true;
}

void app::WireFrameMaterial::bind()
{
	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uColor", m_color);
}

void app::WireFrameMaterial::setColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void app::WireFrameMaterial::setColor(const glm::vec4 & color)
{
	m_color = color;
}

void app::WireFrameMaterial::setColor(const sf::Color & color)
{
	m_color = glm::vec4(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

const glm::vec4 & app::WireFrameMaterial::getColor() const
{
	return m_color;
}
