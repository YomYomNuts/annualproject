#ifndef __DELAUNAYGENERATOR__
#define __DELAUNAYGENERATOR__

#include "../../EsgiGL/Common/vector.h"
#include "../../EsgiGL/EsgiGL.h"
#include "Edge.h"
#include "Face.h"
#include "GameObject.h"

#include <vector>

class DelaunayGenerator
{
	// Attributs
public:
protected:
private:

	//Methods
public:
	DelaunayGenerator(void);
	~DelaunayGenerator(void);
	static std::vector<unsigned short>* LaunchCircleGeneration(float radiusZone, int nbVerticesGenerate, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces);
	static void LaunchTriangulationGeneration(GameObject * go, std::vector<unsigned short> * listIndexVerticesFaces, vec3 & normalFace);
	static void AddPoint(vec3 * pos, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces, std::vector<vec3*> * listVerticesConvexPolygon);
protected:
private:
};

#endif