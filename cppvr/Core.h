#pragma once

#include <SFML/Window/Event.hpp>

#include "ManagerLocator.h"

namespace ej 
{
	class Core
	{
	public:
		Core();

		void run();
		void stop();

		virtual void onHandleEvent(const sf::Event& event) = 0;
		virtual void onUpdate(float dt) = 0;

		ManagerLocator& getManagerLocator();

	protected:
		ManagerLocator m_managerLocator;

	private:
		bool m_isRunning;
	};
}

#define EJ_MAIN(Core) int main() { \
	try { \
		Core game; \
		game.run(); \
	} catch (const std::exception& e) { \
		std::cout << e.what() << std::endl; \
		std::cin.get(); \
	} \
	return 0; \
}