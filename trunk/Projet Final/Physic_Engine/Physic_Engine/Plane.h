#ifndef __PLANE__
#define __PLANE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "GameObject.h"

class Plane:public GameObject
{
public:
	Plane(void);
	Plane(vec3 upperRight, vec3 upperLeft, vec3 lowerRight,vec3 lowerLeft); 
	void render(GLint *positionAttrib, GLint *colorUniform);
	vector<vec3> getCoordinates();
	void update(float elapsedTime);
	void changePosition(float X, float Y, float Z);
	void rotate(vec3 velocity, float elapsedTime);
	vector<vec3>* getListOfVertices();
	vector<GLushort>* getListOfIndexes();
	void clearLists();
	void deleteLists();
	~Plane(void);

	
private:
	vector<vec3> verticesList;
	vector<GLushort> indices;
	vec3 quad[4];
	
	MathFunctions MF;

};

#endif