#include "Cube.h"


Cube::Cube(void):GameObject(1.0f, true)
{
	this->quadPosition.push_back(vec3(-1.0f, -1.0f,  1.0f));
    this->quadPosition.push_back(vec3( 1.0f, -1.0f,  1.0f));
    this->quadPosition.push_back(vec3( 1.0f,  1.0f,  1.0f));
    this->quadPosition.push_back(vec3(-1.0f,  1.0f,  1.0f));
    this->quadPosition.push_back(vec3(-1.0f, -1.0f, -1.0f));
    this->quadPosition.push_back(vec3( 1.0f, -1.0f, -1.0f));
    this->quadPosition.push_back(vec3( 1.0f,  1.0f, -1.0f));
    this->quadPosition.push_back(vec3(-1.0f,  1.0f, -1.0f));

	this->wireframeMode = false;

	this->indexes.push_back(3);
    this->indexes.push_back(0);
    this->indexes.push_back(1);
    this->indexes.push_back(1);
    this->indexes.push_back(3);
    this->indexes.push_back(2);
    this->indexes.push_back(2);
    this->indexes.push_back(1);
    this->indexes.push_back(5);
    this->indexes.push_back(5);
    this->indexes.push_back(2);
    this->indexes.push_back(6);
    this->indexes.push_back(6);
    this->indexes.push_back(5);
    this->indexes.push_back(4);
    this->indexes.push_back(4);
    this->indexes.push_back(6);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(4);
    this->indexes.push_back(0);
    this->indexes.push_back(0);
    this->indexes.push_back(7);
    this->indexes.push_back(3);
    this->indexes.push_back(3);
    this->indexes.push_back(2);
    this->indexes.push_back(6);
    this->indexes.push_back(6);
    this->indexes.push_back(3);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(4);
    this->indexes.push_back(4);
    this->indexes.push_back(0);
    this->indexes.push_back(1);
	this->indexes.push_back(1);
	this->indexes.push_back(4);
	this->indexes.push_back(5);

	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	this->size = 1.0f;

	this->mass = MF.getVolumeOfACube(1.0f);
}

Cube::Cube(float posX, float posY, float posZ, float size):GameObject(size, true)
{
	size /= 2;

	this->quadPosition.push_back(vec3(-size + 0.0f, -size + 0.0f,  size + 0.0f));
    this->quadPosition.push_back(vec3( size + 0.0f, -size + 0.0f,  size + 0.0f));
    this->quadPosition.push_back(vec3( size + 0.0f,  size + 0.0f,  size + 0.0f));
    this->quadPosition.push_back(vec3(-size + 0.0f,  size + 0.0f,  size + 0.0f));
    this->quadPosition.push_back(vec3(-size + 0.0f, -size + 0.0f, -size + 0.0f));
    this->quadPosition.push_back(vec3( size + 0.0f, -size + 0.0f, -size + 0.0f));
    this->quadPosition.push_back(vec3( size + 0.0f,  size + 0.0f, -size + 0.0f));
    this->quadPosition.push_back(vec3(-size + 0.0f,  size + 0.0f, -size + 0.0f));

	this->wireframeMode = false;

	this->indexes.push_back(3);
    this->indexes.push_back(0);
    this->indexes.push_back(1);
    this->indexes.push_back(1);
    this->indexes.push_back(3);
    this->indexes.push_back(2);
    this->indexes.push_back(2);
    this->indexes.push_back(1);
    this->indexes.push_back(5);
    this->indexes.push_back(5);
    this->indexes.push_back(2);
    this->indexes.push_back(6);
    this->indexes.push_back(6);
    this->indexes.push_back(5);
    this->indexes.push_back(4);
    this->indexes.push_back(4);
    this->indexes.push_back(6);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(4);
    this->indexes.push_back(0);
    this->indexes.push_back(0);
    this->indexes.push_back(7);
    this->indexes.push_back(3);
    this->indexes.push_back(3);
    this->indexes.push_back(2);
    this->indexes.push_back(6);
    this->indexes.push_back(6);
    this->indexes.push_back(3);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(7);
    this->indexes.push_back(4);
    this->indexes.push_back(4);
    this->indexes.push_back(0);
    this->indexes.push_back(1);
	this->indexes.push_back(1);
	this->indexes.push_back(4);
	this->indexes.push_back(5);

	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	this->size = size;

	changePosition(posX, posY, posZ);

	this->mass = MF.getVolumeOfACube(size);
}

