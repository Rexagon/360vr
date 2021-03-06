#include "Rendering/SimpleMeshMaterial.h"

#include <Rendering/ForwardRenderer.h>
#include <Managers/RenderingManager.h>

app::SimpleMeshMaterial::SimpleMeshMaterial(const ej::Core& core, ej::Texture* diffuseTexture) :
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

	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uDiffuseTexture", 0);

	m_renderingParameters = ej::ForwardRenderer::createRenderingParameters();
	m_renderingParameters.faceCullingSide = GL_BACK;
}

void app::SimpleMeshMaterial::bind()
{
	m_renderingManager->bindTexture(m_diffuseTexture, 0);

	m_renderingManager->setCurrentShader(m_shader);
	m_shader->setUniform("uColor", m_color);
	m_shader->setUniform("uTextureFlip", m_textureFlip);
	m_shader->setUniform("uHasTexture", static_cast<int>(m_diffuseTexture != nullptr));
}

void app::SimpleMeshMaterial::setDiffuseTexture(ej::Texture* texture)
{
	m_diffuseTexture = texture;
}

ej::Texture* app::SimpleMeshMaterial::getDiffuseTexture() const
{
	return m_diffuseTexture;
}

void app::SimpleMeshMaterial::setTextureFlipped(bool horizontally, bool vertically)
{
	m_textureFlip.x = 1 - static_cast<int>(horizontally);
	m_textureFlip.y = 1 - static_cast<int>(vertically);
}

const glm::ivec2& app::SimpleMeshMaterial::getTextureFlip() const
{
	return m_textureFlip;
}

void app::SimpleMeshMaterial::setColor(float r, float g, float b, float a)
{
	m_color = glm::vec4(r, g, b, a);
}

void app::SimpleMeshMaterial::setColor(const glm::vec4& color)
{
	m_color = color;
}

void app::SimpleMeshMaterial::setColor(const sf::Color& color)
{
	m_color = glm::vec4(
		static_cast<float>(color.r) / 255.0f,
		static_cast<float>(color.g) / 255.0f,
		static_cast<float>(color.b) / 255.0f,
		static_cast<float>(color.a) / 255.0f
	);
}

const glm::vec4& app::SimpleMeshMaterial::getColor() const
{
	return m_color;
}

bool app::SimpleMeshMaterial::hasTexture() const
{
	return m_diffuseTexture != nullptr;
}
