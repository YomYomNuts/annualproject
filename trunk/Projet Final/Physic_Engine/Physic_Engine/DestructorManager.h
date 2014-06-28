#ifndef __DESTRUCTORMANAGER__
#define __DESTRUCTORMANAGER__

//#include "GameObjectManager.h"
#include "Generic_Object.h"
#include "ObjectManager.h"

class DestructorManager
{
	// Attributs
public:
protected:
private:

	//Methods
public:
	DestructorManager(void);
	~DestructorManager(void);
	static void LaunchDestruction(ObjectManager & gom, Generic_Object * goImpact, vec3 positionImpact, vec3 positionLookAt);
protected:
private:
};

#endif