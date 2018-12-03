#include "RenderingManager.h"

#include <GL/glew.h>

#include "WindowManager.h"

using namespace ej;

RenderingManager::RenderingManager(const Core& core) :
	BaseManager(core)
{
	if (glewInit() != GLEW_OK) {
		throw std::runtime_error("Unable to initialize GLEW");
	}
}
