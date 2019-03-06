#pragma once

#include <SFML/Graphics/Font.hpp>

#include <UI/Widget.h>

#include "Rendering/TextMaterial.h"

namespace app
{
	class TextWidget : public ej::Widget
	{
	public:
		explicit TextWidget(const ej::Core& core);

		void update(float dt) override;

		void setText(const std::string& text);
		const std::string& getText() const;

		void setBold(bool bold);
		bool isBold() const;

		void setCharacterSize(unsigned int size);
		unsigned int getCharacterSize() const;

		void setLetterSpacing(float spacingFactor);
		float getLetterSpacing() const;

		void setFont(sf::Font* font);
		sf::Font* getFont() const;

		sf::FloatRect getBounds() const;

	private:
		void ensureGeometryUpdate() const;

		TextMaterial m_material;

		std::string m_text;
		bool m_isBold = false;
		unsigned int m_characterSize = 36;
		float m_letterSpacingFactor = 1.0f;

		sf::Font* m_font = nullptr;

		mutable sf::FloatRect m_bounds;
		mutable bool m_geometryNeedUpdate = false;
	};
}
