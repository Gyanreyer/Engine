#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/euler_angles.hpp>

using glm::vec3;
using glm::mat4;

using glm::translate;
using glm::yawPitchRoll;
using glm::scale;
using glm::radians;

class Transform
{
public:
	Transform();
	Transform(vec3 loc, vec3 rot, vec3 siz);
	~Transform();

	vec3 location;
	vec3 rotation;
	vec3 size;
	mat4 matrix;

	vec3 forward;
	vec3 right;

	void updateMatrix();
	void updateForward();
};

