#ifndef __OBJECTMANAGER__
#define __OBJECTMANAGER__

#include "../EsgiGL/Common/EsgiUtils.h"
#include <iostream>
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include "MathFunctions.h"
#include "GameObject.h"
#include "RigidBody_Cube.h"
#include "Common/EsgiTimer.h"


class ObjectManager
{
public:
	ObjectManager(void);
	int getNumberOfObject();
	void addObject(GameObject* object);
	void update(float elapsedTime);
	void render(GLint *positionAttrib, GLint *colorUniform);
	void setGravity(float gravity);
	GameObject* getGameObjectById(int id);
	
	~ObjectManager(void);

private:
	vector<GameObject*> listOfObject;

	float getDistanceSphereToPlane(GameObject* sphere, GameObject* plane, float elapsedTime);
	float getDistanceSphereToSphere(GameObject* sphere1, GameObject* sphere2, float elapsedTime);
	float getDistanceCubeToCube(GameObject* cube1, GameObject* cube2, float elapsedTime);
	float getDistanceCubeToPlane(GameObject* cube, GameObject* plane, float elapsedTime);
	float getDistanceBetweenGameObjects(vec3* posGO1, vec3* posGO2, vec3* velocityGO1, vec3* normalGO2, float elapsedTime);
	bool isVertexInsideObject(vector<vec3>* objectListOfNormals, vector<vec3>* objectListOfCOG, vector<GLushort>* objectListOfIndexes,
										 vector<vec3>* objectListIndexesOfFaces, vector<vec3>* objectListOfVertices,
										 vec3 vertexPos, vec3* velocityOfCurrentObject, float elapsedTime);
	//vec4 isSegmentInsideObject(vector<vec3>* objectListOfNormals, vector<vec3>* objectListOfCOG, vector<GLushort>* objectListOfIndexes,
	//									 vector<vec3>* objectListIndexesOfFaces, vector<vec3>* objectListOfVertices,
	//									 vec3 pointA, vec3 pointB, vec3* velocityOfCurrentObject, float elapsedTime);
	
	vec4 isSegmentInsideObject(RigidBody* otherObjectRigidBody, vec3 pointA, vec3 pointB, vec3* velocityOfCurrentObject, float elapsedTime);
	void defineGameObjectAround(float elapsedTime);
	void reorientObject(GameObject* object, float elapsedTime);
	void updateCollisionWithSphere(GameObject* currentObject, float elapsedTime);
	void updateCollisionWithCube(GameObject* currentObject, float elapsedTime);
	void updateCollisionWithGeneric(GameObject* currentObject, float elapsedTime);
	
	MathFunctions MF;

};

#endif