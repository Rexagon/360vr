#pragma once

#include "BaseManager.h"

#include "Shader.h"
#include "FrameBuffer.h"
#include "RenderingState.h"
#include "ForwardRenderer.h"

namespace ej
{
	class WindowManager;

	class RenderingManager : public BaseManager, public PointerDefs<RenderingManager>
	{
	public:
		RenderingManager(const Core& core);
		virtual ~RenderingManager() = default;

		void init();

		RenderingState* getState() const;

		ForwardRenderer* getForwardRenderer() const;

	protected:
		WindowManager::ptr m_windowManager;
		RenderingState::ptr m_renderingState;

		ForwardRenderer::ptr m_forwardRenderer;
	};
}
