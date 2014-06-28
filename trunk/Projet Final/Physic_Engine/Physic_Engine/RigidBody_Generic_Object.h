#ifndef __RIGIDBODY_GENERIC_OBJECT__
#define __RIGIDBODY_GENERIC_OBJECT__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "MathFunctions.h"
#include "Quaternion.h"

using namespace std;

class RigidBody_Generic_Object
{
public:

	bool isNULL;

	RigidBody_Generic_Object(void);
	RigidBody_Generic_Object(vector<vec3> verticesList, vector<GLushort> indexesList, float posX, float posY, float posZ);
	void render(GLint *positionAttrib, GLint *colorUniform);
	vec3 getCenterOfObject();
	vector<vec3>* getListOfVertices();
	void changePosition(float posX, float posY, float posZ);
	void rotate(float angle, vec3 u);
	void calculateNormalsAndCenterOfGravities();
	vector<vec3>* getListIndexesFaces();
	vector<vec3>* getListOfCenterOfGravities();
	vector<vec3>* getListOfNormals();
	vector<GLushort>* getListOfIndexes();
	float getSize();
	vec3 getPosLowestVertex();
	unsigned int getLowestVertex();
	// Return vec4(index of face, position of intersection)
	vec4 getCollidingFace();
	void setIsStatic(bool movable);
	bool getIsStatic();
	void setSize(float size);
	void setAlternateFacesAndNormals(bool aternateFacesAndNormals);
	bool getAlternateFacesAndNormals();
	void changeVerticesList(vector<vec3> verticesList);
	void changeIndexesList(vector<GLushort> indexesList);
	void resetWireframeModeindexes(vector<GLushort> indexesList);
	void calculateCenterOfObject();
	~RigidBody_Generic_Object(void);

private:
	vector<vec3> verticesList;
	vector<GLushort> indices;
	vec3 centerOfObject;
	float size;
	bool isStatic;
	bool alternateFacesAndNormals;

	vector<vec3> axeX;
	vector<vec3> axeY;
	vector<vec3> axeZ;

	MathFunctions MF;
	Quaternion quaternion;

	vector<vec3> listOfNormals;
	vector<vec3> listOfCenterOfGravities;
	vector<vec3> listIndexesFaces;

	vector<GLushort> listIndexesWireframe;

};

#endif