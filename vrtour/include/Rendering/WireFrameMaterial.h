#pragma once

#include <SFML/Graphics/Color.hpp>

#include <Resources/Material.h>

namespace app
{
	class WireFrameMaterial final : public ej::Material
	{
	public:
		explicit WireFrameMaterial(const ej::Core& core);

		void bind() override;

		void setColor(float r, float g, float b, float a = 1.0f);
		void setColor(const glm::vec4& color);
		void setColor(const sf::Color& color);
		const glm::vec4& getColor() const;

	private:
		glm::vec4 m_color{ 1.0f };
	};
}
