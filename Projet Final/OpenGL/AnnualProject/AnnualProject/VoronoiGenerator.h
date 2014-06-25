#ifndef __VORONOIGENERATOR__
#define __VORONOIGENERATOR__

#include "GameObject.h"
#include "GameObjectManager.h"

class VoronoiGenerator
{
	// Attributs
public:
protected:
private:
	
	//Methods
public:
	VoronoiGenerator(void);
	~VoronoiGenerator(void);
	static void GenerateVoronoi(GameObjectManager & gom, const GameObject * baseGameObject, const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float radius, float height, float scale, const int particles);
protected:
private:
};

#endif