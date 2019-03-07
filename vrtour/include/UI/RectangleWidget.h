#pragma once

#include <UI/Widget.h>
#include <Rendering/WidgetMaterial.h>
#include <Managers/RenderingManager.h>

namespace app
{
	class RectangleWidget : public ej::Widget
	{
	public:
		explicit RectangleWidget(const ej::Core& core);

		void update(float dt) override;

	private:
		WidgetMaterial m_material;
	};
}
