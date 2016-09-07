#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>

#include "Engine.h"

int main()
{
	Engine gameEngine;


	if (!gameEngine.init()) {
		std::cout << "Init failed";
		return -1;
	}

	if (!gameEngine.bufferModels()) {
		std::cout << "Buffer failed";
		return -1;
	}

	if (!gameEngine.useShaders()) {
		std::cout << "Shaders failed";
		return -1;
	}

	gameEngine.gameLoop();

	return 0;
}