#include "RigidBody_Generic_Object.h"
#include "RigidBody_Generic_Object.h"


RigidBody_Generic_Object::RigidBody_Generic_Object(void)
{
	this->isNULL = true;
	this->isStatic = false;
}

RigidBody_Generic_Object::RigidBody_Generic_Object(vector<vec3> verticesList, vector<GLushort> indexesList, float posX, float posY, float posZ)
{
	this->verticesList = verticesList;
	this->indices = indexesList;

	this->isNULL = false;

	vec3 highestPoint = verticesList.at(0);
	vec3 lowestPoint = verticesList.at(0);
	vec3 leftPoint = verticesList.at(0);
	vec3 rightPoint = verticesList.at(0);
	vec3 frontPoint = verticesList.at(0);
	vec3 backPoint = verticesList.at(0);

	for(int i = 1; i < this->verticesList.size(); ++i)
	{
		if(this->verticesList.at(i).y > highestPoint.y)
			highestPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).y < lowestPoint.y)
			lowestPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).x > rightPoint.x)
			rightPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).x < leftPoint.x)
			leftPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).z > frontPoint.z)
			frontPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).z < backPoint.z)
			backPoint = this->verticesList.at(i);
	}

	this->centerOfObject = vec3((rightPoint.x + leftPoint.x)/2.0f, (highestPoint.y + lowestPoint.y)/2.0f, (frontPoint.z + backPoint.z)/2.0f);
	
	this->size = MF.distanceBetween2points(highestPoint, centerOfObject);

	this->axeX.push_back(centerOfObject);
	this->axeX.push_back(vec3(centerOfObject.x + size * 1.3f, centerOfObject.y, centerOfObject.z));

	this->axeY.push_back(centerOfObject);
	this->axeY.push_back(vec3(centerOfObject.x, centerOfObject.y  + size * 1.3f, centerOfObject.z));

	this->axeZ.push_back(centerOfObject);
	this->axeZ.push_back(vec3(centerOfObject.x, centerOfObject.y, centerOfObject.z  + size * 1.3f));
	this->isStatic = false;

	//this->listIndexesWireframe;

	//changePosition(posX, posY, posZ);
}

void RigidBody_Generic_Object::changePosition(float posX, float posY, float posZ)
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

void RigidBody_Generic_Object::render(GLint *positionAttrib, GLint *colorUniform)
{
	if(this->listIndexesWireframe.size() == 0)
	{
		glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->verticesList.at(0).x);
		glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
		glEnableVertexAttribArray(*positionAttrib);
		glDrawElements(GL_LINES, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));
	}
	else
	{
		glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->verticesList.at(0).x);
		glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
		glEnableVertexAttribArray(*positionAttrib);
		glDrawElements(GL_LINES, this->listIndexesWireframe.size(),GL_UNSIGNED_SHORT,&this->listIndexesWireframe.at(0));
	}

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

void RigidBody_Generic_Object::rotate(float angle, vec3 u)
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

vector<vec3>* RigidBody_Generic_Object::getListOfVertices()
{
	return &this->verticesList;
}

vec3 RigidBody_Generic_Object::getCenterOfObject()
{
	return this->centerOfObject;
}

