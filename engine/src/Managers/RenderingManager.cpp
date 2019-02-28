#include "Managers/RenderingManager.h"

#include "Core/Core.h"
#include "Managers/WindowManager.h"

ej::RenderingManager::RenderingManager(const Core& core) :
	BaseManager(core)
{
	m_windowManager = core.get<WindowManager>();

	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize GLEW");
	}

	m_renderingState = std::make_shared<RenderingState>();
	m_renderingState->apply();
}

void ej::RenderingManager::init()
{
	m_forwardRenderer = std::make_shared<ForwardRenderer>(m_core);
	m_uiRenderer = std::make_shared<UIRenderer>(m_core);
}

ej::RenderingState* ej::RenderingManager::getState() const
{
	return m_renderingState.get();
}

ej::ForwardRenderer* ej::RenderingManager::getForwardRenderer() const
{
	return m_forwardRenderer.get();
}

ej::UIRenderer* ej::RenderingManager::getUIRenderer() const
{
	return m_uiRenderer.get();
}
