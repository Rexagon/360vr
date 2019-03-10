#pragma once

#include "Renderer.h"
#include "Scene/CameraEntity.h"

namespace ej
{
	/**
	 * \brief Simple renderer with one render target
	 */
	class ForwardRenderer final : public Renderer
	{
	public:
		static RenderingParameters createRenderingParameters();

		/**
		 * \param core Owner of this manager
		 */
		explicit ForwardRenderer(const Core& core);

		/**
		 * \brief Draw all entities to current frame buffer
		 * 
		 * Does nothing if camera entity was not set.
		 * Clears rendering queue after calling this function.
		 */
		void draw() override;

		/**
		 * \brief Set current camera entity
		 * \param entity Camera entity
		 */
		void setCameraEntity(CameraEntity* entity);

		/**
		 * \brief Return current camera entity
		 * \return Camera entity. Can be nullptr
		 */
		CameraEntity* getCameraEntity() const;

	private:
		CameraEntity* m_cameraEntity = nullptr;
	};
}