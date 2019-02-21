#pragma once

#include <SFML/Graphics/Font.hpp>

#include <Managers/RenderingManager.h>

#include "Widget.h"
#include "Rendering/TextMaterial.h"

class TextWidget : public Widget, public ej::PointerDefs<TextWidget>
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

	void setFont(std::shared_ptr<sf::Font> font);
	sf::Font* getFont() const;

private:
	void ensureGeometryUpdate();

	ej::RenderingManager::ptr m_renderingManager;
	TextMaterial::ptr m_material;

	std::string m_text;
	bool m_isBold = false;
	unsigned int m_characterSize = 36;
	float m_letterSpacingFactor = 1.0f;

	bool m_geometryNeedUpdate = false;

	std::shared_ptr<sf::Font> m_font;
};