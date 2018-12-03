#pragma once

#include <SFML/Graphics.hpp>

#include "BaseManager.h"

namespace ej
{
	class WindowManager;

	class RenderingManager : public BaseManager, public PointerDefs<RenderingManager>
	{
	public:
		explicit RenderingManager(const Core& core);

	private:
		std::shared_ptr<WindowManager> m_windowManager;
	};
}