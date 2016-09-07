#include "Car.h"

Car::Car() : Car(vec3(0), vec3(0), vec3(0), "textures/CarModel.jpg", "models/CarModel.obj", colliderType::sphere, 1.f, 1.f, 1.f){}

Car::Car(vec3 loc, vec3 rot, vec3 size, const string texFile, const string modFile,	colliderType coll,
	float m, float maxS, float turnS) : Object(loc, rot, size, texFile, modFile, coll)
{
	

	maxSpeed = maxS;
	turnSpeed = turnS;

	mass = m;

	rigidbody = Rigidbody(mass);

}

Car::~Car()
{
}

void Car::steer(float direction)
{
	rigidbody.addTorque(direction * turnSpeed);
}

void Car::accelerate(float direction)
{
	rigidbody.addForce(direction * maxSpeed * transform.forward);
}


void Car::updateCar(float deltaTime)
{
	transform.updateForward();

	rigidbody.updatePhysics(maxSpeed, turnSpeed, 1.f);

	transform.location += rigidbody.velocity * deltaTime;

	transform.rotation.y += rigidbody.angularSpeed * deltaTime;

}

