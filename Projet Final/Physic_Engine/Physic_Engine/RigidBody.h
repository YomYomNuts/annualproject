#ifndef __RIGIDBODY__
#define __RIGIDBODY__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "RigidBody_Sphere.h"
#include "RigidBody_Plane.h"
#include "RigidBody_Cube.h"
#include "RigidBody_Generic_Object.h"

enum typeRigidBody{RIGID_SPHERE, RIGID_CUBE, RIGID_PLANE, RIGID_GENERIC, RIGID_NONE};


class RigidBody
{
public:
	RigidBody(void);
	RigidBody(typeRigidBody type);
	//void getCollisions(int idOfCurrentObject, ObjectManager* objectManager, vec3* velocity);
	void activateRigidBody(bool status);
	void addRigidBody(typeRigidBody type,vector<vec3> verticesList, vector<GLushort> indexesList, vec3 position, bool alternateFacesAndNormals);
	void addRigidBody(typeRigidBody type, vec3 position, float size);
	void addRigidBody(typeRigidBody type, vec3 position, float width, float length);
	void renderRigidBody(GLint *positionAttrib, GLint *colorUniform);
	bool getRigidBodyStatus();
	void setDisplayRigidBody(bool display);
	bool getDisplayRigidBody();
	void updateRigidBody(vec3 position);
	vec3 getCenterOfObject();
	void calculateNormalsAndCenterOfGravities();
	RigidBody_Sphere* getRigidBodyObjectSphere();
	RigidBody_Plane* getRigidBodyObjectPlane();
	RigidBody_Cube* getRigidBodyObjectCube();
	RigidBody_Generic_Object* getRigidBodyObjectGeneric();
	void rotate(float angle, vec3 u);
	typeRigidBody getType();
	unsigned int getLowestVertex();
	vec3 getPosLowestVertex();
	vec4 getCollidingFace();
	vector<vec3>* getListIndexesFaces();
	vector<vec3>* getListOfCenterOfGravities();
	vector<vec3>* getListOfNormals();
	vector<GLushort>* getListOfIndexes();
	vector<vec3>* getListOfVertices();
	void setIsStatic(bool movable);
	bool getIsStatic();
	float getSize();
	void setSize(float size);
	void changeVerticesList(vector<vec3> verticesList);
	void changeIndexesList(vector<GLushort> indexesList);
	~RigidBody(void);

private:
	typeRigidBody type;
	bool isActivated;
	bool displayRigidbody;

	RigidBody_Sphere rigidSphere;
	RigidBody_Plane rigidPlane;
	RigidBody_Cube rigidCube;
	RigidBody_Generic_Object rigidGeneric;

	//bool getCollisionBetweenSphereAndSphere(GameObject* object);
	//bool getCollisionBetweenSphereAndPlane(Sphere* currentObject, Plane* plane, vec3* velocity);
};

#endif