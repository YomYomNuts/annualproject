#ifndef __DESTRUCTORMANAGER__
#define __DESTRUCTORMANAGER__

#include "GameObjectManager.h"

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
	static void LaunchDestruction(GameObjectManager & gom, GameObject * goImpact, vec3 positionImpact, vec3 positionLookAt);
protected:
private:
};

#endif