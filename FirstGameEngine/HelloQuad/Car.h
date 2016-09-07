#pragma once
#include <map>

#include "Object.h"
#include "Rigidbody.h"

class Car :
	public Object
{
public:
	Car();
	Car(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile, colliderType coll, float m, float maxS, float turnS);
	~Car();

	Rigidbody rigidbody;

	float mass;

	float maxSpeed;
	float turnSpeed;

	float steerAngle;
	float throttle;
	float brake;

	float wheelbase;

	float torque;


	void steer(float direction);
	void accelerate(float direction);

	void updateCar(float deltaTime);

};

