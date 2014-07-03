#include "GameObject.h"


GameObject::GameObject()
{
	this->id = this->count++;
	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	this->displayAxesBool = false;
	this->centerOfObject = vec3(0.0f, 0.0f, 0.0f);
	this->wireframeMode = false;
	this->useTexture = 0.0f;
	this->alternateFacesAndNormals = false;


	this->direction = vec3(0.0f, 0.0f, 0.0f);
	this->velocity = vec3(0.0f, 0.0F,0.0f);

	this->axeX.push_back(centerOfObject);
	this->axeX.push_back(vec3(centerOfObject.x * 1.5f, centerOfObject.y, centerOfObject.z));

	this->axeY.push_back(centerOfObject);
	this->axeY.push_back(vec3(centerOfObject.x, centerOfObject.y * 1.5f, centerOfObject.z));

	this->axeZ.push_back(centerOfObject);
	this->axeZ.push_back(vec3(centerOfObject.x, centerOfObject.y, centerOfObject.z * 1.5f));

	this->norm = vec3(0.0f, 0.0f, 0.0f);
	this->normal = vec3(0.0f, 0.0f, 0.0f);
	this->mass = 0;
	this->renderObject = true;
	this->rotationPrecision = 0.01f;

	this->listGameObjectAround = new vector<GameObject*>();
}

GameObject::GameObject(float axeSize, bool alternateFacesAndNormals)
{
	this->id = this->count++;
	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);
	this->displayAxesBool = false;
	this->centerOfObject = vec3(0.0f, 0.0f, 0.0f);
	this->wireframeMode = false;
	this->useTexture = 0.0f;

	this->direction = vec3(0.0f, 0.0f, 0.0f);
	this->velocity = vec3(0.0f, 0.0F,0.0f);

	this->axeX.push_back(centerOfObject);
	this->axeX.push_back(vec3(centerOfObject.x + (axeSize * 1.5f), centerOfObject.y, centerOfObject.z));

	this->axeY.push_back(centerOfObject);
	this->axeY.push_back(vec3(centerOfObject.x, centerOfObject.y  + (axeSize * 1.5f), centerOfObject.z));

	this->axeZ.push_back(centerOfObject);
	this->axeZ.push_back(vec3(centerOfObject.x, centerOfObject.y, centerOfObject.z  + (axeSize * 1.5f)));
	
	this->norm = vec3(0.0f, 0.0f, 0.0f);
	this->normal = vec3(0.0f, 0.0f, 0.0f);
	this->mass = 0;
	this->renderObject = true;
	this->rotationPrecision = 0.01f;

	this->alternateFacesAndNormals = alternateFacesAndNormals;
	this->listGameObjectAround = new vector<GameObject*>();
}

void GameObject::setColor(float r, float g, float b, float a)
{
	this->color = vec4(r,g,b,a);
}

void GameObject::setWireframeMode(bool active)
{
	this->wireframeMode = active;
}

void GameObject::setUseTexture(bool use)
{
	this->useTexture = (use ? 1.0f : 0.0f);
}

void GameObject::setDisplayAxes(bool status)
{
	this->displayAxesBool = status;
}

void GameObject::displayAxes(GLint *positionAttrib, GLint *colorUniform)
{
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

vec3 GameObject::getCenterOfObject()
{
	return this->centerOfObject;
}

unsigned int GameObject::getId()
{
	return this->id;
}

vector<GameObject*>* GameObject::getObjectsAround()
{
	return this->listGameObjectAround;
}

Components* GameObject::getComponents()
{
	return &this->components;
}

vec3 GameObject::getAxesOrientation()
{
	vec3 vectX;
	vec3 vectY;
	vec3 vectZ;

	vec3 vectXOrigin;
	vec3 vectYOrigin;
	vec3 vectZOrigin;

	vectXOrigin = vec3(1,0,0);
	vectYOrigin = vec3(0,1,0);
	vectZOrigin = vec3(0,0,1);

	vectX = MF.getVector(this->axeX.at(0), this->axeX.at(1));
	vectY = MF.getVector(this->axeY.at(0), this->axeY.at(1));
	vectZ = MF.getVector(this->axeZ.at(0), this->axeZ.at(1));

	vec3 vector1;
	vec3 vector2;
	vec3 cp;
	//vector1 = MF.getVector(this->verticesList.at(0), this->verticesList.at(1));
	//vector2 = MF.getVector(this->verticesList.at(0), this->verticesList.at(2));
	cp = MF.crossProduct(vectX, vectZ);

	cout << "normal: x: " << MF.normalizeVector(cp).x << " y: " << MF.normalizeVector(cp).y << " z: " << MF.normalizeVector(cp).z << endl;;


	return vec3(MF.getAngleBetweenVectors(vectX, vectXOrigin), MF.getAngleBetweenVectors(vectY, vectYOrigin), MF.getAngleBetweenVectors(vectZ, vectZOrigin));
}

vec3 GameObject::getNormal()
{
	vec3 vectX;
	vec3 vectZ;
	vec3 cp;

	vectX = MF.getVector(this->axeX.at(0), this->axeX.at(1));
	vectZ = MF.getVector(this->axeZ.at(0), this->axeZ.at(1));

	cp = MF.crossProduct(vectX, vectZ);

	return MF.normalizeVector(MF.inverseNormal(cp));
}

float GameObject:: getAxeSize()
{
	float x;
	float y;
	float z;

	x = pow(this->axeX.at(1).x - this->axeX.at(0).x,2);
	y = pow(this->axeY.at(1).x - this->axeY.at(0).x,2);
	z = pow(this->axeZ.at(1).x - this->axeZ.at(0).x,2);

	return sqrt(x + y + z);
}

void GameObject::increaseVelocity(vec3 velocity)
{
	this->velocity += velocity;
}

void GameObject::setVelocity(vec3 velocity)
{
	this->velocity = velocity;
}

vec3* GameObject::getVelocity()
{
	return &this->velocity;
}

void GameObject::addGameObjectAround(GameObject* object)
{
	unsigned int i;
	bool add;

	add = true;

	for(i = 0; i < this->listGameObjectAround->size(); ++i)
	{
		if(this->listGameObjectAround->at(i)->getId() == object->getId())
		{
			add = false;
			break;
		}
	}

	(add)?this->listGameObjectAround->push_back(object):0;
}

vector<GameObject*>* GameObject::getGameObjectAround()
{
	return this->listGameObjectAround;
}

void GameObject::update(float elapsedTime)
{
}

float GameObject::getMass()
{
	return this->mass;
}

void GameObject::setMass(float mass)
{
	this->mass = mass;
}

void GameObject::setRotationPrecision(float rotationPrecision)
{
	this->rotationPrecision = rotationPrecision;
}

float GameObject::getRotationPrecision()
{
	return this->rotationPrecision;
}

void GameObject::setAlternateFacesAndNormals(bool alternateFacesAndNormals)
{
	this->alternateFacesAndNormals = alternateFacesAndNormals;
}

bool GameObject::getAlternateFacesAndNormals()
{
	return this->alternateFacesAndNormals;
}

GameObject::~GameObject(void)
{
}
