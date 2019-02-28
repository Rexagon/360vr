#pragma once

#include "Scene/CameraEntity.h"
#include "Rendering/Renderer.h"

namespace ej
{
	class WindowManager;

	/**
	 * \brief Renderer for user interface
	 */
	class UIRenderer final : public Renderer, public PointerDefs<UIRenderer>
	{
	public:
		/**
		 * \param core Owner of this manager
		 */
		explicit UIRenderer(const Core& core);

		/**
		 * \brief Draw all entities to current frame buffer
		 *
		 * Does nothing if camera entity was not set.
		 * Clears rendering queue after calling this function.
		 */
		void draw() override;

	private:
		std::shared_ptr<WindowManager> m_windowManager;

		CameraEntity::ptr m_cameraEntity;
	};
}
