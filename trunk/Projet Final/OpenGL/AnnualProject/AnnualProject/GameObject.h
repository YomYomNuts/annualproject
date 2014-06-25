#ifndef __OBJECT__
#define __OBJECT__

#include "../../EsgiGL/Common/vector.h"
#include "../../EsgiGL/EsgiGL.h"
#include "Edge.h"
#include "Face.h"

#include <vector>

class GameObject
{
	// Attributs
public:
	std::vector<vec3> * listVertices;
	std::vector<Edge*> * listEdges;
	std::vector<Face*> * listFaces;
	std::vector<unsigned short> * listIndexes;
	std::vector<unsigned short> * listIndexesWireframe;
	std::vector<unsigned short> * listIndexesPoint;
	vec4 * color;
protected:
private:

	//Methods
public:
	GameObject(void);
	GameObject(const GameObject * go);
	~GameObject(void);
	void TranslateAndLookAtAndScale(const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float valueScale);
	void Scale(float valueScale);
	void Move(vec3 positionToAdd);
	std::vector<unsigned short> * RemoveVertexAround(vec3 positionImpact, float radius);
	void Merge(vec3 positionImpact, GameObject * go, std::vector<unsigned short> * listVerticesConvexPolygon, std::vector<unsigned short> * listIndexNeighbour);
	void CalculateIndexes();
	virtual void RenderObject(GLint & position_attrib, GLint & color_attrib);
protected:
private:
};

#endif