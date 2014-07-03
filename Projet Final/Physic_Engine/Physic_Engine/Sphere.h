#ifndef __SPHERE__
#define __SPHERE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "GameObject.h"

using namespace std;

class Sphere:public GameObject
{
public:
	Sphere(void);
	Sphere(float posX, float posY, float posZ);
	Sphere(float posX, float posY, float posZ, float radius, int levelOfDetail);
	void render(GLint *positionAttrib, GLint *colorUniform);
	void update(float elapsedTime);
	float getRadius();
	void changePosition(float posX, float posY, float posZ);
	void rotate(vec3 velocity, float elapsedTime);
	vector<GLushort>* getListOfIndexes();
	void clearLists();
	void deleteLists();
	~Sphere(void);
	
private:
	unsigned int nbVertex;
	vector<GLushort> indices;
	float radius;

	vector<vec3> frontTriangle;
	vector<vec3> leftTriangle;
	vector<vec3> rearTriangle;
	vector<vec3> rightTriangle;
	vector<vec3> frontTriangleBottom;
	vector<vec3> leftTriangleBottom;
	vector<vec3> rearTriangleBottom;
	vector<vec3> rightTriangleBottom;

	void createSphere(unsigned int levelOfDetail);
	void defineIndices(int levelOfDetail);
	
	void changeLevelOfDetail(int levelOfDetail);
	
};

#endif