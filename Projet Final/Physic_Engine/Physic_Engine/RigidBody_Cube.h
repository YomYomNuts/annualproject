#ifndef __RIGIDBODY_CUBE__
#define __RIGIDBODY_CUBE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "MathFunctions.h"
#include "Quaternion.h"

using namespace std;

class RigidBody_Cube
{
public:

	bool isNULL;

	RigidBody_Cube(void);
	RigidBody_Cube(vec3 position, float size);
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
	~RigidBody_Cube(void);

private:
	vector<vec3> verticesList;
	vector<GLushort> indices;
	vec3 centerOfObject;
	float size;
	bool isStatic;

	vector<vec3> axeX;
	vector<vec3> axeY;
	vector<vec3> axeZ;

	MathFunctions MF;
	Quaternion quaternion;

	vector<vec3> listOfNormals;
	vector<vec3> listOfCenterOfGravities;
	vector<vec3> listIndexesFaces;

};

#endif