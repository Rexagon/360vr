#pragma once

#include <SFML/Graphics/Color.hpp>

#include <Resources/Texture.h>
#include <Resources/Material.h>

namespace app
{
	class WidgetMaterial final : public ej::Material
	{
	public:
		explicit WidgetMaterial(const ej::Core& core);

		void bind() override;

		void setColor(float r, float g, float b, float a = 1.0f);
		void setColor(const sf::Color& color);
		void setColor(const glm::vec4& color);
		const glm::vec4& getColor() const;

	private:
		ej::Texture* m_texture = nullptr;
		glm::vec4 m_color{ 1.0f };
	};
}
