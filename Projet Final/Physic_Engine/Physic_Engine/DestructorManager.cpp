#include "DestructorManager.h"
//#include "GameObject.h"
#include "DelaunayGenerator.h"
#include "ParaboloidSurfaceProjection.h"
#include "VoronoiGenerator.h"



DestructorManager::DestructorManager(void)
{
}

DestructorManager::~DestructorManager(void)
{
}

void DestructorManager::LaunchDestruction(ObjectManager & gom, Generic_Object * goImpact, vec3 positionImpact, vec3 positionLookAt)
{
	float radius = 45.0f;
	float height = 30.0f;
	int nbPointsCircle = 40;
	int nbParticlesVoronoi = 5;

	std::vector<unsigned short> * listIndexNeighbour = goImpact->RemoveVertexAround(vec3(0,0,0), radius * 1.1f);
	goImpact->CalculateIndexes();
	Generic_Object * go = new Generic_Object();
	std::vector<unsigned short> * listVerticesConvexPolygon = DelaunayGenerator::LaunchCircleGeneration(radius, nbPointsCircle, go->verticesList, go->listEdges, go->listFaces);
	ParaboloidSurfaceProjection::LaunchProjection(radius, height, go->verticesList, go->listEdges, go->listFaces);
	go->Scale(0.7f);
	VoronoiGenerator::GenerateVoronoi(gom, go, positionImpact, positionLookAt, vec3(0.0f, -height/2, 0.0f), radius, height, 0.7f, nbParticlesVoronoi);
	go->TranslateAndLookAtAndScale(positionImpact, positionLookAt, vec3(0.0f, -height/2, 0.0f), 1.3f);
	go->CalculateIndexes();
	goImpact->Merge(positionImpact, go, listVerticesConvexPolygon, listIndexNeighbour);
	delete listVerticesConvexPolygon;
}