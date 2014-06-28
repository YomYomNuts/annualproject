#ifndef __PARABOLOIDSURFACEPROJECTION__
#define __PARABOLOIDSURFACEPROJECTION__

#include "../../EsgiGL/Common/vector.h"
#include "../../EsgiGL/Common/matrix.h"
#include "../../EsgiGL/EsgiGL.h"
#include "Edge.h"
#include "Face.h"

#include <vector>

class ParaboloidSurfaceProjection
{
	// Attributs
public:
protected:
private:
	
	//Methods
public:
	ParaboloidSurfaceProjection(void);
	~ParaboloidSurfaceProjection(void);
	static void LaunchProjection(float radius, float height, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces);
protected:
private:
};

#endif