void RigidBody_Generic_Object::calculateNormalsAndCenterOfGravities()
{
	vec3 normal;
	bool inverse = false;
	
	unsigned int i;

	vec3 point0;
	vec3 point1;
	vec3 point2;
	vec3 face[3];
	

	this->listOfNormals.clear();
	this->listOfCenterOfGravities.clear();
	this->listIndexesFaces.clear();

	if(alternateFacesAndNormals)
	{
		for(i = 0; i < this->indices.size(); i += 3)
		{
			point0 = this->verticesList.at(this->indices[i]);
			point1 = this->verticesList.at(this->indices[i+1]);
			point2 = this->verticesList.at(this->indices[i+2]);

			normal = MF.getNormalFrom2Vecs(point1, point0, point1, point2);
		
			if(!MF.isTriangleFlat(point1, point0, point1, point2))
			{
				if(inverse)
				{
					normal = this->MF.inverseNormal(normal);
					inverse = false;
				}
				else
				{
					inverse = true;
				}

				this->listOfNormals.push_back(normal);

				this->listOfCenterOfGravities.push_back(vec3(
							(point0.x + point1.x + point2.x) / 3,
							(point0.y + point1.y + point2.y) / 3,
							(point0.z + point1.z + point2.z) / 3));
			
				this->listIndexesFaces.push_back(vec3(i, i+1, i+2));
			}
			else
			{
				inverse=(inverse?false:true);
			}

		}
	}
	else
	{
		for(i = 0; i < this->indices.size(); i += 3)
		{
			point0 = this->verticesList.at(this->indices[i]);
			point1 = this->verticesList.at(this->indices[i+1]);
			point2 = this->verticesList.at(this->indices[i+2]);

			normal = MF.getNormalFrom2Vecs(point1, point0, point1, point2);
		
			if(!MF.isTriangleFlat(point1, point0, point1, point2))
			{
					
				this->listOfNormals.push_back(normal);

				this->listOfCenterOfGravities.push_back(vec3(
							(point0.x + point1.x + point2.x) / 3,
							(point0.y + point1.y + point2.y) / 3,
							(point0.z + point1.z + point2.z) / 3));
			
				this->listIndexesFaces.push_back(vec3(i, i+1, i+2));
			}

		}
	}
}

vector<vec3>* RigidBody_Generic_Object::getListIndexesFaces()
{
	return &this->listIndexesFaces;
}

vector<vec3>* RigidBody_Generic_Object::getListOfCenterOfGravities()
{
	return &this->listOfCenterOfGravities;
}

vector<vec3>* RigidBody_Generic_Object::getListOfNormals()
{
	return &this->listOfNormals;
}

vector<GLushort>* RigidBody_Generic_Object::getListOfIndexes()
{
	return &this->indices;
}

float RigidBody_Generic_Object::getSize()
{
	return this->size;
}

vec3 RigidBody_Generic_Object::getPosLowestVertex()
{
	vec3 posVertex = this->verticesList.at(0);

	for(int i = 1; i < this->verticesList.size(); ++i)
	{
		if(posVertex.y > this->verticesList.at(i).y)
			posVertex = this->verticesList.at(i);
	}

	return posVertex;
}

unsigned int RigidBody_Generic_Object::getLowestVertex()
{
	unsigned int lower = 0;

	for(int i = 1; i < this->verticesList.size(); ++i)
	{
		if(this->verticesList.at(lower).y > this->verticesList.at(i).y)
			lower = i;
	}

	return lower;
}

