#ifndef __COMPONENTS__
#define __COMPONENTS__

#include "RigidBody.h"
#include "Quaternion.h"
#include "Gravity.h"

class Components
{
public:
	Components(void);
	RigidBody* getRigidBody();
	Gravity* getGravity();
	Quaternion* getQuaternion();
	~Components(void);

private:

	RigidBody rigidbody;
	Quaternion quaternion;
	Gravity gravity;
};

#endif