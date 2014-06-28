#ifndef __FACE__
#define __FACE__

#include "Edge.h"

#include <vector>

class Face
{
	// Attributs
public:
	Edge * edge1;
	Edge * edge2;
	Edge * edge3;
	bool normalInverse;
protected:
private:

	//Methods
public:
	Face(void);
	Face(const Face * face);
	Face(Edge * edge1, Edge * edge2, Edge * edge3);
	~Face(void);
	void VerifyFace(std::vector<vec3> * listVertex, vec3 & directionNormal);
	static bool VertexInFace(vec3 * pos, vec3 * pos0, vec3 * pos1, vec3 * pos2, vec3 * pos3);
	static void Circumcenter3D(vec3 * a, vec3 * b, vec3 * c, vec3 * circumcenter);
	static void VerifyFaces(std::vector<vec3> * listVertex, std::vector<Face*> * listFaces, vec3 & directionNormal);
protected:
private:
};

#endif