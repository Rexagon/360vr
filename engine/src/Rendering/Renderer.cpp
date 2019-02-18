#include "Rendering/Renderer.h"

#include "Managers/RenderingManager.h"

ej::Renderer::Renderer(const Core& core)
{
	m_renderingManager = core.get<RenderingManager>();
}

void ej::Renderer::push(MeshEntity* entity)
{
	m_entities.push_back(entity);
}
