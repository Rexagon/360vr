#pragma once

#include <UI/Widget.h>
#include <Managers/RenderingManager.h>

class RectangleWidget : public ej::Widget, public ej::PointerDefs<RectangleWidget>
{
public:
	explicit RectangleWidget(const ej::Core& core);

	void update(float dt) override;
};