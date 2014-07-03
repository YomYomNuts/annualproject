#ifndef __GENERIC_OBJECT__
#define __GENERIC_OBJECT__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "GameObject.h"
#include "Edge.h"
#include "Face.h"

using namespace std;

class Generic_Object : public GameObject
{
public:
	Generic_Object(void);
	Generic_Object(vector<vec3> &verticesList, vector<GLushort> &indexesList, float posX, float posY, float posZ, bool worldPosition);
	void setColor(float r, float g, float b, float a);
	void setWireframeMode(bool active);
	float getSize();
	void update(float elapsedTime);
	void render(GLint *positionAttrib, GLint *colorUniform, GLint *texture_uniform);
	void changePosition(float X, float Y, float Z);
	void rotate(vec3 velocity, float elapsedTime);
	void calculateNormalsAndCenterOfGravities();
	vector<vec3>* getListOfNormals();
	vector<vec3>* getListOfCenterOfGravities();
	vector<vec3>* getListOfVertices();
	vector<GLushort>* getListOfIndexes();
	vector<vec3>* getListIndexesFaces();
	void CalculateIndexes();
	void Scale(float valueScale);
	void Merge(vec3 positionImpact, Generic_Object * go, std::vector<unsigned short> * listVerticesConvexPolygon, std::vector<unsigned short> * listIndexNeighbour);
	std::vector<unsigned short> * RemoveVertexAround(vec3 positionImpact, float radius);
	void TranslateAndLookAtAndScale(const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float valueScale);
	void calculateCenterOfObject();
	void clearLists();
	void deleteLists();
	~Generic_Object(void);

	vector<GLushort>* indexesList;
	vector<vec3>* verticesList;

	vector<Edge*>* listEdges;
	vector<Face*>* listFaces;

	vector<unsigned short> * listIndexesWireframe;
	vector<unsigned short> * listIndexesPoint;

private:
	
	float size;
};



#endif