vec4 RigidBody_Generic_Object::getCollidingFace()
{
	int lowerVertex = getLowestVertex();
	vector<int> possibleFaceIndexList;
	vector<vec3> possibleFaceList;

	for(int i = 0; i < this->listIndexesFaces.size(); ++i)
	{
		if(this->indices.at(this->listIndexesFaces.at(i).x) == lowerVertex ||
			this->indices.at(this->listIndexesFaces.at(i).y) == lowerVertex ||
			this->indices.at(this->listIndexesFaces.at(i).z) == lowerVertex)
		{
			possibleFaceList.push_back(this->listIndexesFaces.at(i));
			possibleFaceIndexList.push_back(i);
		}
	}

	float distance;
	float dotProduct;

	for(int i = 0; i < possibleFaceList.size(); ++i)
	{
		distance = 0.0f;

		dotProduct = MF.dotProduct(vec3(0.0f, -1.0f, 0.0f), this->listOfNormals.at(possibleFaceIndexList.at(i)));

		if(dotProduct != 0.0f)
		{
			// Get the distance of the point of intersection between the pointA and the current face
			distance = (MF.dotProduct(this->listOfNormals.at(possibleFaceIndexList.at(i)), this->listOfCenterOfGravities.at(possibleFaceIndexList.at(i)) - this->centerOfObject))/dotProduct;
					
			// Get the position of the point of intersection
			vec3 posIntersection = vec3(0.0f, -1.0f, 0.0f) * distance + this->centerOfObject;

			unsigned int indexVertexA;
			unsigned int indexVertexB;
			unsigned int indexVertexC;

			vec3 vertexA;
			vec3 vertexB;
			vec3 vertexC;

			indexVertexA = this->indices.at(possibleFaceList.at(i).x);
			indexVertexB = this->indices.at(possibleFaceList.at(i).y);
			indexVertexC = this->indices.at(possibleFaceList.at(i).z);

			// Get the 3 points of the triangle defining the face
			vertexA = this->verticesList.at(indexVertexA);
			vertexB = this->verticesList.at(indexVertexB);
			vertexC = this->verticesList.at(indexVertexC);

			// Check that the intersection point is on the segment and in the triangle
			if(MF.pointOnSegment(posIntersection, this->centerOfObject, vec3(this->centerOfObject.x, this->centerOfObject.y - 1000000.0f, this->centerOfObject.z)) && MF.pointInTriangle(posIntersection, vertexA, vertexB, vertexC))
				return vec4(possibleFaceIndexList.at(i), posIntersection.x, posIntersection.y, posIntersection.z);
		}
	}

	return vec4(-1, 0.0f, 0.0f, 0.0f);
}

void RigidBody_Generic_Object::setIsStatic(bool movable)
{
	this->isStatic = movable;
}

bool RigidBody_Generic_Object::getIsStatic()
{
	return this->isStatic;
}

void RigidBody_Generic_Object::setSize(float size)
{
	this->size = size;
}

void RigidBody_Generic_Object::setAlternateFacesAndNormals(bool alternateFacesAndNormals)
{
	this->alternateFacesAndNormals = alternateFacesAndNormals;
}

bool RigidBody_Generic_Object::getAlternateFacesAndNormals()
{
	return this->alternateFacesAndNormals;
}

void RigidBody_Generic_Object::changeVerticesList(vector<vec3> verticesList)
{
	this->verticesList = verticesList;
}

void RigidBody_Generic_Object::changeIndexesList(vector<GLushort> indexesList)
{
	this->indices = indexesList;
}

void RigidBody_Generic_Object::resetWireframeModeindexes(vector<GLushort> indexesList)
{
	this->listIndexesWireframe = indexesList;
}

void RigidBody_Generic_Object::calculateCenterOfObject()
{
	vec3 highestPoint = this->verticesList.at(0);
	vec3 lowestPoint = this->verticesList.at(0);
	vec3 leftPoint = this->verticesList.at(0);
	vec3 rightPoint = this->verticesList.at(0);
	vec3 frontPoint = this->verticesList.at(0);
	vec3 backPoint = this->verticesList.at(0);

	for(int i = 1; i < this->verticesList.size(); ++i)
	{
		if(this->verticesList.at(i).y > highestPoint.y)
			highestPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).y < lowestPoint.y)
			lowestPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).x > rightPoint.x)
			rightPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).x < leftPoint.x)
			leftPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).z > frontPoint.z)
			frontPoint = this->verticesList.at(i);

		if(this->verticesList.at(i).z < backPoint.z)
			backPoint = this->verticesList.at(i);
	}

	this->centerOfObject = vec3((rightPoint.x + leftPoint.x)/2.0f, (highestPoint.y + lowestPoint.y)/2.0f, (frontPoint.z + backPoint.z)/2.0f);
	
	this->size = MF.distanceBetween2points(highestPoint, centerOfObject);

}

RigidBody_Generic_Object::~RigidBody_Generic_Object(void)
{
}
