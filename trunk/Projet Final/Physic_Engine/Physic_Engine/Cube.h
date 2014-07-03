#ifndef __CUBE__
#define __CUBE__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "GameObject.h"

using namespace std;

enum facesOrientation{INSIDE, OUTSIDE};

class Cube : public GameObject
{
public:
	Cube(void);
	Cube(float posX, float posY, float posZ, float size);
	void setColor(float r, float g, float b, float a);
	void setWireframeMode(bool active);
	void flipFaces(facesOrientation orientation);

	void update(float elapsedTime);
	void render(GLint *positionAttrib, GLint *colorUniform);
	void changePosition(float X, float Y, float Z);
	void rotate(vec3 velocity, float elapsedTime);
	float getSize();
	void calculateNormalsAndCenterOfGravities();
	vector<vec3>* getListOfNormals();
	vector<vec3>* getListOfCenterOfGravities();
	vector<vec3>* getListOfVertices();
	vector<GLushort>* getListOfIndexes();
	vector<vec3>* getListIndexesFaces();
	void clearLists();
	void deleteLists();

	/*void calculateNormalsAndCenterOfGravities();
	vector<vec3>* getListOfNormals();
	vector<vec3>* getListOfCenterOfGravities();
	vector<vec3>* getListOfVertices();
	vector<GLushort>* getListOfIndexes();
	vector<vec3>* getListIndexesFaces();*/
	
	~Cube(void);

private:
	bool wireframeMode;
	
	vec4 color;
	vector<GLushort> indexes;
	vector<vec3> quadPosition;

	float size;
};



#endif