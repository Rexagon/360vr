#include "Rendering/SimpleMeshMaterial.h"

#include <Managers/RenderingManager.h>

SimpleMeshMaterial::SimpleMeshMaterial(const ej::Core& core, ej::Texture::ptr diffuseTexture) :
	Material(core), m_diffuseTexture(diffuseTexture)
{
	const std::string shaderName = "simple";

	m_shader = m_shaderManager->get(shaderName);
	if (m_shader == nullptr) {
		m_shader = m_shaderManager->bind(shaderName,
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".vert"),
			ej::ShaderManager::FromFile("shaders/" + shaderName + ".frag"))->get(shaderName);
	}

	m_shader->setAttribute(0, "vPosition");
	m_shader->setAttribute(1, "vTexCoords");
	m_shader->setAttribute(2, "vNormal");

	m_renderingManager->getState()->setCurrentShader(m_shader.get());
	m_shader->setUniform("uDiffuseTexture", 0);
}

void SimpleMeshMaterial::bind()
{
	auto state = m_renderingManager->getState();

	state->setFaceCullingSide(GL_BACK);
	state->bindTexture(m_diffuseTexture.get(), 0);

	state->setCurrentShader(m_shader.get());

	m_shader->setUniform("uColor", m_color);
	m_shader->setUniform("uTextureFlip", m_textureFlip);
	m_shader->setUniform("uHasTexture", static_cast<int>(m_diffuseTexture != nullptr));
}

void SimpleMeshMaterial::setDiffuseTexture(ej::Texture::ptr texture)
{
	m_diffuseTexture = texture;
}

ej::Texture* SimpleMeshMaterial::getDiffuseTexture() const
{
	return m_diffuseTexture.get();
}

void SimpleMeshMaterial::setTextureFlipped(bool horizontally, bool vertically)
{
	m_textureFlip.x = 1 - static_cast<int>(horizontally);
	m_textureFlip.y = 1 - static_cast<int>(vertically);
}

const glm::ivec2& SimpleMeshMaterial::getTextureFlip() const
{
	return m_textureFlip;
}

void SimpleMeshMaterial::setColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void SimpleMeshMaterial::setColor(const glm::vec4& color)
{
	m_color = color;
}

void SimpleMeshMaterial::setColor(const sf::Color& color)
{
	m_color = glm::vec4(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

const glm::vec4& SimpleMeshMaterial::getColor() const
{
	return m_color;
}

bool SimpleMeshMaterial::hasTexture() const
{
	return m_diffuseTexture != nullptr;
}
