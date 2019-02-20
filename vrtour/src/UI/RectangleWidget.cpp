#include "UI/RectangleWidget.h"

#include <Managers/MeshManager.h>

RectangleWidget::RectangleWidget(const ej::Core& core) :
	Widget(core)
{
	m_renderingManager = core.get<ej::RenderingManager>();

	const auto meshName = "rectangle_mesh";

	auto mesh = core.get<ej::MeshManager>()->get(meshName);
	if (mesh == nullptr) {
		mesh = core.get<ej::MeshManager>()->bind(meshName, []() {
			return ej::MeshGeometry::createQuad(glm::vec2(0.5f), 
				ej::MeshGeometry::TEXTURED_VERTEX);
		})->get(meshName);
	}

	auto material = std::make_shared<WidgetMaterial>(core);

	m_meshEntity = std::make_shared<ej::MeshEntity>(mesh, material);
}

void RectangleWidget::update(float dt)
{
	m_renderingManager->getForwardRenderer()->push(m_meshEntity.get());
}

void RectangleWidget::setSize(const glm::vec2& size)
{
	m_meshEntity->getTransform().setScale(size.x, size.y, 1.0f);
}

glm::vec2 RectangleWidget::getSize() const
{
	return glm::vec2(m_meshEntity->getTransform().getScale());
}
