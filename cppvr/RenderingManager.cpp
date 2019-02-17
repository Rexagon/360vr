#include "RenderingManager.h"

#include "Core.h"
#include "WindowManager.h"

ej::RenderingManager::RenderingManager(const Core& core) :
	BaseManager(core)
{
	m_windowManager = core.get<WindowManager>();

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize GLEW");
	}

	m_renderingState = std::make_shared<RenderingState>(core);
	m_renderingState->apply();
}

void ej::RenderingManager::init()
{
	m_forwardRenderer = std::make_shared<ForwardRenderer>(m_core);
}

ej::RenderingState* ej::RenderingManager::getState() const
{
	return m_renderingState.get();
}

ej::ForwardRenderer* ej::RenderingManager::getForwardRenderer() const
{
	return m_forwardRenderer.get();
}
