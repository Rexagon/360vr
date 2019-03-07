#include "UI/TextWidget.h"

#include <Managers/MeshManager.h>

app::TextWidget::TextWidget(const ej::Core& core) :
	Widget(core), m_material(core)
{
	const auto meshName = "text_mesh_" + std::to_string(reinterpret_cast<intptr_t>(this));

	auto mesh = core.get<ej::MeshManager>()->get(meshName);
	if (mesh == nullptr) {
		mesh = core.get<ej::MeshManager>()->bind(meshName, []() {
			return ej::MeshGeometry(ej::MeshGeometry::TEXTURED_VERTEX);
		}).get(meshName);
	}

	mesh->setBufferUsage(GL_DYNAMIC_DRAW);

	initMesh(mesh, &m_material);
}

void app::TextWidget::update(float dt)
{
	ensureGeometryUpdate();
}

void app::TextWidget::setText(const std::string& text)
{
	if (m_text != text) {
		m_text = text;
		m_geometryNeedUpdate = true;
	}
}

const std::string& app::TextWidget::getText() const
{
	return m_text;
}

void app::TextWidget::setBold(const bool bold)
{
	if (m_isBold != bold) {
		m_isBold = bold;
		m_geometryNeedUpdate = true;
	}
}

bool app::TextWidget::isBold() const
{
	return m_isBold;
}

void app::TextWidget::setCharacterSize(const unsigned int size)
{
	if (m_characterSize != size) {
		m_characterSize = size;
		m_geometryNeedUpdate = true;
	}
}

unsigned app::TextWidget::getCharacterSize() const
{
	return m_characterSize;
}

void app::TextWidget::setLetterSpacing(const float spacingFactor)
{
	if (m_letterSpacingFactor != spacingFactor) {
		m_letterSpacingFactor = spacingFactor;
		m_geometryNeedUpdate = true;
	}
}

float app::TextWidget::getLetterSpacing() const
{
	return m_letterSpacingFactor;
}

void app::TextWidget::setFont(sf::Font* font)
{
	if (font != nullptr && m_font != font) {
		m_font = font;
		m_geometryNeedUpdate = true;
	}
}

sf::Font* app::TextWidget::getFont() const
{
	return m_font;
}

sf::FloatRect app::TextWidget::getBounds() const
{
	ensureGeometryUpdate();

	return m_bounds;
}

void app::TextWidget::ensureGeometryUpdate() const
{
	if (m_font == nullptr || !m_geometryNeedUpdate) {
		return;
	}

	m_geometryNeedUpdate = false;

	const_cast<TextMaterial*>(&m_material)->setTexture(&m_font->getTexture(m_characterSize));

	ej::MeshGeometry meshGeometry(ej::MeshGeometry::TEXTURED_VERTEX);
	meshGeometry.positions.reserve(m_text.size() * 4);
	meshGeometry.texCoords.reserve(m_text.size() * 4);
	meshGeometry.indices.reserve(m_text.size() * 6);

	if (m_text.empty()) {
		m_meshEntity.getMesh()->update(meshGeometry);
		return;
	}

	auto whitespaceWidth = m_font->getGlyph(L' ', m_characterSize, m_isBold).advance;
	const auto letterSpacing = (whitespaceWidth / 3.f) * (m_letterSpacingFactor - 1.f);
	whitespaceWidth += letterSpacing;
	const auto lineSpacing = m_font->getLineSpacing(m_characterSize);

	unsigned int index = 0;

	auto x = 0.0f;
	auto y = static_cast<float>(m_characterSize);

	glm::vec2 boundsMin(m_characterSize, m_characterSize);
	glm::vec2 boundsMax;

	char prevChar = 0;
	for (const auto& curChar : m_text) {
		if (curChar == '\r') {
			continue;
		}

		// Apply kerning offset
		x += m_font->getKerning(prevChar, curChar, m_characterSize);
		prevChar = curChar;

		// Handle special characters
		if (curChar == ' ' || curChar == '\n' || curChar == '\t') {
			boundsMin.x = std::min(boundsMin.x, x);
			boundsMin.y = std::min(boundsMin.y, y);

			switch (curChar) {
			case ' ':
				x += whitespaceWidth;
				break;
			case '\n':
				y += lineSpacing;
				x = 0.0f;
				break;
			case '\t':
				x += whitespaceWidth * 4.0f;
				break;
			default:
				break;
			}

			boundsMax.x = std::max(boundsMax.x, x);
			boundsMax.y = std::max(boundsMax.y, y);

			continue;
		}

		// Extract glyph description
		const auto& glyph = m_font->getGlyph(curChar, m_characterSize, m_isBold);

		// Append glyph vertices
		const auto padding = 1.0f;

		const auto left = glyph.bounds.left - padding;
		const auto top = glyph.bounds.top - padding;
		const auto right = glyph.bounds.left + glyph.bounds.width + padding;
		const auto bottom = glyph.bounds.top + glyph.bounds.height + padding;

		boundsMin.x = std::min(boundsMin.x, x + left);
		boundsMin.y = std::min(boundsMin.y, y + top);
		boundsMax.x = std::max(boundsMax.x, x + right);
		boundsMax.y = std::max(boundsMax.y, y + bottom);

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

	const auto size = boundsMax - boundsMin;

	const glm::vec3 offset(-boundsMin - size * 0.5f, 0.0f);
	for (auto& position : meshGeometry.positions) {
		position += offset;
	}

	m_bounds.left = boundsMin.x;
	m_bounds.top = boundsMin.y;
	m_bounds.width = size.x;
	m_bounds.height = size.y;
	m_meshEntity.getMesh()->update(meshGeometry);
}
