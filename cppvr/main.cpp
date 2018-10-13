#include "Core.h"

class MainScene : public core::Scene
{
public:
	void onInit() override
	{

	}

private:

};

int main()
{
	try 
	{
		core::Core game;

		game.provideManager<core::WindowManager>("CVR", 1024, 768);
		game.provideManager<core::SceneManager>(std::make_unique<MainScene>());
		game.provideManager<core::InputManager>();

		game.run();
	}
	catch (const std::exception& e) 
	{

	}

	return 0;
}