#include "RigidBody_Plane.h"


RigidBody_Plane::RigidBody_Plane(void)
{
	this->isNULL = true;
	this->isStatic = false;
}

RigidBody_Plane::RigidBody_Plane(vec3 position, float width, float length)
{
	this->isNULL = false;

	this->centerOfObject = vec3(0.0f, 0.0f, 0.0f);

	this->verticesList.push_back(vec3(- (width/2), 0.0f, - (length/2)));
	this->verticesList.push_back(vec3(- (width/2), 0.0f, (length/2)));
	this->verticesList.push_back(vec3((width/2), 0.0f, (length/2)));
	this->verticesList.push_back(vec3((width/2), 0.0F, - (length/2)));

	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(2);
	this->indices.push_back(2);
	this->indices.push_back(0);
	this->indices.push_back(3);
	this->indices.push_back(2);

	this->axeX.push_back(centerOfObject);
	this->axeX.push_back(vec3(centerOfObject.x + 1.5f, centerOfObject.y, centerOfObject.z));

	this->axeY.push_back(centerOfObject);
	this->axeY.push_back(vec3(centerOfObject.x, centerOfObject.y  + 1.5f, centerOfObject.z));

	this->axeZ.push_back(centerOfObject);
	this->axeZ.push_back(vec3(centerOfObject.x, centerOfObject.y, centerOfObject.z  + 1.5f));

	changePosition(position.x, position.y, position.z);
	this->isStatic = false;
}

void RigidBody_Plane::changePosition(float posX, float posY, float posZ)
{
	unsigned int i;

	this->centerOfObject.x += posX;
	this->centerOfObject.y += posY;
	this->centerOfObject.z += posZ;

	for(i = 0; i < this->verticesList.size(); ++i)
	{
		this->verticesList.at(i).x += posX;
		this->verticesList.at(i).y += posY;
		this->verticesList.at(i).z += posZ;
	}

	// AXES

	for(i = 0; i < this->axeX.size(); ++i)
	{
		this->axeX.at(i).x += posX;
		this->axeX.at(i).y += posY;
		this->axeX.at(i).z += posZ;

		this->axeY.at(i).x += posX;
		this->axeY.at(i).y += posY;
		this->axeY.at(i).z += posZ;

		this->axeZ.at(i).x += posX;
		this->axeZ.at(i).y += posY;
		this->axeZ.at(i).z += posZ;
	}
	
}

void RigidBody_Plane::render(GLint *positionAttrib, GLint *colorUniform)
{
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->verticesList.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	// X RED
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->axeX.at(0).x);
	glUniform4f(*colorUniform,1.0f, 0.0f, 0.0f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawArrays(GL_LINES,0,2);

	// Y GREEN
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->axeY.at(0).x);
	glUniform4f(*colorUniform,0.0f, 1.0f, 0.0f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawArrays(GL_LINES,0,2);

	// Z BLUE
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->axeZ.at(0).x);
	glUniform4f(*colorUniform,0.0f, 0.0f, 1.0f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawArrays(GL_LINES,0,2);
}

vector<vec3> RigidBody_Plane::getCoordinates()
{
	return this->verticesList;
}

vec3 RigidBody_Plane::getNormal()
{
	vec3 vector1;
	vec3 vector2;
	vec3 cp;
	
	vector1 = MF.getVector(this->verticesList.at(0), this->verticesList.at(1));
	vector2 = MF.getVector(this->verticesList.at(0), this->verticesList.at(2));

	//cp = MF.getNormalFrom2Vecs(vector1, vector2);
	
	cp = MF.getNormalFrom2Vecs(this->axeX.at(0), this->axeX.at(1), this->axeZ.at(0), this->axeZ.at(1));
	return cp;
	//cp = MF.crossProduct(vector1, vector2);

	//return MF.normalizeVector(cp);
}

void RigidBody_Plane::rotate(float angle, vec3 u)
{
	vec4 q;
	vec4 qPrime;
	vec4 result;

	u = MF.normalizeVector(u);

	q = this->quaternion.getQuaternion4f(angle, u);

	qPrime = this->quaternion.getQuaternion4f(angle, u* (-1));

	unsigned int i;

	for(i = 0; i < this->verticesList.size(); ++i)
	{
		this->verticesList.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->verticesList.at(i).x, this->verticesList.at(i).y, this->verticesList.at(i).z));
		this->verticesList.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->verticesList.at(i) += this->centerOfObject;
	}

	// AXES

	for(i = 0; i < 2; ++i)
	{
		this->axeX.at(i) -= this->centerOfObject;
		result =  this->quaternion.multiplyQuaternion4f(q, vec4(0,axeX.at(i).x, axeX.at(i).y, axeX.at(i).z));
		this->axeX.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->axeX.at(i) += this->centerOfObject;

		this->axeY.at(i) -= this->centerOfObject;
		result =  this->quaternion.multiplyQuaternion4f(q, vec4(0,axeY.at(i).x, axeY.at(i).y, axeY.at(i).z));
		this->axeY.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->axeY.at(i) += this->centerOfObject;;

		this->axeZ.at(i) -= this->centerOfObject;
		result =  this->quaternion.multiplyQuaternion4f(q, vec4(0,axeZ.at(i).x, axeZ.at(i).y, axeZ.at(i).z));
		this->axeZ.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->axeZ.at(i) += this->centerOfObject;
	}

}

vec3 RigidBody_Plane::getCenterOfObject()
{
	return this->centerOfObject;
}

void RigidBody_Plane::setIsStatic(bool movable)
{
	this->isStatic = movable;
}

bool RigidBody_Plane::getIsStatic()
{
	return this->isStatic;
}

RigidBody_Plane::~RigidBody_Plane(void)
{
}
