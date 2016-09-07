#include "Rigidbody.h"

Rigidbody::Rigidbody() : Rigidbody(1.f){}

Rigidbody::Rigidbody(float m)
{
	mass = m;
	inertia = mass;
	speed = 0;

	torque = 0;
	angularSpeed = 0;
	velocity = vec3(0);
	force = vec3(0);

	friction = 0.999f;

	angularFriction = 0.997f;
}

Rigidbody::~Rigidbody()
{
}


void Rigidbody::addForce(vec3 addedForce)
{
	force += addedForce;
	std::cout << force.x << ", " << force.z << "\n";
}

void Rigidbody::addTorque(float addedTorque)
{
	torque += addedTorque;
}



void Rigidbody::updatePhysics(float maxSpeed, float maxTurnSpeed, float deltaTime)
{

	velocity += (force / mass) * deltaTime;
	force *= 0;

	speed = glm::length(velocity);

	if (speed > maxSpeed)
	{
		velocity = glm::normalize(velocity) * maxSpeed;
		speed = maxSpeed;
	}
	

	angularSpeed += (torque / mass) * deltaTime;
	torque = 0;

	if(angularSpeed > maxTurnSpeed)
	{
		angularSpeed = maxTurnSpeed;
	}
	else if (angularSpeed < -maxTurnSpeed)
	{
		angularSpeed = -maxTurnSpeed;
	}


	velocity *= friction;
	angularSpeed *= angularFriction;

}