void Cube::setColor(float r, float g, float b, float a)
{
	this->color = vec4(r,g,b,a);
}

void Cube::setWireframeMode(bool active)
{
	this->wireframeMode = active;
}

void Cube::render(GLint *positionAttrib, GLint *colorUniform)
{
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &quadPosition.at(0).x);
	glUniform4f(*colorUniform,color.x, color.y, color.z, color.w);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_TRIANGLE_STRIP,this->indexes.size(),GL_UNSIGNED_SHORT,&this->indexes.at(0));

	if(this->wireframeMode)
	{
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1.0, 1.0 );
		glUniform4f(*colorUniform,0.f, 0.f, 0.f, 1.0f);
		glDrawElements(GL_LINE_LOOP,this->indexes.size(),GL_UNSIGNED_SHORT,&this->indexes.at(0));
	}
	

	if(this->displayAxesBool)
		displayAxes(positionAttrib,colorUniform);
}

void Cube::update(float elapsedTime)
{
	changePosition(-1 * (this->velocity.x * elapsedTime), -1 * (this->velocity.y * elapsedTime), -1 * (this->velocity.z * elapsedTime));
	vec3 velocity;

	velocity = MF.roundf(*getVelocity(), 1);

	if(velocity.x != 0.0f || velocity.z != 0.0f)
		this->rotate(velocity, elapsedTime);
}

