#pragma once

#include <memory>

#include "ManagerLocator.h"

namespace ej 
{
	class Core
	{
	public:
		// Starts main game loop
		// Before calling this, you must provide at least this managers:
		//   - WindowManager
		//   - InputManager
		//   - InputManager
		void run();

		ManagerLocator& getManagerLocator();

	private:
		ManagerLocator m_managerLocator;
	};
}
