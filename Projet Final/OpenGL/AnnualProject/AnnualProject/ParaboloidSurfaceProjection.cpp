#include "ParaboloidSurfaceProjection.h"

ParaboloidSurfaceProjection::ParaboloidSurfaceProjection(void)
{
}

ParaboloidSurfaceProjection::~ParaboloidSurfaceProjection(void)
{
}

void ParaboloidSurfaceProjection::LaunchProjection(float radius, float height, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces)
{
	float coefficient = height / (pow(radius, 2));

	for (unsigned int i = 0; i < listVertex->size(); ++i)
	{
		listVertex->at(i).z = coefficient * pow(listVertex->at(i).x, 2) + coefficient * pow(listVertex->at(i).y, 2) - height/2;
	}
}