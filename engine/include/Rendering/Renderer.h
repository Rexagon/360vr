#pragma once

#include <vector>

#include "Scene/MeshEntity.h"

namespace ej
{
	class RenderingManager;

	class Renderer
	{
	public:
		Renderer(const Core& core);
		virtual ~Renderer() = default;

		void push(MeshEntity* entity);
		virtual void draw() = 0;

	protected:
		std::shared_ptr<RenderingManager> m_renderingManager;

		std::vector<MeshEntity*> m_entities;
	};
}