#ifndef __GAMEOBJECT__
#define __GAMEOBJECT__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "Components.h"
#include "MathFunctions.h"

using namespace std;

class GameObject
{
public:

	GameObject();
	GameObject(float sizeAxe, bool alternateFacesAndNormals);
	void setColor(float r, float g, float b, float a);
	void setWireframeMode(bool status);
	void setDisplayAxes(bool status);
	vec3 getCenterOfObject();
	unsigned int getId();
	vector<GameObject*>* getObjectsAround();
	Components* getComponents();
	vec3 getAxesOrientation();
	vec3 getNormal();
	void increaseVelocity(vec3 velocity);
	void setVelocity(vec3 velocity);
	vec3* getVelocity();
	void addGameObjectAround(GameObject* object);
	vector<GameObject*>* getGameObjectAround();
	float getAxeSize();
	float getMass();
	void setMass(float mass);
	void setRenderObject(bool display) { this->renderObject = display; };
	bool getRenderObject() {return this->renderObject; };
	void setRotationPrecision(float rotationPrecision);
	float getRotationPrecision();
	void setAlternateFacesAndNormals(bool aternateFacesAndNormals);
	bool getAlternateFacesAndNormals();

	//VIRTUAL
	void virtual update(float elapsedTime);
	void virtual render(GLint *positionAttrib, GLint *colorUniform) = 0;
	void virtual changePosition(float X, float Y, float Z) = 0;
	void virtual rotate(vec3 velocity, float elapsedTime) = 0;
	void virtual clearLists() = 0;
	void virtual deleteLists() = 0;
	
	~GameObject(void);
	
protected:
	static int count;
	int id;
	vec4 color;
	bool displayAxesBool;
	bool wireframeMode;
	vec3 centerOfObject;
	Components components;

	vec3 direction;
	vector<GameObject*>* listGameObjectAround;
	vector<vec3> axeX;
	vector<vec3> axeY;
	vector<vec3> axeZ;
	vec3 velocity;
	vec3 normal;
	vec3 norm;
	float mass;
	bool renderObject;
	float rotationPrecision;
	bool alternateFacesAndNormals;

	MathFunctions MF;

	void displayAxes(GLint *positionAttrib, GLint *colorUniform);
};

#endif