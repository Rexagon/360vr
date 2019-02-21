#pragma once

#include <Managers/RenderingManager.h>

#include "Widget.h"
#include "Rendering/WidgetMaterial.h"

class RectangleWidget : public Widget, public ej::PointerDefs<RectangleWidget>
{
public:
	explicit RectangleWidget(const ej::Core& core);

	void update(float dt) override;

	void setSize(const glm::vec2& size);
	glm::vec2 getSize() const;

private:
	ej::RenderingManager::ptr m_renderingManager;
	WidgetMaterial::ptr m_widgetMaterial;
};