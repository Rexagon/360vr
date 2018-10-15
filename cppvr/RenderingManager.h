#pragma once

#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>

#include "BaseManager.h"

namespace ej
{
	class WindowManager;

	class RenderingManager : public BaseManager
	{
	public:
		explicit RenderingManager(const ManagerLocator& locator);

	private:
		std::shared_ptr<WindowManager> m_windowManager;
	};
}