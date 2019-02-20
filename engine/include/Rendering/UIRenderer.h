#pragma once

#include "Scene/CameraEntity.h"
#include "Rendering/Renderer.h"

namespace ej
{
	class WindowManager;

	class UIRenderer : public Renderer, public PointerDefs<UIRenderer>
	{
	public:
		explicit UIRenderer(const Core& core);

		void draw() override;

	private:
		std::shared_ptr<WindowManager> m_windowManager;

		CameraEntity::ptr m_cameraEntity;
	};
}
