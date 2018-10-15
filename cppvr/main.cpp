#include <iostream>

#include "Core.h"
#include "MainScene.h"

#include "WindowManager.h"
#include "SceneManager.h"
#include "InputManager.h"

#include "RenderingManager.h"

using namespace ej;

int main()
{
	try 
	{
		Core game;

		game.getManagerLocator().provide<WindowManager>("CVR", 1024, 768);
		game.getManagerLocator().provide<SceneManager>(std::make_unique<MainScene>());
		game.getManagerLocator().provide<InputManager>();

		game.getManagerLocator().provide<RenderingManager>();

		game.run();
	}
	catch (const std::exception& e) {
		std::cout << e.what() << std::endl;
		std::cin.get();
	}

	return 0;
}