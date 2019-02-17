#pragma once

#include "Renderer.h"
#include "CameraEntity.h"

namespace ej
{
	class ForwardRenderer : public Renderer, public PointerDefs<ForwardRenderer>
	{
	public:
		explicit ForwardRenderer(const Core& core);

		void draw() override;

		void setCameraEntity(CameraEntity::ptr entity);
		CameraEntity* getCameraEntity() const;

	private:
		CameraEntity::ptr m_cameraEntity;
	};
}