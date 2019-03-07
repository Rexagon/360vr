#pragma once

#include "Core/BaseManager.h"
#include "Rendering/RenderingState.h"

#include "Rendering/UIRenderer.h"
#include "Rendering/ForwardRenderer.h"

namespace ej
{
	class WindowManager;

	/**
	 * \brief Give access to renderer and rendering state
	 */
	class RenderingManager final : public BaseManager
	{
	public:
		/**
		 * \brief Constructor
		 * 
		 * Create and apply rendering state.
		 * \see ej::RenderingState
		 * 
		 * \param core Owner of this manager
		 */
		explicit RenderingManager(const Core& core);

		/**
		 * \brief Initialize renderers
		 * 
		 * Each call of this function will destroy previous
		 * renderers (if they existed) and create new.
		 */
		void init();

		/**
		 * \brief Get rendering state
		 * \return Rendering state. Cannot be nullptr
		 */
		RenderingState* getState() const;

		/**
		 * \brief Get forward renderer
		 * \return Forward renderer. Can be nullptr if manager was 
		 * not initialized
		 */
		ForwardRenderer* getForwardRenderer() const;

		/**
		 * \brief Get UI renderer
		 * \return UI renderer. Can be nullptr if manager was 
		 * not initialized
		 */
		UIRenderer* getUIRenderer() const;

	protected:
		WindowManager* m_windowManager = nullptr;

		std::unique_ptr<RenderingState> m_renderingState{};

		std::unique_ptr<ForwardRenderer> m_forwardRenderer{};
		std::unique_ptr<UIRenderer> m_uiRenderer{};
	};
}
