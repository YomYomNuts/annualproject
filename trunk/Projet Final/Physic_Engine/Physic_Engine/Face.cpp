#include "Face.h"

#include <map>

Face::Face(void)
{
	this->edge1 = NULL;
	this->edge2 = NULL;
	this->edge3 = NULL;
}

Face::Face(const Face * face)
{
	this->edge1 = new Edge(face->edge1);
	this->edge2 = new Edge(face->edge2);
	this->edge3 = new Edge(face->edge3);
	this->normalInverse = face->normalInverse;
}

Face::Face(Edge * edge1, Edge * edge2, Edge * edge3)
{
	this->edge1 = edge1;
	this->edge2 = edge2;
	this->edge3 = edge3;
}

Face::~Face(void)
{
}

bool Face::VertexInFace(vec3 * pos, vec3 * pos0, vec3 * pos1, vec3 * pos2, vec3 * pos3)
{
	if (pos0 == pos2)
	{
		if (pos0->Convex(pos1, pos3) <= 0) // Right
		{
			if (pos0->Convex(pos1, pos) <= 0 && pos3->Convex(pos0, pos) <= 0 && pos1->Convex(pos3, pos) <= 0)
				return true;
			else
				return false;
		}
		else
		{
			if (pos1->Convex(pos0, pos) <= 0 && pos3->Convex(pos1, pos) <= 0 && pos0->Convex(pos3, pos) <= 0)
				return true;
			else
				return false;
		}
	}
	else if (pos1 == pos2)
	{
		if (pos1->Convex(pos0, pos3) <= 0) // Right
		{
			if (pos1->Convex(pos0, pos) <= 0 && pos3->Convex(pos1, pos) <= 0 && pos0->Convex(pos3, pos) <= 0)
				return true;
			else
				return false;
		}
		else
		{
			if (pos0->Convex(pos1, pos) <= 0 && pos3->Convex(pos0, pos) <= 0 && pos1->Convex(pos3, pos) <= 0)
				return true;
			else
				return false;
		}
	}
	else if (pos0 == pos3)
	{
		if (pos0->Convex(pos1, pos2) <= 0) // Right
		{
			if (pos0->Convex(pos1, pos) <= 0 && pos2->Convex(pos0, pos) <= 0 && pos1->Convex(pos2, pos) <= 0)
				return true;
			else
				return false;
		}
		else
		{
			if (pos1->Convex(pos0, pos) <= 0 && pos2->Convex(pos1, pos) <= 0 && pos0->Convex(pos2, pos) <= 0)
				return true;
			else
				return false;
		}
	}
	else
	{
		if (pos1->Convex(pos0, pos2) <= 0) // Right
		{
			if (pos1->Convex(pos0, pos) <= 0 && pos2->Convex(pos1, pos) <= 0 && pos0->Convex(pos2, pos) <= 0)
				return true;
			else
				return false;
		}
		else
		{
			if (pos0->Convex(pos1, pos) <= 0 && pos2->Convex(pos0, pos) <= 0 && pos1->Convex(pos2, pos) <= 0)
				return true;
			else
				return false;
		}
	}
}

void Face::Circumcenter3D(vec3 * a, vec3 * b, vec3 * c, vec3 * circumcenter)
{
	float denominator = 2 * (a->x * (b->y - c->y) + b->x * (c->y - a->y) + c->x * (a->y - b->y));
	float powerA = a->x * a->x + a->y * a->y;
	float powerB = b->x * b->x + b->y * b->y;
	float powerC = c->x * c->x + c->y * c->y;

	if (denominator == 0)
	{
		// Tous colinéaires
		if (a->x == 0 && b->x == 0 && c->x == 0)
		{
			circumcenter->x = 0;
			circumcenter->y = (a->y + b->y + c->y) / 3;
			circumcenter->z = (a->z + b->z + c->z) / 3;
		}
		else
		{
			circumcenter->x = (a->x + b->x + c->x) / 3;
			circumcenter->y = a->y;
			circumcenter->z = (a->z + b->z + c->z) / 3;
		}
	}
	else
	{
		circumcenter->x = (powerA * (b->y - c->y) + powerB * (c->y - a->y) + powerC * (a->y - b->y)) / denominator;
		circumcenter->y = (powerA * (c->x - b->x) + powerB * (a->x - c->x) + powerC * (b->x - a->x)) / denominator;
		circumcenter->z = 0;
	}
}

void Face::VerifyFace(std::vector<vec3> * listVertex, vec3 & directionNormal)
{
	Edge * edge1 = this->edge1, * edge2 = this->edge2;
	vec3 * p0 = &listVertex->at(edge1->indexVertex1);
	vec3 * p1 = &listVertex->at(edge1->indexVertex2);
	vec3 cross;

	if (edge1->indexVertex1 == edge2->indexVertex1)
		cross = (*p0 - *p1).Cross(listVertex->at(edge2->indexVertex2) - *p1);
	else if (edge1->indexVertex1 == edge2->indexVertex2)
		cross = (*p0 - *p1).Cross(listVertex->at(edge2->indexVertex1) - *p1);
	else if (edge1->indexVertex2 == edge2->indexVertex2)
		cross = (*p0 - *p1).Cross(listVertex->at(edge2->indexVertex1) - *p1);
	else
		cross = (*p0 - *p1).Cross(listVertex->at(edge2->indexVertex2) - *p1);

	this->normalInverse = true;
	if (cross.x < 0 && directionNormal.x > 0 || cross.x > 0 && directionNormal.x < 0 ||
		cross.y < 0 && directionNormal.y > 0 || cross.y > 0 && directionNormal.y < 0 ||
		cross.z < 0 && directionNormal.z > 0 || cross.z > 0 && directionNormal.z < 0)
		this->normalInverse = false;
}

void Face::VerifyFaces(std::vector<vec3> * listVertex, std::vector<Face*> * listFaces, vec3 & directionNormal)
{
	for (unsigned int i = 0; i < listFaces->size(); ++i)
	{
		listFaces->at(i)->VerifyFace(listVertex, directionNormal);
	}
}