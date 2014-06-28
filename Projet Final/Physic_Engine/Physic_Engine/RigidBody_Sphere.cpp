#include "RigidBody_Sphere.h"


RigidBody_Sphere::RigidBody_Sphere(void)
{
	this->isNULL = true;
	this->isStatic = false;
}

RigidBody_Sphere::RigidBody_Sphere(vec3 position, float radius)
{
	this->radius = radius;

	createSphere(10);
	defineIndices(10);

	this->centerOfObject = vec3(0.0f, 0.0f, 0.0f);

	this->axeX.push_back(centerOfObject);
	this->axeX.push_back(vec3(centerOfObject.x + (radius * 1.5f), centerOfObject.y, centerOfObject.z));

	this->axeY.push_back(centerOfObject);
	this->axeY.push_back(vec3(centerOfObject.x, centerOfObject.y  + (radius * 1.5f), centerOfObject.z));

	this->axeZ.push_back(centerOfObject);
	this->axeZ.push_back(vec3(centerOfObject.x, centerOfObject.y, centerOfObject.z  + (radius * 1.5f)));

	changePosition(position.x, position.y, position.z);

	this->isNULL = false;
	this->isStatic = false;
}

void RigidBody_Sphere::rotate(float angle, vec3 u)
{
	vec4 q;
	vec4 qPrime;
	vec4 result;

	u = MF.normalizeVector(u);

	q = this->quaternion.getQuaternion4f(angle, u);

	qPrime = this->quaternion.getQuaternion4f(angle, u* (-1));

	unsigned int i;

	for(i = 0; i < this->nbVertex; ++i)
	{
		this->frontTriangle.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->frontTriangle.at(i).x, this->frontTriangle.at(i).y, this->frontTriangle.at(i).z));
		this->frontTriangle.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->frontTriangle.at(i) += this->centerOfObject;

		this->leftTriangle.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->leftTriangle.at(i).x, this->leftTriangle.at(i).y, this->leftTriangle.at(i).z));
		this->leftTriangle.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->leftTriangle.at(i) += this->centerOfObject;

		this->rearTriangle.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->rearTriangle.at(i).x, this->rearTriangle.at(i).y, this->rearTriangle.at(i).z));
		this->rearTriangle.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->rearTriangle.at(i) += this->centerOfObject;

		this->rightTriangle.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->rightTriangle.at(i).x, this->rightTriangle.at(i).y, this->rightTriangle.at(i).z));
		this->rightTriangle.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->rightTriangle.at(i) += this->centerOfObject;

		this->frontTriangleBottom.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->frontTriangleBottom.at(i).x, this->frontTriangleBottom.at(i).y, this->frontTriangleBottom.at(i).z));
		this->frontTriangleBottom.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->frontTriangleBottom.at(i) += this->centerOfObject;

		this->leftTriangleBottom.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->leftTriangleBottom.at(i).x, this->leftTriangleBottom.at(i).y, this->leftTriangleBottom.at(i).z));
		this->leftTriangleBottom.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->leftTriangleBottom.at(i) += this->centerOfObject;

		this->rearTriangleBottom.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->rearTriangleBottom.at(i).x, this->rearTriangleBottom.at(i).y, this->rearTriangleBottom.at(i).z));
		this->rearTriangleBottom.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->rearTriangleBottom.at(i) += this->centerOfObject;

		this->rightTriangleBottom.at(i) -= this->centerOfObject;
		result = this->quaternion.multiplyQuaternion4f(q, vec4(0,this->rightTriangleBottom.at(i).x, this->rightTriangleBottom.at(i).y, this->rightTriangleBottom.at(i).z));
		this->rightTriangleBottom.at(i) = this->quaternion.multiplyQuaternion3f(result, qPrime);
		this->rightTriangleBottom.at(i) += this->centerOfObject;
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

