#include "UI/TextWidget.h"

#include <Managers/MeshManager.h>

TextWidget::TextWidget(const ej::Core& core) :
	Widget(core)
{
	const auto meshName = "text_mesh_" + std::to_string(reinterpret_cast<intptr_t>(this));

	auto mesh = core.get<ej::MeshManager>()->get(meshName);
	if (mesh == nullptr) {
		mesh = core.get<ej::MeshManager>()->bind(meshName, []() {
			return ej::MeshGeometry(ej::MeshGeometry::TEXTURED_VERTEX);
		})->get(meshName);
	}

	mesh->setBufferUsage(GL_DYNAMIC_DRAW);

	m_material = std::make_shared<TextMaterial>(core);

	initMesh(mesh, m_material);
}

void TextWidget::update(float dt)
{
	ensureGeometryUpdate();
}

void TextWidget::setText(const std::string& text)
{
	if (m_text != text) {
		m_text = text;
		m_geometryNeedUpdate = true;
	}
}

const std::string& TextWidget::getText() const
{
	return m_text;
}

void TextWidget::setBold(bool bold)
{
	if (m_isBold != bold) {
		m_isBold = bold;
		m_geometryNeedUpdate = true;
	}
}

bool TextWidget::isBold() const
{
	return m_isBold;
}

void TextWidget::setCharacterSize(unsigned int size)
{
	if (m_characterSize != size) {
		m_characterSize = size;
		m_geometryNeedUpdate = true;
	}
}

unsigned TextWidget::getCharacterSize() const
{
	return m_characterSize;
}

void TextWidget::setLetterSpacing(float spacingFactor)
{
	if (m_letterSpacingFactor != spacingFactor) {
		m_letterSpacingFactor = spacingFactor;
		m_geometryNeedUpdate = true;
	}
}

float TextWidget::getLetterSpacing() const
{
	return m_letterSpacingFactor;
}

void TextWidget::setFont(std::shared_ptr<sf::Font> font)
{
	if (font != nullptr && m_font != font) {
		m_font = font;
		m_geometryNeedUpdate = true;
	}
}

sf::Font* TextWidget::getFont() const
{
	return m_font.get();
}

void TextWidget::ensureGeometryUpdate()
{
	if (m_font == nullptr || !m_geometryNeedUpdate) {
		return;
	}

	m_geometryNeedUpdate = false;

	m_material->setTexture(&m_font->getTexture(m_characterSize));

	ej::MeshGeometry meshGeometry(ej::MeshGeometry::TEXTURED_VERTEX);
	meshGeometry.positions.reserve(m_text.size() * 4);
	meshGeometry.texCoords.reserve(m_text.size() * 4);
	meshGeometry.indices.reserve(m_text.size() * 6);

	if (m_text.empty()) {
		m_meshEntity->getMesh()->update(meshGeometry);
		return;
	}

	auto whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, m_isBold).advance;
	const auto letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
	whitespaceWidth += letterSpacing;
	const auto lineSpacing = m_font->getLineSpacing(m_characterSize);

	unsigned int index = 0;

	auto x = 0.0f;
	auto y = static_cast<float>(m_characterSize);

	char prevChar = 0;
	for (const auto& curChar : m_text) {
		if (curChar == '\r') {
			continue;
		}

		x += m_font->getKerning(prevChar, curChar, m_characterSize);

		switch (curChar) {
		case ' ':
			x += whitespaceWidth;
			continue;
		case '\n':
			y += lineSpacing;
			x = 0.0f;
			continue;
		case '\t':
			x += whitespaceWidth * 4.0f;
			continue;
		default:
			break;
		}

		prevChar = curChar;

		const auto& glyph = m_font->getGlyph(curChar, m_characterSize, m_isBold);

		// Append glyph vertices
		const auto padding = 1.0;

		const auto left = glyph.bounds.left - padding;
		const auto top = glyph.bounds.top - padding;
		const auto right = glyph.bounds.left + glyph.bounds.width + padding;
		const auto bottom = glyph.bounds.top + glyph.bounds.height + padding;

		const auto textureSize = sf::Vector2f(m_font->getTexture(m_characterSize).getSize());

		const auto u1 = (static_cast<float>(glyph.textureRect.left) - padding) / textureSize.x;
		const auto v1 = (static_cast<float>(glyph.textureRect.top) - padding) / textureSize.y;
		const auto u2 = (static_cast<float>(glyph.textureRect.left + glyph.textureRect.width) + padding) / textureSize.x;
		const auto v2 = (static_cast<float>(glyph.textureRect.top + glyph.textureRect.height) + padding) / textureSize.y;

		// top left
		meshGeometry.positions.emplace_back(x + left, y + top, 0.0f);
		meshGeometry.texCoords.emplace_back(u1, v1);

		// top right
		meshGeometry.positions.emplace_back(x + right, y + top, 0.0f);
		meshGeometry.texCoords.emplace_back(u2, v1);

		// bottom right
		meshGeometry.positions.emplace_back(x + right, y + bottom, 0.0f);
		meshGeometry.texCoords.emplace_back(u2, v2);

		// bottom left
		meshGeometry.positions.emplace_back(x + left, y + bottom, 0.0f);
		meshGeometry.texCoords.emplace_back(u1, v2);

		// filling indices
		meshGeometry.indices.emplace_back(index + 0);
		meshGeometry.indices.emplace_back(index + 1);
		meshGeometry.indices.emplace_back(index + 2);
		meshGeometry.indices.emplace_back(index + 2);
		meshGeometry.indices.emplace_back(index + 3);
		meshGeometry.indices.emplace_back(index + 0);

		index += 4;

		x += glyph.advance + letterSpacing;
	}

	m_meshEntity->getMesh()->update(meshGeometry);
}
