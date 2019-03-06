#include "UI/RectangleWidget.h"

#include <Managers/MeshManager.h>

#include "Rendering/WidgetMaterial.h"

app::RectangleWidget::RectangleWidget(const ej::Core& core) :
	Widget(core), m_material(core)
{
	const auto meshName = "rectangle_mesh";

	auto mesh = core.get<ej::MeshManager>()->get(meshName);
	if (mesh == nullptr) {
		mesh = core.get<ej::MeshManager>()->bind(meshName, []() {
			return ej::MeshGeometry::createQuad(glm::vec2(0.5f), 
				ej::MeshGeometry::TEXTURED_VERTEX);
		}).get(meshName);
	}

	initMesh(mesh, &m_material);
}

void app::RectangleWidget::update(float dt)
{
	//TODO: call draw method here
}
