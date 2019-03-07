#pragma once

#include <vector>

#include "Scene/MeshEntity.h"

namespace ej
{
	class RenderingManager;

	/**
	 * \brief Renderer base class
	 */
	class Renderer
	{
	public:
		/**
		 * \brief Default constructor
		 * \param core Owner of this manager
		 */
		explicit Renderer(const Core& core);

		/**
		 * \brief Default destructor
		 */
		virtual ~Renderer() = default;

		/**
		 * \brief Add entity to render queue
		 * \param entity Entity pointer
		 */
		void push(MeshEntity* entity);

		/**
		 * \brief Draw all entities in queue
		 * 
		 * This is a pure virtual function that has to be implemented
		 * by the derived class to define how entities in render queue
		 * should be rendered.
		 */
		virtual void draw() = 0;

	protected:
		RenderingManager* m_renderingManager = nullptr;

		std::vector<MeshEntity*> m_entities;
	};
}