void RigidBody_Sphere::createSphere(unsigned int levelOfDetail)
{
	if(levelOfDetail < 1)
		levelOfDetail = 1;

	unsigned int i;

	vector<vec3> triangle;
	vector<vec3> *finalTriangle;
	unsigned a;
	finalTriangle = &this->frontTriangle;

	// Variables to calculate the main triangle
	float sideSize;

	// Variables to calculate the left and right subdivision of the main triangle
	vector<vec3> leftSide;
	vector<vec3> rightSide;
	float stepX;
	float stepY;
	float stepZ;
	vec3 pos;

	// Variables to place the triangles inbetween the left and right side
	int div;
	int j;

	// Variables for triangles' curvation
	float distance;
	float dx;
	float dy;
	float dz;
	

	/******************************************************/
	////////// Define the main triangle (UP FRONT) \\\\\\\\\
	/******************************************************/
	
	sideSize = 1.0f;
	
	for(a = 0; a < 8; ++a)
	{
		triangle.clear();

		triangle.push_back(vec3(0.0f, 0.0f, 0.0f));
		triangle.push_back(vec3(0.0f, 0.0f, 0.0f));
		triangle.push_back(vec3(0.0f, 0.0f, 0.0f));

		switch (a)
		{
			case 0:
				/******************************************************/
				////////// Define the main triangle (UP FRONT) \\\\\\\\\
				/******************************************************/

				triangle.at(0).x = 0;
				triangle.at(0).y = sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = -1 * (sideSize / 2.0f);
				triangle.at(1).y = 0;
				triangle.at(1).z = sideSize / 2.0f;

				triangle.at(2).x = sideSize / 2.0f;
				triangle.at(2).y = 0;
				triangle.at(2).z = sideSize / 2.0f;

				finalTriangle = &this->frontTriangle;

				break;
			case 1:
				/*****************************************************/
				////////// Define the main triangle (UP LEFT) \\\\\\\\\
				/*****************************************************/

				triangle.at(0).x = 0;
				triangle.at(0).y = sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = -1 * (sideSize / 2.0f);
				triangle.at(1).y = 0;
				triangle.at(1).z = -1 * (sideSize / 2.0f);

				triangle.at(2).x = -1 * (sideSize / 2.0f);
				triangle.at(2).y = 0;
				triangle.at(2).z = sideSize / 2.0f;

				finalTriangle = &this->leftTriangle;

				break;
			case 2:
				/*****************************************************/
				////////// Define the main triangle (UP REAR) \\\\\\\\\
				/*****************************************************/

				triangle.push_back(vec3(0.0f, 0.0f, 0.0f));
				triangle.push_back(vec3(0.0f, 0.0f, 0.0f));
				triangle.push_back(vec3(0.0f, 0.0f, 0.0f));

				triangle.at(0).x = 0;
				triangle.at(0).y = sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = sideSize / 2.0f;
				triangle.at(1).y = 0;
				triangle.at(1).z = -1 * (sideSize / 2.0f);

				triangle.at(2).x = -1 * (sideSize / 2.0f);
				triangle.at(2).y = 0;
				triangle.at(2).z = -1 * (sideSize / 2.0f);

				finalTriangle = &this->rearTriangle;

				break;
			case 3:
				/******************************************************/
				////////// Define the main triangle (UP RIGHT) \\\\\\\\\
				/******************************************************/
	
				triangle.at(0).x = 0;
				triangle.at(0).y = sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = sideSize / 2.0f;
				triangle.at(1).y = 0;
				triangle.at(1).z = sideSize / 2.0f;

				triangle.at(2).x = sideSize / 2.0f;
				triangle.at(2).y = 0;
				triangle.at(2).z = -1 * (sideSize / 2.0f);

				finalTriangle = &this->rightTriangle;

				break;
			case 4:
				/**********************************************************/
				////////// Define the main triangle (BOTTOM FRONT) \\\\\\\\\
				/**********************************************************/

				triangle.at(0).x = 0;
				triangle.at(0).y = -1 * sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = (sideSize / 2.0f);
				triangle.at(1).y = 0;
				triangle.at(1).z = sideSize / 2.0f;

				triangle.at(2).x = -1 * (sideSize / 2.0f);
				triangle.at(2).y = 0;
				triangle.at(2).z = sideSize / 2.0f;

				finalTriangle = &this->frontTriangleBottom;

				break;
			case 5:
				/*********************************************************/
				////////// Define the main triangle (BOTTOM LEFT) \\\\\\\\\
				/*********************************************************/
	
				triangle.at(0).x = 0;
				triangle.at(0).y = -1 * sideSize * cosf(M_PI * 45.0f/180.0f);
				triangle.at(0).z = 0;

				triangle.at(1).x = -1 * (sideSize / 2.0f);
				triangle.at(1).y = 0;
				triangle.at(1).z = sideSize / 2.0f;

				triangle.at(2).x = -1 * (sideSize / 2.0f);
				triangle.at(2).y = 0;
				triangle.at(2).z = -1 * (sideSize / 2.0f);

				finalTriangle = &this->leftTriangleBottom;

				break;
			case 6:
				
				/*********************************************************/
				////////// Define the main triangle (BOTTOM REAR) \\\\\\\\\
				/*********************************************************/

				triangle.at(0).x = 0;
				triangle.at(0).y = -1 * (sideSize * cosf(M_PI * 45.0f/180.0f));
				triangle.at(0).z = 0;

				triangle.at(1).x = -1 * (sideSize / 2.0f);
				triangle.at(1).y = 0;
				triangle.at(1).z = -1 * (sideSize / 2.0f);

				triangle.at(2).x = sideSize / 2.0f;
				triangle.at(2).y = 0;
				triangle.at(2).z = -1 * (sideSize / 2.0f);

				finalTriangle = &this->rearTriangleBottom;

				break;
			case 7:
				/**********************************************************/
				////////// Define the main triangle (BOTTOM RIGHT) \\\\\\\\\
				/**********************************************************/
	
				triangle.at(0).x = 0;
				triangle.at(0).y = -1 * (sideSize * cosf(M_PI * 45.0f/180.0f));
				triangle.at(0).z = 0;

				triangle.at(1).x = sideSize / 2.0f;
				triangle.at(1).y = 0;
				triangle.at(1).z = -1 * (sideSize / 2.0f);

				triangle.at(2).x = sideSize / 2.0f;
				triangle.at(2).y = 0;
				triangle.at(2).z = sideSize / 2.0f;

				finalTriangle = &this->rightTriangleBottom;

				break;
		}
		
	
		////////////////// Subdivision of the main triangle \\\\\\\\\\\\\\\\

		// Subdivide the LEFT side
		/*
			  ->/\
			 ->/  \
			->/    \
		*/
		stepX = (triangle.at(1).x - triangle.at(0).x)/levelOfDetail;
		stepY = (triangle.at(1).y - triangle.at(0).y)/levelOfDetail;
		stepZ = (triangle.at(1).z - triangle.at(0).z)/levelOfDetail;

		leftSide.clear();
		pos = triangle.at(0);

		for(i = 0; i < levelOfDetail ; ++i)
		{
			pos.x += stepX;
			pos.y += stepY;
			pos.z += stepZ;

			leftSide.push_back(pos);
		}

		// Subdivide the RIGHT side
		/*
				/\<-
			   /  \<-
			  /    \<-
		*/
		stepX = (triangle.at(2).x - triangle.at(0).x)/levelOfDetail;
		stepY = (triangle.at(2).y - triangle.at(0).y)/levelOfDetail;
		stepZ = (triangle.at(2).z - triangle.at(0).z)/levelOfDetail;

		rightSide.clear();
		pos = triangle.at(0);

		for(i = 0; i < levelOfDetail ; ++i)
		{
			pos.x += stepX;
			pos.y += stepY;
			pos.z += stepZ;

			rightSide.push_back(pos);
		}

		// Place all the points between the left and right sides to create triangles
		/*
				/\
			   /\/\
			  /\/\/\
		*/

		finalTriangle->push_back(triangle.at(0));

		div =0;

		if(a == 0 || a == 2 || a == 4 || a == 6)
		{
			for(i = 0; i < levelOfDetail; ++i)
			{
				++div;

				stepX = (rightSide.at(i).x - leftSide.at(i).x) / div;

				finalTriangle->push_back(leftSide.at(i));

				pos = leftSide.at(i);

				for(j = 0; j < div; ++j)
				{
					pos.x += stepX;

					finalTriangle->push_back(pos);
				}
			}
		}
		else if(a == 1 || a == 3 || a == 5 || a == 7)
		{
			for(i = 0; i < levelOfDetail; ++i)
			{
				++div;

				stepZ = (rightSide.at(i).z - leftSide.at(i).z) / div;

				finalTriangle->push_back(leftSide.at(i));

				pos = leftSide.at(i);

				for(j = 0; j < div; ++j)
				{
					pos.z += stepZ;

					finalTriangle->push_back(pos);
				}
			}
		}

		// Curve all the triangle around the origin of the world (0.0, 0.0, 0.0)
	
		for(i = 0; i < finalTriangle->size(); ++i)
		{
			dx = finalTriangle->at(i).x - 0.0f;
			dy = finalTriangle->at(i).y - 0.0f;
			dz = finalTriangle->at(i).z - 0.0f;

			distance = sqrtf(pow(dx,2) + pow(dy,2) + pow(dz, 2));

			finalTriangle->at(i).x = dx * this->radius / distance;
			finalTriangle->at(i).y = dy * this->radius / distance;
			finalTriangle->at(i).z = dz * this->radius / distance;
		}
	}

	this->nbVertex = finalTriangle->size();
}

