#pragma once

#include "Core/BaseManager.h"
#include "Resources/Shader.h"
#include "Rendering/FrameBuffer.h"
#include "Rendering/RenderingState.h"
#include "Rendering/ForwardRenderer.h"

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
