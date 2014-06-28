#ifndef __RIGIDBODY_SPHERE__
#define __RIGIDBODY_SPHERE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "MathFunctions.h"
#include "Quaternion.h"

using namespace std;

class RigidBody_Sphere
{
public:
	bool isNULL;

	RigidBody_Sphere(void);
	RigidBody_Sphere(vec3 position, float radius);
	void render(GLint *positionAttrib, GLint *colorUniform);
	float getRadius();
	vec3 getCenterOfObject();
	vec3 getNormal();
	void changePosition(float posX, float posY, float posZ);
	void rotate(float angle, vec3 u);
	void setIsStatic(bool movable);
	bool getIsStatic();
	~RigidBody_Sphere(void);


private:
	unsigned int nbVertex;
	vector<GLushort> indices;
	float radius;
	vec3 centerOfObject;
	bool isStatic;
	
	vector<vec3> frontTriangle;
	vector<vec3> leftTriangle;
	vector<vec3> rearTriangle;
	vector<vec3> rightTriangle;
	vector<vec3> frontTriangleBottom;
	vector<vec3> leftTriangleBottom;
	vector<vec3> rearTriangleBottom;
	vector<vec3> rightTriangleBottom;

	vector<vec3> axeX;
	vector<vec3> axeY;
	vector<vec3> axeZ;

	MathFunctions MF;
	Quaternion quaternion;

	void createSphere(unsigned int levelOfDetail);
	void defineIndices(int levelOfDetail);
	
};

#endif