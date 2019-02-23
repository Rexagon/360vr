#include "UI/RectangleWidget.h"

#include <Managers/MeshManager.h>

#include "Rendering/WidgetMaterial.h"

RectangleWidget::RectangleWidget(const ej::Core& core) :
	Widget(core)
{
	const auto meshName = "rectangle_mesh";

	auto mesh = core.get<ej::MeshManager>()->get(meshName);
	if (mesh == nullptr) {
		mesh = core.get<ej::MeshManager>()->bind(meshName, []() {
			return ej::MeshGeometry::createQuad(glm::vec2(0.5f), 
				ej::MeshGeometry::TEXTURED_VERTEX);
		})->get(meshName);
	}

	const auto material = std::make_shared<WidgetMaterial>(core);

	initMesh(mesh, material);
}

void RectangleWidget::update(float dt)
{
	//TODO: call draw method here
}