void RigidBody_Sphere::defineIndices(int levelOfDetail)
{
	if(levelOfDetail < 1)
		levelOfDetail = 1;

	int nbLoop;

	int changingThreshold;
	int	a;
	int b;
	int c;
	int nbToFlatTriangleStart;

	int i;
	int j;
	
	int pos;

	nbLoop = 0;
	a = 1;
	for(i = 0; i < levelOfDetail; ++i)
	{
		nbLoop += a++;
	}

	pos = 0;
	changingThreshold = 1;
	a = 1;
	b = 0;
	nbToFlatTriangleStart = 0;

	for(i = 0; i < nbLoop; ++i)
	{
		b++;
		
		this->indices.push_back(i + a);
		this->indices.push_back(i + a + 1);
		this->indices.push_back(i);

		if(changingThreshold == b)
		{
			++a;
			b=0;
			changingThreshold +=1;
		}

		if(b > 0)
		{
			this->indices.push_back(i);
			this->indices.push_back(i + 1);
			this->indices.push_back(i + a + 1);
		}
		else if(i < (nbLoop-1))
		{

			c = i;

			for(j = 0; j < nbToFlatTriangleStart; ++j)
			{
				this->indices.push_back(c);
				this->indices.push_back(c - 1);
				this->indices.push_back(c - 1);
				this->indices.push_back(c);
				this->indices.push_back(c - 1);
				this->indices.push_back(c - 1);

				--c;
			}

			this->indices.push_back(i - (a - 2));
			this->indices.push_back(i - (a - 2) + (a - 1));
			this->indices.push_back(i - (a - 2) + (a - 1));

			++nbToFlatTriangleStart;
		}

	}
}

