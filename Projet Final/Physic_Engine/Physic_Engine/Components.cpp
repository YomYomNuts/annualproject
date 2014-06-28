#include "Components.h"


Components::Components(void)
{
	this->gravity = Gravity();
	this->quaternion = Quaternion();
	this->rigidbody = RigidBody(RIGID_NONE);
}

Gravity* Components::getGravity()
{
	return &this->gravity;
}

RigidBody* Components::getRigidBody()
{
	return &this->rigidbody;
}

Quaternion* Components::getQuaternion()
{
	return &this->quaternion;
}

Components::~Components(void)
{
}
