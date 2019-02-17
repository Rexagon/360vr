#include "Renderer.h"

#include "RenderingManager.h"

ej::Renderer::Renderer(const Core& core)
{
	m_renderingManager = core.get<RenderingManager>();
}

void ej::Renderer::push(MeshEntity* entity)
{
	m_entities.push_back(entity);
}
