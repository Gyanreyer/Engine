#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <FreeImage.h>
#include <vector>
#include <map>
#include <glm/gtx/euler_angles.hpp>
#include <math.h>

#include "ShaderManager.h"
#include "Model.h"

#include "Car.h"
#include "TrackPiece.h"
#include "Object.h"

#include "Time.h"


using std::vector;
using std::map;
using glm::vec3;
using glm::vec2;
using glm::mat4;
using glm::mat3;

using glm::length;
using glm::normalize;
using glm::distance;
using glm::yawPitchRoll;
using glm::perspective;
using glm::lookAt;
using glm::radians;


class Engine
{
public:
	Engine();
	~Engine();

	const float PI = 3.14159265f;

	GLFWwindow* GLFWwindowPtr;

	bool init();
	bool bufferModels();
	bool gameLoop();
	bool useShaders();
	
	bool checkCollision(const Object &obj1, const Object &obj2);
	int checkCollision(const Car &car, const TrackPiece &trackPiece);

	vec3 lightPos;

	struct camera
	{
		vec3 location;
		vec3 rotation;

		mat3 rotationMatrix;

		float zoom;
		int width;
		int height;

		float fov;
		float aspectRatio;
		float zNear;
		float zFar;	

		mat4 matrix;
	};

	float currentTime;
	float prevTime;
	float deltaTime;

	camera mainCamera;

	Car player;
	vector<Object> gameObjects;
	vector<TrackPiece> tracks;	
	
private:
	map<const string, GLuint> textures;

	ShaderManager shaderManager;
	void cameraUpdate();
	void playerUpdate();
	GLuint loadImage(const string imageFile);
};

