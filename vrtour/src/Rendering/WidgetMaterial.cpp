#include "Rendering/WidgetMaterial.h"

#include <Managers/RenderingManager.h>
#include <Managers/TextureManager.h>

WidgetMaterial::WidgetMaterial(const ej::Core& core) :
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

	m_texture = core.get<ej::TextureManager>()->get("carpet");

	m_renderingManager->getState()->setCurrentShader(m_shader.get());
	m_shader->setUniform("uTexture", 3);
}

void WidgetMaterial::bind()
{
	auto state = m_renderingManager->getState();

	state->bindTexture(m_texture.get(), 0);

	state->setCurrentShader(m_shader.get());

	m_shader->setUniform("uColor", m_color);
	m_shader->setUniform("uHasTexture", static_cast<int>(m_texture != nullptr));
}

void WidgetMaterial::setColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void WidgetMaterial::setColor(const sf::Color& color)
{
	m_color = glm::vec4(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

void WidgetMaterial::setColor(const glm::vec4& color)
{
	m_color = color;
}

const glm::vec4& WidgetMaterial::getColor() const
{
	return m_color;
}
