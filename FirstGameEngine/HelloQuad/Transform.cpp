#include "Transform.h"



Transform::Transform()
{
	Transform(vec3(0),vec3(0),vec3(0));
}

Transform::Transform(vec3 loc, vec3 rot, vec3 siz)
{
	location = loc;
	rotation = rot;
	size = siz;

	updateMatrix();
	updateForward();
}

Transform::~Transform(){ }

void Transform::updateMatrix()
{
	matrix = translate(location) *
		yawPitchRoll(radians(rotation.y), radians(rotation.x), radians(rotation.z)) *
		glm::scale(size);
}

void Transform::updateForward()
{
	forward = vec3(sinf(radians(rotation.y)),0,cosf(radians(rotation.y)));
	right = vec3(-forward.z,0,forward.x);//Get right vector from forward
}
