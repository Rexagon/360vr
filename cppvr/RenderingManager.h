#pragma once

#include <SFML/Graphics.hpp>
#include <glm/vec2.hpp>

#include "BaseManager.h"

namespace ej
{
	class WindowManager;

	class RenderingManager : public ej::BaseManager
	{
	public:
		explicit RenderingManager(const ej::ManagerLocator& locator);

	private:
		std::shared_ptr<ej::WindowManager> m_windowManager;
	};
}