#pragma once
#include <glm/glm.hpp>
#include "Transform.h"

#include <iostream>

using glm::vec3;


class Rigidbody
{
public:
	Rigidbody();
	Rigidbody(float m);
	~Rigidbody();

	vec3 velocity;
	vec3 force;
	float mass;

	vec3 airDrag;
	vec3 rollResistance;

	float angularSpeed;
	float torque;
	float inertia;

	float friction;
	float angularFriction;
	float dragConst;

	float speed;

	void addForce(vec3 addedForce);
	void addTorque(float addedTorque);

	void updatePhysics(float maxSpeed, float maxTurnSpeed, float deltaTime);

};

