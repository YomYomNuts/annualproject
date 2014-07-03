#include "Plane.h"


Plane::Plane(void):GameObject(1.0f, true)
{
	this->verticesList.push_back(vec3(-10.0f, 0.0f, -10.0f));
	this->verticesList.push_back(vec3(-10.0f, 0.0f, 10.0f));
	this->verticesList.push_back(vec3(10.0f, 0.0f, 10.0f));
	this->verticesList.push_back(vec3(10.0f, 0.0f, -10.0f));

	this->indices.push_back(0);
	this->indices.push_back(1);
	this->indices.push_back(2);
	this->indices.push_back(2);
	this->indices.push_back(0);
	this->indices.push_back(3);
	this->indices.push_back(2);

	this->color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

Plane::Plane(vec3 upperRight, vec3 upperLeft, vec3 lowerRight,vec3 lowerLeft)
{
	this->verticesList.push_back(lowerLeft);
	this->verticesList.push_back(lowerRight);
	this->verticesList.push_back(upperLeft);
	this->verticesList.push_back(upperRight);
	
	this->quad[0] = lowerLeft;
	this->quad[1] = lowerRight;
	this->quad[2] = upperLeft;
	this->quad[3] = upperRight;

	this->indices.push_back(2);
	this->indices.push_back(3);
	this->indices.push_back(1);
	this->indices.push_back(1);
	this->indices.push_back(2);
	this->indices.push_back(0);

	this->color = vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

void Plane::render(GLint *positionAttrib, GLint *colorUniform, GLint * texture_uniform)
{
	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->verticesList.at(0).x);
	glUniform4f(*colorUniform,color.x, color.y, color.z, color.w);
	glUniform1f(*texture_uniform, this->useTexture);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_TRIANGLE_STRIP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));
	

	if(wireframeMode)
	{
		glEnable( GL_POLYGON_OFFSET_FILL );
		glPolygonOffset( 1.0, 1.0 );
		glUniform4f(*colorUniform,0.f, 0.f, 0.f, 1.0f);
		glUniform1f(*texture_uniform, 0.0f);
		glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));
	}

	if(displayAxesBool)
		displayAxes(positionAttrib, colorUniform);
}

vector<vec3> Plane::getCoordinates()
{
	return this->verticesList;
}

void Plane::update(float elapsedTime)
{
}

void Plane::changePosition(float X, float Y, float Z)
{
}

void Plane::rotate(vec3 velocity, float elapsedTime)
{
}

vector<vec3>* Plane::getListOfVertices()
{
	return &this->verticesList;
}

vector<GLushort>* Plane::getListOfIndexes()
{
	return &this->indices;
}
void Plane::clearLists()
{
	this->verticesList.clear();
	this->indices.clear();
}

void Plane::deleteLists()
{
}


Plane::~Plane(void)
{
}
