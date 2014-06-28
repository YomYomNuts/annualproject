#include "RigidBody.h"


RigidBody::RigidBody(void)
{
	this->type = RIGID_NONE;
	this->isActivated = false;
	this->displayRigidbody = false;
}

RigidBody::RigidBody(typeRigidBody type)
{
	this->type = type;
	this->isActivated = false;
	this->displayRigidbody = false;
}

void RigidBody::addRigidBody(typeRigidBody type,vector<vec3> verticesList, vector<GLushort> indexesList, vec3 position, bool alternateFacesAndNormals)
{
	this->rigidSphere = RigidBody_Sphere();
	this->rigidPlane = RigidBody_Plane();
	this->rigidCube = RigidBody_Cube();
	this->rigidGeneric = RigidBody_Generic_Object();
	
	if(type == RIGID_GENERIC)
	{
		this->type = type;
		this->rigidGeneric = RigidBody_Generic_Object(verticesList, indexesList, position.x, position.y, position.z);
		this->rigidGeneric.setAlternateFacesAndNormals(alternateFacesAndNormals);
	}
}

void RigidBody::addRigidBody(typeRigidBody type, vec3 position, float size)
{
	this->rigidSphere = RigidBody_Sphere();
	this->rigidPlane = RigidBody_Plane();
	this->rigidCube = RigidBody_Cube();
	this->rigidGeneric = RigidBody_Generic_Object();

	if(type == RIGID_SPHERE)
	{
		this->type = type;
		this->rigidSphere = RigidBody_Sphere(position, size);
	}
	else if(type == RIGID_CUBE)
	{
		this->type = type;
		this->rigidCube = RigidBody_Cube(position, size);
	}
}

void RigidBody::addRigidBody(typeRigidBody type, vec3 position, float width, float length)
{
	this->rigidSphere = RigidBody_Sphere();
	this->rigidPlane = RigidBody_Plane();
	this->rigidCube = RigidBody_Cube();

	if(type == RIGID_PLANE)
	{
		this->type = type;
		this->rigidPlane = RigidBody_Plane(position, width, length);
	}
}

void RigidBody::updateRigidBody(vec3 position)
{
	switch(this->type)
	{
		case RIGID_SPHERE:
			this->rigidSphere.changePosition(position.x, position.y, position.z);
			break;
		case RIGID_CUBE:
			this->rigidCube.changePosition(position.x, position.y, position.z);
			break;
		case RIGID_GENERIC:
			this->rigidGeneric.changePosition(position.x, position.y, position.z);
			break;
	}
}

typeRigidBody RigidBody::getType()
{
	return this->type;
}

void RigidBody::renderRigidBody(GLint *positionAttrib, GLint *colorUniform)
{
	if(!this->rigidSphere.isNULL)
	{
		if(this->displayRigidbody)
			this->rigidSphere.render(positionAttrib, colorUniform);
	}
	else if(!this->rigidPlane.isNULL)
	{
		if(this->displayRigidbody)
			this->rigidPlane.render(positionAttrib, colorUniform);
	}
	else if(!this->rigidCube.isNULL)
	{
		if(this->displayRigidbody)
			this->rigidCube.render(positionAttrib, colorUniform);
	}
	else if(!this->rigidGeneric.isNULL)
	{
		if(this->displayRigidbody)
			this->rigidGeneric.render(positionAttrib, colorUniform);
	}
}

void RigidBody::setDisplayRigidBody(bool display)
{
	this->displayRigidbody = display;
}

vec3 RigidBody::getCenterOfObject()
{
	switch (this->type)
	{
		case RIGID_SPHERE:
			return this->rigidSphere.getCenterOfObject();
			break;
		case RIGID_PLANE:
			return this->rigidPlane.getCenterOfObject();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getCenterOfObject();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getCenterOfObject();
			break;
		default:
			return vec3(0.0f, 0.0f, 0.0f);

	}
}

bool RigidBody::getDisplayRigidBody()
{
	return this->displayRigidbody;
}

RigidBody_Sphere* RigidBody::getRigidBodyObjectSphere()
{
	return &this->rigidSphere;
}

RigidBody_Plane* RigidBody::getRigidBodyObjectPlane()
{
	return &this->rigidPlane;
}

RigidBody_Cube* RigidBody::getRigidBodyObjectCube()
{
	return &this->rigidCube;
}

RigidBody_Generic_Object* RigidBody::getRigidBodyObjectGeneric()
{
	return &this->rigidGeneric;
}

void RigidBody::activateRigidBody(bool status)
{
	this->isActivated = status;
}

