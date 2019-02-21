#pragma once

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#include <Resources/Material.h>

class TextMaterial : public ej::Material, public ej::PointerDefs<TextMaterial>
{
public:
	TextMaterial(const ej::Core& core);

	void bind() override;

	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(const sf::Color& color);
	void setColor(const glm::vec4& color);
	const glm::vec4& getColor() const;

	void setTexture(const sf::Texture* texture);

private:
	const sf::Texture* m_texture = nullptr;
	glm::vec4 m_color{ 1.0f };
};
