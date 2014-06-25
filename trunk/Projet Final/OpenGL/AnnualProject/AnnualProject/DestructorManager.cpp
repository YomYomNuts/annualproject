#include "DestructorManager.h"
#include "GameObject.h"
#include "DelaunayGenerator.h"
#include "ParaboloidSurfaceProjection.h"
#include "VoronoiGenerator.h"


DestructorManager::DestructorManager(void)
{
}

DestructorManager::~DestructorManager(void)
{
}

void DestructorManager::LaunchDestruction(GameObjectManager & gom, GameObject * goImpact, vec3 positionImpact, vec3 positionLookAt)
{
	std::vector<unsigned short> * listIndexNeighbour = goImpact->RemoveVertexAround(vec3(0,0,0), 3.0f);
	goImpact->CalculateIndexes();
	GameObject * go = new GameObject();
	std::vector<unsigned short> * listVerticesConvexPolygon = DelaunayGenerator::LaunchCircleGeneration(8.0f, 100, go->listVertices, go->listEdges, go->listFaces);
	ParaboloidSurfaceProjection::LaunchProjection(8.0f, 5.0f, go->listVertices, go->listEdges, go->listFaces);
	go->Scale(0.7f);
	VoronoiGenerator::GenerateVoronoi(gom, go, positionImpact, positionLookAt, vec3(0.0f, -2.5f, 0.0f), 8.0f, 5.0f, 0.7f, 64);
	go->TranslateAndLookAtAndScale(positionImpact, positionLookAt, vec3(0.0f, -2.5f, 0.0f), 1.3f);
	go->CalculateIndexes();
	goImpact->Merge(positionImpact, go, listVerticesConvexPolygon, listIndexNeighbour);
	delete listVerticesConvexPolygon;
}