bool RigidBody::getRigidBodyStatus()
{
	return this->isActivated;
}

void RigidBody::rotate(float angle, vec3 u)
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			this->rigidPlane.rotate(angle, u);
			break;
		case RIGID_SPHERE:
			this->rigidSphere.rotate(angle, u);
			break;
		case RIGID_CUBE:
			this->rigidCube.rotate(angle, u);
			break;
		case RIGID_GENERIC:
			this->rigidGeneric.rotate(angle, u);
			break;
	}
}

void RigidBody::calculateNormalsAndCenterOfGravities()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			this->rigidCube.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_GENERIC:
			this->rigidGeneric.calculateNormalsAndCenterOfGravities();
			break;
	}
}

vec3 RigidBody::getPosLowestVertex()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getPosLowestVertex();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getPosLowestVertex();
			break;
		default:
			return vec3(0.0f, 0.0f, 0.0f);
			break;
	}

}

unsigned int RigidBody::getLowestVertex()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getLowestVertex();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getLowestVertex();
			break;
		default:
			return -1;
			break;
	}
}

vec4 RigidBody::getCollidingFace()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getCollidingFace();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getCollidingFace();
			break;
		default:
			return vec4(0.0f, 0.0f, 0.0f, 0.0f);
			break;
	}
}

vector<vec3>* RigidBody::getListIndexesFaces()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getListIndexesFaces();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getListIndexesFaces();
			break;
		default:
			return NULL;
			break;
	}
}

vector<vec3>* RigidBody::getListOfCenterOfGravities()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getListOfCenterOfGravities();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getListOfCenterOfGravities();
			break;
		default:
			return NULL;
			break;
	}
}

vector<vec3>* RigidBody::getListOfNormals()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getListOfNormals();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getListOfNormals();
			break;
		default:
			return NULL;
			break;
	}
}

vector<GLushort>* RigidBody::getListOfIndexes()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getListOfIndexes();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getListOfIndexes();
			break;
		default:
			return NULL;
			break;
	}
}

vector<vec3>* RigidBody::getListOfVertices()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			//this->rigidPlane.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_SPHERE:
			//this->rigidSphere.calculateNormalsAndCenterOfGravities();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getListOfVertices();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getListOfVertices();
			break;
		default:
			return NULL;
			break;
	}
}

void RigidBody::setIsStatic(bool movable)
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			this->rigidPlane.setIsStatic(movable);
			break;
		case RIGID_SPHERE:
			this->rigidSphere.setIsStatic(movable);
			break;
		case RIGID_CUBE:
			this->rigidCube.setIsStatic(movable);
			break;
		case RIGID_GENERIC:
			this->rigidGeneric.setIsStatic(movable);
			break;
	}
}

bool RigidBody::getIsStatic()
{
	switch(this->type)
	{
		case RIGID_PLANE: 
			return this->rigidPlane.getIsStatic();
			break;
		case RIGID_SPHERE:
			return this->rigidSphere.getIsStatic();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getIsStatic();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getIsStatic();
			break;
		default:
			return false;
			break;
	}
}

float RigidBody::getSize()
{
	switch(this->type)
	{
		/*case RIGID_PLANE: 
			return this->rigidPlane.getIsStatic();
			break;*/
		case RIGID_SPHERE:
			return this->rigidSphere.getRadius();
			break;
		case RIGID_CUBE:
			return this->rigidCube.getSize();
			break;
		case RIGID_GENERIC:
			return this->rigidGeneric.getSize();
			break;
		default:
			return -1.0f;
			break;
	}
}

void RigidBody::setSize(float size)
{
	switch(this->type)
	{
		/*case RIGID_PLANE: 
			return this->rigidPlane.getIsStatic();
			break;*/
		case RIGID_SPHERE:
			//return this->rigidSphere.getRadius();
			//break;
		case RIGID_CUBE:
			//return this->rigidCube.getSize();
			//break;
		case RIGID_GENERIC:
			return this->rigidGeneric.setSize(size);
			break;
	}
}

void RigidBody::changeVerticesList(vector<vec3> verticesList)
{
	switch(this->type)
	{
		case RIGID_GENERIC:
			this->rigidGeneric.changeVerticesList(verticesList);
			break;
	}
}

void RigidBody::changeIndexesList(vector<GLushort> indexesList)
{
	switch(this->type)
	{
		case RIGID_GENERIC:
			this->rigidGeneric.changeIndexesList(indexesList);
			break;
	}
}

RigidBody::~RigidBody(void)
{
}
