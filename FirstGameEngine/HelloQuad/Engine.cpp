#include "Engine.h"

namespace
{
	map<int, bool> keyIsDown;
	map<int, bool> keyWasDown;

	void mouseClick(GLFWwindow * windowPtr, int button, int action, int mods)
	{
		keyIsDown[button] = action;
	}

	void keyPress(GLFWwindow * windowPtr, int key, int scancode, int action, int mods)
	{
		keyIsDown[key] = (action != 0);
	}
}

Engine::Engine()
{
}

Engine::~Engine()
{
	//Iterate through map and delete textures
	for (map<const string, GLuint>::iterator it = textures.begin(); it != textures.end(); it++)
	{
		glDeleteTextures(1, &(it->second));
	}

	//Clear texture map
	textures.clear();
}

bool Engine::init()
{

	//Initialize window library
	if (glfwInit() == GL_FALSE)
		return false;

	GLFWwindowPtr =
		glfwCreateWindow(800, 800, "Ryan Geyer DSA1 Engine", NULL, NULL);


	if (GLFWwindowPtr != nullptr)
		glfwMakeContextCurrent(GLFWwindowPtr);
	else
	{
		glfwTerminate();
		return false;
	}

	if (glewInit() != GLEW_OK)
	{
		glfwTerminate();
		return false;
	}

	glfwSetMouseButtonCallback(GLFWwindowPtr, mouseClick);
	glfwSetKeyCallback(GLFWwindowPtr, keyPress);

	//SET UP OBJECTS
	player = Car(vec3(0,0,0), vec3(0,0,0), vec3(0.1,0.1,0.1), "textures/CarModelTexture.jpg", "models/CarModel.obj", colliderType::sphere, 1.f, 4.f, 30.f );

	gameObjects = {
		Object(vec3(2,0,5), vec3(0,0,0), vec3(1,1,1), "textures/rockTexture.png", "models/sphere.obj", colliderType::sphere)
	};

	tracks = {
		TrackPiece(vec3(0,0,0), vec3(90,0,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(0,0,6), vec3(90,0,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-6,0,6), vec3(90,90,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(-12,0,6), vec3(90,180,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-12,0,12), vec3(90,0,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-18,0,12), vec3(90,270,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-18,0,6), vec3(90,0,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(-18,0,0), vec3(90,0,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(-18,0,-6), vec3(90,0,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(-18,0,-12), vec3(90,180,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-12,0,-12), vec3(90,90,0), "textures/straightTile.jpg", TrackPiece::TrackType::straight),
		TrackPiece(vec3(-6,0,-12), vec3(90,90,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(-6,0,-6), vec3(90,270,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
		TrackPiece(vec3(0,0,-6), vec3(90,90,0), "textures/cornerTile.jpg", TrackPiece::TrackType::corner),
	};

	//Load all textures for objects
	textures[player.textureFile] = loadImage(player.textureFile);

	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		//If a key doesn't exist for a loaded texture, load it and add to map
		if (textures.find(gameObjects[i].textureFile) == textures.end())
		{
			textures[gameObjects[i].textureFile] = loadImage(gameObjects[i].textureFile);
		}
	}

	for (unsigned int i = 0; i < tracks.size(); i++)
	{
		if (textures.find(tracks[i].textureFile) == textures.end())
		{
			textures[tracks[i].textureFile] = loadImage(tracks[i].textureFile);
		}
	}

	//Set up camera
	mainCamera = { vec3(0,4,0), vec3(0,0,0), mat3(), 1.f, 800, 600, 0.f,0.f,.01f,1000.f, mat4() };
	mainCamera.fov = PI * .4f / mainCamera.zoom;
	mainCamera.aspectRatio = (float)mainCamera.width / (float)mainCamera.height;

	lightPos = vec3(-9,50,0);


	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glClearColor(0.2f, 0.8f, 0.6f, 1.0f);

	return true;
}


bool Engine::bufferModels()
{
	if(!player.bufferModel()) return false;

	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if(!gameObjects[i].bufferModel()) return false;
	}

	for (unsigned int i = 0; i < tracks.size(); i++)
	{
		if(!tracks[i].bufferModel()) return false;
	}

	return true;
}

bool Engine::useShaders()
{
	GLuint programIndex = shaderManager.loadShaders("shaders/vShader.glsl", "shaders/fShader.glsl");

	if (programIndex == 0) return false;

	glUseProgram(programIndex);

	return true;
}

GLuint Engine::loadImage(const string imageFile)
{
		FIBITMAP* image = FreeImage_Load(FreeImage_GetFileType(imageFile.c_str(), 0), imageFile.c_str());

		if (image == nullptr)
			return -1;

		FIBITMAP* image32Bit = FreeImage_ConvertTo32Bits(image);
		FreeImage_Unload(image);

		GLuint texID;

		glGenTextures(1, &texID);
		glBindTexture(GL_TEXTURE_2D, texID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB_ALPHA,
			FreeImage_GetWidth(image32Bit), FreeImage_GetHeight(image32Bit), 0,
			GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(image32Bit));

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		FreeImage_Unload(image32Bit);

		glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
	
}

bool Engine::checkCollision(const Object &obj1, const Object &obj2)
{
	//If either object doesn't have collider, stop because they can't collide
	if(obj1.collider == colliderType::none || obj2.collider == colliderType::none)
		return false;

	if (obj1.collider == obj2.collider)
	{
		vec3 distVec = obj1.transform.location - obj2.transform.location;
		float distSq = glm::dot(distVec, distVec);

		//Sphere-sphere
		if (obj1.collider == colliderType::sphere)
		{
			return (distSq < pow(player.transform.size.x, 2) || distSq < pow(obj2.transform.size.x, 2));
		}
		//AABB-AABB
		else
		{
			return abs(obj1.transform.location.x - obj2.transform.location.x) * 2 < (obj1.transform.size.x + obj2.transform.size.x) &&
				abs(obj1.transform.location.y - obj2.transform.location.y) * 2 < (obj1.transform.size.y + obj2.transform.size.y) &&
				abs(obj1.transform.location.z - obj2.transform.location.z) * 2 < (obj1.transform.size.z + obj2.transform.size.z);
		}
	}

	else
	{
		//Sphere-AABB
		float distSq = 0;

		vec3 boxMin;
		vec3 boxMax;
		vec3 sphereCenter;
		float radiusSq;

		if (obj1.collider == colliderType::axisAligned)
		{
			boxMin = obj1.transform.location - obj1.transform.size;
			boxMax = obj1.transform.location + obj1.transform.size;

			sphereCenter = obj2.transform.location;
			radiusSq = pow(obj2.transform.size.x, 2);
		}
		else
		{
			boxMin = obj2.transform.location - obj2.transform.size;
			boxMax = obj2.transform.location + obj2.transform.size;

			sphereCenter = obj1.transform.location;
			radiusSq = pow(obj1.transform.size.x, 2);
		}

		if (sphereCenter.x < boxMin.x)
		{
			distSq += pow((boxMin.x - sphereCenter.x), 2);
		}
		else if (sphereCenter.x > boxMax.x)
		{
			distSq += pow((sphereCenter.x - boxMax.x), 2);
		}

		if (sphereCenter.y < boxMin.y)
		{
			distSq += pow((boxMin.y - sphereCenter.y), 2);
		}
		else if (sphereCenter.y > boxMax.y)
		{
			distSq += pow((sphereCenter.y - boxMax.y), 2);
		}

		if (sphereCenter.z < boxMin.z)
		{
			distSq += pow((boxMin.z - sphereCenter.z), 2);
		}
		else if (sphereCenter.z > boxMax.z)
		{
			distSq += pow((sphereCenter.z - boxMax.z), 2);
		}

		return distSq < radiusSq;
	}

}

int Engine::checkCollision(const Car &car, const TrackPiece &trackPiece)
{
	vec3 distVector = player.transform.location - trackPiece.transform.location;

	//If square dist is >18, car is not on/near enough to the current tile so just return now
	if (dot(distVector, distVector) > 18.f)
	{
		return -1;
	}

	//Check both colliders 
	for (int i = 0; i < 2; i++)
	{
		//Sphere-AABB
		float distSq = 0;

		vec3 boxMin = trackPiece.colliders[i].location - trackPiece.colliders[i].size;
		vec3 boxMax = trackPiece.colliders[i].location + trackPiece.colliders[i].size;

		vec3 sphereCenter = player.transform.location;
		float  radiusSq = pow(player.transform.size.x, 2);


		if (sphereCenter.x < boxMin.x)
		{
			distSq += pow((boxMin.x - sphereCenter.x), 2);
		}
		else if (sphereCenter.x > boxMax.x)
		{
			distSq += pow((sphereCenter.x - boxMax.x), 2);
		}

		if (sphereCenter.y < boxMin.y)
		{
			distSq += pow((boxMin.y - sphereCenter.y), 2);
		}
		else if (sphereCenter.y > boxMax.y)
		{
			distSq += pow((sphereCenter.y - boxMax.y), 2);
		}

		if (sphereCenter.z < boxMin.z)
		{
			distSq += pow((boxMin.z - sphereCenter.z), 2);
		}
		else if (sphereCenter.z > boxMax.z)
		{
			distSq += pow((sphereCenter.z - boxMax.z), 2);
		}

		if (distSq < radiusSq)
		{
			return i;//Return index of collider hitting
		}
	}

	return -1;
	
}

void Engine::cameraUpdate()
{
	float rotDiff = player.transform.rotation.y - mainCamera.rotation.y;

	if (abs(rotDiff) > 0.01f)
	{
		mainCamera.rotation.y += rotDiff * deltaTime;
	}

	vec3 locDiff = (player.transform.location - (player.transform.forward * 2.5f) + vec3(0,0.75,0)) - mainCamera.location;

	if (length(locDiff) > 0.01f)
	{
		mainCamera.location += locDiff * 0.5f * deltaTime;
	}


	mainCamera.rotationMatrix = (mat3)yawPitchRoll(radians(mainCamera.rotation.y), radians(mainCamera.rotation.x), radians(mainCamera.rotation.z));

	mainCamera.matrix = perspective(mainCamera.fov, mainCamera.aspectRatio, mainCamera.zNear, mainCamera.zFar) *
		lookAt(mainCamera.location, player.transform.location, mainCamera.rotationMatrix * vec3(0,1,0));


	glUniformMatrix4fv(4, 1, GL_FALSE, &mainCamera.matrix[0][0]);
	glUniform3fv(5, 1, &mainCamera.location[0]);
}

void Engine::playerUpdate()
{
	//A and D keys rotate left and right

	if (keyIsDown[GLFW_KEY_A])
	{
		player.steer(1.f);
	}
	if (keyIsDown[GLFW_KEY_D])
	{
		player.steer(-1.f);
	}


	//W and S keys accelerate forward and backward based on rotation angle
	if (keyIsDown[GLFW_KEY_W])
	{
		player.accelerate(1.f);

	}
	else if (keyIsDown[GLFW_KEY_S])
	{
		player.accelerate(-1.f);
	}

	
/*
	//Check for collision
	for (unsigned int i = 0; i < gameObjects.size(); i++)
	{
		if (checkCollision(player, gameObjects[i]))
		{
			//player.rigidBody.velocity = player.transform.location - gameObjects[i].transform.location;
		}
	}
	

	for (unsigned int i = 0; i < tracks.size(); i++)
	{
		int trackCollIndex = checkCollision(player, tracks[i]);

		if (trackCollIndex >= 0)
		{
			player.accelerate(-1.2f);
			//player.rigidBody.velocity = (trackObjects[i].transfrm.location - trackObjects[i].colliders[trackCollIndex].location);

		}
	}*/

	//Update location by adding velocity
	player.updateCar(deltaTime);

	player.transform.updateMatrix();

	//Upload matrices to graphics card
	glUniformMatrix4fv(3, 1, GL_FALSE, &player.transform.matrix[0][0]);

	//Bind appropriate texture to draw
	glBindTexture(GL_TEXTURE_2D, textures[player.textureFile]);

	cameraUpdate();

	glUniform3fv(6, 1, &lightPos[0]);

	player.model.render();
}


bool Engine::gameLoop()
{	
	while (!glfwWindowShouldClose(GLFWwindowPtr))
	{
		prevTime = currentTime;
		currentTime = (float)glfwGetTime();

		deltaTime = currentTime - prevTime;

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Store keys from last frame and then update
		keyWasDown = keyIsDown;
		glfwPollEvents();

		//If escape pressed, need to exit game
		if (keyIsDown[GLFW_KEY_ESCAPE])
		{
			glfwSetWindowShouldClose(GLFWwindowPtr, GL_TRUE);
		}

		playerUpdate();


		//Run through gameObjects and update matrices
		for (unsigned int i = 0; i < gameObjects.size(); i++)
		{	

			//Update transform matrix
			gameObjects[i].transform.matrix = glm::translate(gameObjects[i].transform.location) *
				yawPitchRoll(gameObjects[i].transform.rotation.y, gameObjects[i].transform.rotation.x, gameObjects[i].transform.rotation.z) *
				glm::scale(gameObjects[i].transform.size);



			//Upload matrices to graphics card
			glUniformMatrix4fv(3, 1, GL_FALSE, &gameObjects[i].transform.matrix[0][0]);

			//Bind appropriate texture to draw
			glBindTexture(GL_TEXTURE_2D, textures[gameObjects[i].textureFile]);

			cameraUpdate();

			glUniform3fv(6, 1, &lightPos[0]);

			gameObjects[i].model.render();

		}

		for (unsigned int i = 0; i < tracks.size(); i++)
		{
			glUniformMatrix4fv(3, 1, GL_FALSE, &tracks[i].transform.matrix[0][0]);

			glBindTexture(GL_TEXTURE_2D, textures[tracks[i].textureFile]);

			cameraUpdate();

			glUniform3fv(6, 1, &lightPos[0]);

			tracks[i].model.render();
		}




		//Swap front (what screen displays) and back (what OpenGL draws to) buffers
		glfwSwapBuffers(GLFWwindowPtr);



	}

	glfwTerminate();

	return true;
}