void Cube::changePosition(float posX, float posY, float posZ)
{
	unsigned int i;

	this->centerOfObject.x += posX;
	this->centerOfObject.y += posY;
	this->centerOfObject.z += posZ;


	for(i = 0; i < 8; ++i)
	{
		this->quadPosition.at(i).x += posX;
		this->quadPosition.at(i).y += posY;
		this->quadPosition.at(i).z += posZ;
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
	
	this->components.getRigidBody()->updateRigidBody(vec3(posX,posY,posZ));
}

void Cube::rotate(vec3 velocity, float elapsedTime)
{
	float angleInclinaisonPlan = 0.0f;

	vec3 norm;
	vec3 normal;
	vec3 d;
	float angle;
	float speed;

	speed = ((velocity.Length())/getSize()) * elapsedTime;// / (90);

	//normal = this->getNormal();
	normal = vec3(0,1,0);

	if(normal != this->normal)
	{
		this->normal = normal;

		norm = MF.normalizeVector(normal);

		norm = vec3(norm.x * (float)sin((angleInclinaisonPlan/M_PI)*180), norm.y * (float)cos((angleInclinaisonPlan/M_PI)*180),0);
		this->norm = norm;
	}

	d.x = velocity.z;
	d.y = 0.0;//velocity.y;
	d.z = velocity.x * -1;

	d = MF.normalizeVector(d);

	d *= -1;
	
	angle = speed * 180.0f / M_PI;

	if(angle >= this->rotationPrecision)
	{

		vec4 q;
		vec4 qPrime;
		vec4 result;
	
		vec3 u;

		u = d;

		u = MF.normalizeVector(u);

		q = this->components.getQuaternion()->getQuaternion4f(angle, u);

		qPrime = this->components.getQuaternion()->getQuaternion4f(angle, u* (-1));

		int i;

		for(i = 0; i < 8; ++i)
		{
			this->quadPosition.at(i) -= this->centerOfObject;
			result = this->components.getQuaternion()->multiplyQuaternion4f(q, vec4(0,this->quadPosition.at(i).x, this->quadPosition.at(i).y, this->quadPosition.at(i).z));
			this->quadPosition.at(i) = this->components.getQuaternion()->multiplyQuaternion3f(result, qPrime);
			this->quadPosition.at(i) += this->centerOfObject;
		}

		// AXES

		for(i = 0; i < 2; ++i)
		{
			this->axeX.at(i) -= this->centerOfObject;
			result =  this->components.getQuaternion()->multiplyQuaternion4f(q, vec4(0,axeX.at(i).x, axeX.at(i).y, axeX.at(i).z));
			this->axeX.at(i) = this->components.getQuaternion()->multiplyQuaternion3f(result, qPrime);
			this->axeX.at(i) += this->centerOfObject;

			this->axeY.at(i) -= this->centerOfObject;
			result =  this->components.getQuaternion()->multiplyQuaternion4f(q, vec4(0,axeY.at(i).x, axeY.at(i).y, axeY.at(i).z));
			this->axeY.at(i) = this->components.getQuaternion()->multiplyQuaternion3f(result, qPrime);
			this->axeY.at(i) += this->centerOfObject;;

			this->axeZ.at(i) -= this->centerOfObject;
			result =  this->components.getQuaternion()->multiplyQuaternion4f(q, vec4(0,axeZ.at(i).x, axeZ.at(i).y, axeZ.at(i).z));
			this->axeZ.at(i) = this->components.getQuaternion()->multiplyQuaternion3f(result, qPrime);
			this->axeZ.at(i) += this->centerOfObject;
		}

		getComponents()->getRigidBody()->rotate(angle, d);
	}
}

void Cube::flipFaces(facesOrientation orientation)
{
	switch(orientation)
	{
	case INSIDE:
		this->indexes[0] = 0;
		this->indexes[1] = 3;
		this->indexes[2] = 2;
		this->indexes[3] = 2;
		this->indexes[4] = 0;
		this->indexes[5] = 1;
		this->indexes[6] = 1;
		this->indexes[7] = 2;
		this->indexes[8] = 6;
		this->indexes[9] = 6;
		this->indexes[10] = 1;
		this->indexes[11] = 5;
		this->indexes[12] = 5;
		this->indexes[13] = 6;
		this->indexes[14] = 7;
		this->indexes[15] = 7;
		this->indexes[16] = 5;
		this->indexes[17] = 4;
		this->indexes[18] = 4;
		this->indexes[19] = 7;
		this->indexes[20] = 3;
		this->indexes[21] = 3;
		this->indexes[22] = 4;
		this->indexes[23] = 0;

		this->indexes[24] = 0;
		this->indexes[25] = 1;
		this->indexes[26] = 4;
		this->indexes[27] = 4;
		this->indexes[28] = 5;
		this->indexes[29] = 1;
		this->indexes[30] = 1;
		this->indexes[31] = 1;
		this->indexes[32] = 2;
		this->indexes[33] = 2;
		this->indexes[34] = 6;
		this->indexes[35] = 7;
		this->indexes[36] = 7;
		this->indexes[37] = 2;
		this->indexes[38] = 3;
		break;
	case OUTSIDE:
		this->indexes[0] = 3;
		this->indexes[1] = 0;
		this->indexes[2] = 1;
		this->indexes[3] = 1;
		this->indexes[4] = 3;
		this->indexes[5] = 2;
		this->indexes[6] = 2;
		this->indexes[7] = 1;
		this->indexes[8] = 5;
		this->indexes[9] = 5;
		this->indexes[10] = 2;
		this->indexes[11] = 6;
		this->indexes[12] = 6;
		this->indexes[13] = 5;
		this->indexes[14] = 4;
		this->indexes[15] = 4;
		this->indexes[16] = 6;
		this->indexes[17] = 7;
		this->indexes[18] = 7;
		this->indexes[19] = 4;
		this->indexes[20] = 0;
		this->indexes[21] = 0;
		this->indexes[22] = 7;
		this->indexes[23] = 3;
		this->indexes[24] = 3;
		this->indexes[25] = 2;
		this->indexes[26] = 6;
		this->indexes[27] = 6;
		this->indexes[28] = 3;
		this->indexes[29] = 7;
		this->indexes[30] = 7;
		this->indexes[31] = 7;
		this->indexes[32] = 4;
		this->indexes[33] = 4;
		this->indexes[34] = 0;
		this->indexes[35] = 1;
		this->indexes[36] = 1;
		this->indexes[37] = 4;
		this->indexes[38] = 5;
		break;
	}
}

float Cube::getSize()
{
	return this->size;
}

vector<vec3>* Cube::getListOfVertices()
{
	return &this->quadPosition;
}

vector<GLushort>* Cube::getListOfIndexes()
{
	return &this->indexes;
}

Cube::~Cube(void)
{
}
