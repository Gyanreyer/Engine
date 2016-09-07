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

#include <fstream>
#include <string>
#include <sstream>

#include <iostream>


using std::vector;
using std::string;

using std::ifstream;
using std::getline;
using std::istringstream;

using glm::vec3;
using glm::vec2;

class Model
{
public:
	Model();
	~Model();

	bool buffer(string objFile);

	void render();

private:
	//Numbers identifying array and buffer we're generating
	unsigned int vertCount;
	GLuint vertArr;

};

struct vertex
{
	vec3 locs;
	vec2 uv;
	vec3 normal;
};

struct vertInd
{
	unsigned int locInd;
	unsigned int uvInd;
	unsigned int normInd;

};