void RigidBody_Sphere::changePosition(float posX, float posY, float posZ)
{
	unsigned int i;

	this->centerOfObject.x += posX;
	this->centerOfObject.y += posY;
	this->centerOfObject.z += posZ;

	for(i = 0; i < this->nbVertex; ++i)
	{
		this->frontTriangle.at(i).x += posX;
		this->frontTriangle.at(i).y += posY;
		this->frontTriangle.at(i).z += posZ;

		this->leftTriangle.at(i).x += posX;
		this->leftTriangle.at(i).y += posY;
		this->leftTriangle.at(i).z += posZ;

		this->rearTriangle.at(i).x += posX;
		this->rearTriangle.at(i).y += posY;
		this->rearTriangle.at(i).z += posZ;

		this->rightTriangle.at(i).x += posX;
		this->rightTriangle.at(i).y += posY;
		this->rightTriangle.at(i).z += posZ;

		this->frontTriangleBottom.at(i).x += posX;
		this->frontTriangleBottom.at(i).y += posY;
		this->frontTriangleBottom.at(i).z += posZ;

		this->leftTriangleBottom.at(i).x += posX;
		this->leftTriangleBottom.at(i).y += posY;
		this->leftTriangleBottom.at(i).z += posZ;

		this->rearTriangleBottom.at(i).x += posX;
		this->rearTriangleBottom.at(i).y += posY;
		this->rearTriangleBottom.at(i).z += posZ;

		this->rightTriangleBottom.at(i).x += posX;
		this->rightTriangleBottom.at(i).y += posY;
		this->rightTriangleBottom.at(i).z += posZ;
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

void RigidBody_Sphere::render(GLint *positionAttrib, GLint *colorUniform)
{

	
	//// FRONT (UP) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->frontTriangle.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	///// RIGHT (UP) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->rightTriangle.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	////// REAR (UP) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->rearTriangle.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));


	////// LEFT (UP) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->leftTriangle.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	//// FRONT (BOTTOM) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->frontTriangleBottom.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	///// RIGHT (BOTTOM) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->rightTriangleBottom.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));


	////// REAR (BOTTOM) \\\\\

		glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->rearTriangleBottom.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	////// LEFT (BOTTOM) \\\\\

	glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->leftTriangleBottom.at(0).x);
	glUniform4f(*colorUniform,0.f, 1.f, 0.f, 1.0f);
	glEnableVertexAttribArray(*positionAttrib);
	glDrawElements(GL_LINE_LOOP, this->indices.size(),GL_UNSIGNED_SHORT,&this->indices.at(0));

	////// AXES \\\\\
	
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

vec3 RigidBody_Sphere::getNormal()
{
	vec3 vectX;
	vec3 vectZ;
	vec3 cp;

	vectX = MF.getVector(this->axeX.at(0), this->axeX.at(1));
	vectZ = MF.getVector(this->axeZ.at(0), this->axeZ.at(1));

	cp = MF.crossProduct(vectX, vectZ);
	
	return MF.normalizeVector(MF.inverseNormal(cp));
}

vec3 RigidBody_Sphere::getCenterOfObject()
{
	return this->centerOfObject;
}

float RigidBody_Sphere::getRadius()
{
	return this->radius;
}

void RigidBody_Sphere::setIsStatic(bool movable)
{
	this->isStatic = movable;
}

bool RigidBody_Sphere::getIsStatic()
{
	return this->isStatic;
}

RigidBody_Sphere::~RigidBody_Sphere(void)
{
}
