#pragma once

#include <SFML/Graphics/Color.hpp>

#include <Resources/Texture.h>
#include <Resources/Material.h>

class SimpleMeshMaterial : public ej::Material, public ej::PointerDefs<SimpleMeshMaterial>
{
public:
	explicit SimpleMeshMaterial(const ej::Core& core, ej::Texture::ptr diffuseTexture = nullptr);

	void bind() override;

	void setDiffuseTexture(ej::Texture::ptr texture);
	ej::Texture* getDiffuseTexture() const;

	void setTextureFlipped(bool horizontally, bool vertically = false);
	const glm::ivec2& getTextureFlip() const;

	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(const glm::vec4& color);
	void setColor(const sf::Color& color);
	const glm::vec4& getColor() const;

	bool hasTexture() const;

private:
	ej::Texture::ptr m_diffuseTexture;
	glm::ivec2 m_textureFlip;

	glm::vec4 m_color{1.0f};
};
