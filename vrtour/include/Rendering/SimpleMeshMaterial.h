#pragma once

#include <SFML/Graphics/Color.hpp>

#include <Resources/Texture.h>
#include <Resources/Material.h>

namespace app
{
	class SimpleMeshMaterial final : public ej::Material
	{
	public:
		explicit SimpleMeshMaterial(const ej::Core& core, ej::Texture* diffuseTexture = nullptr);

		void bind() override;

		void setDiffuseTexture(ej::Texture* texture);
		ej::Texture* getDiffuseTexture() const;

		void setTextureFlipped(bool horizontally, bool vertically = false);
		const glm::ivec2& getTextureFlip() const;

		void setColor(float r, float g, float b, float a = 1.0f);
		void setColor(const glm::vec4& color);
		void setColor(const sf::Color& color);
		const glm::vec4& getColor() const;

		bool hasTexture() const;

	private:
		ej::Texture* m_diffuseTexture = nullptr;
		glm::ivec2 m_textureFlip{ 0, 0 };

		glm::vec4 m_color{ 1.0f };
	};
}
