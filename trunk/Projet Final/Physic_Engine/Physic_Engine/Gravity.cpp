#include "Gravity.h"

Gravity::Gravity(void)
{
	this->gravity = 0.3f;
	this->time = 0;
	this->useGravity = false;
	this->weight = 0.0f;
	this->force = vec3(0.0f, 0.0f, 0.0f);

	this->resistance = 1000.0f;
	this->bounciness = 1000.0f;
}

Gravity::Gravity(float gravity)
{
	this->gravity = gravity;
	this->time = 0;
	this->useGravity = false;
	this->weight = 0.0f;
	this->force = vec3(0.0f, 0.0f, 0.0f);

	this->resistance = 1000.0f;
	this->bounciness = 1000.0f;
}


void Gravity::setGravity(float gravity)
{
	this->gravity = gravity;
}

void Gravity::setUseGravity(bool use)
{
	this->useGravity = use;
}

bool Gravity::getUseGravity()
{
	return this->useGravity;
}

void Gravity::resetTime()
{
	this->time = 0.0f;
}

void Gravity::setWeight(float weight)
{
	this->weight = this->gravity * weight;
}

void Gravity::addForce(vec3 force, vec3* velocity, float elapsedTime)
{
	//this->time += elapsedTime;

	this->force.x += force.x;
	this->force.y += force.y;
	this->force.z += force.z;

	velocity->x += this->gravity * force.x * elapsedTime;
	velocity->y += this->gravity * force.y * elapsedTime;
	velocity->z += this->gravity * force.z * elapsedTime;
}

vec3 Gravity::getForce()
{
	return this->force;
}

float Gravity::getResistance()
{
	return this->resistance;
}

float Gravity::getBounciness()
{
	return this->bounciness;
}

void Gravity::setResistance(float mass, float resistance)
{
	this->resistance = mass / resistance;
}

void Gravity::setBounciness(float mass, float bounciness)
{
	this->bounciness = mass / bounciness;
}

Gravity::~Gravity(void)
{
}
