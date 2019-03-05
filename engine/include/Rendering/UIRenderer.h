#pragma once

#include "Scene/CameraEntity.h"
#include "Rendering/Renderer.h"

namespace ej
{
	class WindowManager;

	/**
	 * \brief Renderer for user interface
	 */
	class UIRenderer final : public Renderer
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
		WindowManager* m_windowManager{ nullptr };

		Camera m_camera{};
		CameraEntity m_cameraEntity;
	};
}
