#include "UI/RectangleWidget.h"

RectangleWidget::RectangleWidget(const ej::Core& core) :
	Widget(core)
{
	m_renderingManager = core.get<ej::RenderingManager>();
}

void RectangleWidget::update(float dt)
{
	m_renderingManager->getForwardRenderer()->push(m_meshEntity.get());
}