#pragma once

#include <SFML/Graphics/Color.hpp>

#include <Resources/Texture.h>
#include <Resources/Material.h>

class WidgetMaterial : public ej::Material, public ej::PointerDefs<WidgetMaterial>
{
public:
	WidgetMaterial(const ej::Core& core);

	void bind() override;

	void setColor(float r, float g, float b, float a = 1.0f);
	void setColor(const sf::Color& color);
	void setColor(const glm::vec4& color);
	const glm::vec4& getColor() const;

private:
	ej::Texture::ptr m_texture;
	glm::vec4 m_color{1.0f};
};
