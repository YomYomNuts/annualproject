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

void DestructorManager::LaunchDestruction(ObjectManager & gom, Generic_Object * goImpact, vec3 positionImpact, vec3 positionLookAt, float radius, float depth, int nbPointsCircle, int nbParticlesVoronoi)
{
	std::vector<unsigned short> * listIndexNeighbour = goImpact->RemoveVertexAround(positionImpact, radius * 1.1f);
	goImpact->CalculateIndexes();
	Generic_Object * go = new Generic_Object();
	std::vector<unsigned short> * listVerticesConvexPolygon = DelaunayGenerator::LaunchCircleGeneration(radius, nbPointsCircle, go->verticesList, go->listEdges, go->listFaces);
	ParaboloidSurfaceProjection::LaunchProjection(radius, depth, go->verticesList, go->listEdges, go->listFaces);
	go->Scale(0.7f);
	//VoronoiGenerator::GenerateVoronoi(gom, go, positionImpact, positionLookAt, vec3(0.0f, -height/2, 0.0f), radius, height, 0.7f, nbParticlesVoronoi);
	go->TranslateAndLookAtAndScale(vec3(0.0f, 0.0f, 0.0f), positionLookAt, vec3(0.0f, -depth/2, 0.0f) + positionImpact, 1.3f);
	go->CalculateIndexes();
	goImpact->Merge(positionImpact, vec3(0.0f, -depth/2, 0.0f), go, listVerticesConvexPolygon, listIndexNeighbour);
	delete listVerticesConvexPolygon;
}