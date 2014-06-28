#ifndef __RIGIDBODY_PLANE__
#define __RIGIDBODY_PLANE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "MathFunctions.h"
#include "Quaternion.h"

using namespace std;

class RigidBody_Plane
{
public:
	bool isNULL;

	RigidBody_Plane(void);
	RigidBody_Plane(vec3 position, float width, float length);
	void render(GLint *positionAttrib, GLint *colorUniform);
	vec3 getNormal();
	vec3 getCenterOfObject();
	void rotate(float angle, vec3 u);
	vector<vec3> getCoordinates();
	void changePosition(float posX, float posY, float posZ);
	void setIsStatic(bool movable);
	bool getIsStatic();
	~RigidBody_Plane(void);

private:
	vec3 normal;
	vector<vec3> verticesList;
	vector<GLushort> indices;
	vec3 centerOfObject;
	bool isStatic;

	vector<vec3> axeX;
	vector<vec3> axeY;
	vector<vec3> axeZ;

	MathFunctions MF;
	Quaternion quaternion;

	
	
};

#endif