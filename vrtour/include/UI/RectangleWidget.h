#pragma once

#include <Managers/RenderingManager.h>

#include "Widget.h"

class RectangleWidget : public Widget
{
public:
	RectangleWidget(const ej::Core& core);

	void update(float dt) override;

private:
	ej::RenderingManager::ptr m_renderingManager;
};