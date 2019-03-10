#include "Rendering/TextMaterial.h"

#include <Rendering/UIRenderer.h>
#include <Managers/TextureManager.h>
#include <Managers/RenderingManager.h>

app::TextMaterial::TextMaterial(const ej::Core& core) :
	Material(core)
{
	const std::string shaderName = "widget";

	m_shader = m_shaderManager->get(shaderName);
	if (m_shader == nullptr) {
		m_shader = m_shaderManager->bind(shaderName,
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".vert"),
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".frag"))->get(shaderName);
	}

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");

	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uTexture", 3);

	m_renderingParameters = ej::UIRenderer::createRenderingParameters();
}

void app::TextMaterial::bind()
{
	if (m_texture != nullptr) {
		m_renderingManager->bindTexture(GL_TEXTURE_2D, 
			m_texture->getNativeHandle(), 4);
	}

	m_renderingManager->setCurrentShader(m_shader);

	m_shader->setUniform("uColor", m_color);
	m_shader->setUniform("uHasTexture", static_cast<int>(m_texture != nullptr));
}

void app::TextMaterial::setColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void app::TextMaterial::setColor(const sf::Color& color)
{
	m_color = glm::vec4(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

void app::TextMaterial::setColor(const glm::vec4& color)
{
	m_color = color;
}

const glm::vec4& app::TextMaterial::getColor() const
{
	return m_color;
}

void app::TextMaterial::setTexture(const sf::Texture* texture)
{
	m_texture = texture;
}
