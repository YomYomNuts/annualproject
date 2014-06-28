#include "ObjectManager.h"

ObjectManager::ObjectManager(void)
{
	this->listOfObject = vector<GameObject*>();
}

void ObjectManager::addObject(GameObject* object)
{
	unsigned int i;
	bool add;

	add = true;

	for(i = 0; i < this->listOfObject.size(); ++i)
	{
		if(this->listOfObject.at(i)->getId() == object->getId())
		{
			add= false;
			break;
		}
	}

	(add)?this->listOfObject.push_back(object):0;
}

int ObjectManager::getNumberOfObject()
{
	return this->listOfObject.size();
}

float ObjectManager::getDistanceSphereToPlane(GameObject* sphere, GameObject* plane, float elapsedTime)
{
	vec3 vector;

	vector = MF.getVector(plane->getComponents()->getRigidBody()->getCenterOfObject(), sphere->getComponents()->getRigidBody()->getCenterOfObject() - (*sphere->getVelocity() * elapsedTime) );

	return MF.dotProduct(vector, plane->getComponents()->getRigidBody()->getRigidBodyObjectPlane()->getNormal());
}

float ObjectManager::getDistanceSphereToSphere(GameObject* sphere1, GameObject* sphere2, float elapsedTime)
{
	vec3 distance;

	distance = (sphere1->getComponents()->getRigidBody()->getCenterOfObject() + (*sphere1->getVelocity() * elapsedTime)) - (sphere2->getComponents()->getRigidBody()->getCenterOfObject() + (*sphere2->getVelocity() * elapsedTime));

	return sqrt(MF.dotProduct(distance, distance));
}

float ObjectManager::getDistanceCubeToPlane(GameObject* cube, GameObject* plane, float elapsedTime)
{
	vec3 vector;

	vector = MF.getVector(plane->getComponents()->getRigidBody()->getCenterOfObject(), cube->getComponents()->getRigidBody()->getCenterOfObject() - (*cube->getVelocity() * elapsedTime) );

	return MF.dotProduct(vector, plane->getComponents()->getRigidBody()->getRigidBodyObjectPlane()->getNormal());
}

float ObjectManager::getDistanceBetweenGameObjects(vec3* posGO1, vec3* posGO2, vec3* velocityGO1, vec3* normalGO2, float elapsedTime)
{
	vec3 vector;

	vector = MF.getVector(*posGO2, *posGO1 - (*velocityGO1 * elapsedTime) );

	return MF.dotProduct(vector, *normalGO2);
}

float ObjectManager::getDistanceCubeToCube(GameObject* cube1, GameObject* cube2, float elapsedTime)
{
	vec3 distance;

	distance = (cube1->getComponents()->getRigidBody()->getCenterOfObject() + (*cube1->getVelocity() * elapsedTime)) - (cube2->getComponents()->getRigidBody()->getCenterOfObject() + (*cube2->getVelocity() * elapsedTime));

	return sqrt(MF.dotProduct(distance, distance));
}

bool ObjectManager::isVertexInsideObject(vector<vec3>* objectListOfNormals, vector<vec3>* objectListOfCOG, vector<GLushort>* objectListOfIndexes,
										 vector<vec3>* objectListIndexesOfFaces, vector<vec3>* objectListOfVertices,
										 vec3 vertexPos, vec3* velocityOfCurrentObject, float elapsedTime)
{
	unsigned int i;
	unsigned int j;
	unsigned int nbNormals;
	unsigned int count;

	vec3 direction;
	vector<vec3> listIntersectionPoints;
	float dotProduct;
	float distance;
	bool intersection;

	nbNormals = objectListOfNormals->size();

	// Loop on all the faces
	for(i = 0; i < nbNormals; i++)
	{
		
		count = 0;
		// Get the direction of the vector from the current vertex pointing to the center of gravity of the current face
		// This define a ray cast
		direction = MF.getVector(vertexPos - (*velocityOfCurrentObject * elapsedTime), objectListOfCOG->at(i));
		direction = MF.normalizeVector(direction);
		
		// Loop on the others faces, to check if the ray cast collides with one of them
		for(j = 0; j < nbNormals; ++j)
		{
			// We don't check twice the same face
			if(j != -1)
			{
				distance = 0.0f;

				// Check if the ray is parallel to the face
				dotProduct = MF.dotProduct(direction, objectListOfNormals->at(j));

				if(dotProduct != 0.0f)
				{
					// Get the distance of the point of intersection between the vertex and the current face
					distance = (MF.dotProduct(objectListOfNormals->at(j), objectListOfCOG->at(j) - (vertexPos - (*velocityOfCurrentObject * elapsedTime))))/dotProduct;
					
					// Get the position of the point of intersection
					vec3 posIntersection = direction * distance + vertexPos;

					if(distance >0)
					{
						unsigned int indexVertexA;
						unsigned int indexVertexB;
						unsigned int indexVertexC;

						vec3 vertexA;
						vec3 vertexB;
						vec3 vertexC;

						indexVertexA = objectListOfIndexes->at(objectListIndexesOfFaces->at(j).x);
						indexVertexB = objectListOfIndexes->at(objectListIndexesOfFaces->at(j).y);
						indexVertexC = objectListOfIndexes->at(objectListIndexesOfFaces->at(j).z);

						// Get the 3 points of the triangle defining the face
						vertexA = objectListOfVertices->at(indexVertexA);
						vertexB = objectListOfVertices->at(indexVertexB);
						vertexC = objectListOfVertices->at(indexVertexC);
						
						// Chekc if the point of intersection is in the triangle
						if(MF.pointInTriangle(posIntersection, vertexA, vertexB, vertexC))
						{
							// Check if the point of intersection already exists
							unsigned int k = 0;
							for(k = 0; k < listIntersectionPoints.size(); ++k)
							{
								if(listIntersectionPoints.at(k) == posIntersection)
								{
									break;
								}
							}

							if(k == listIntersectionPoints.size())
							{
								// Increase the number of intersection
								++count;
								// Save the point of intersection so we don't count it again
								listIntersectionPoints.push_back(posIntersection);
							}
						}
					}
				}
			}
		}

		// if the number of intersection of the ray cast is modulo 2, then it means the point is OUTSIDE  the polygon
		intersection = (count%2 == 0?false:true);

		if(intersection)
			return true;
	}

	return false;
}

vec4 ObjectManager::isSegmentInsideObject(RigidBody* otherObjectRigidBody, vec3 pointA, vec3 pointB, vec3* velocityOfCurrentObject, float elapsedTime)
{
	unsigned int i;
	unsigned int nbNormals;

	vec3 direction;
	float dotProduct;
	float distance;
	float size;

	nbNormals = otherObjectRigidBody->getListOfNormals()->size();

	// Get the direction
	direction = MF.getVector(pointA - (*velocityOfCurrentObject * elapsedTime), pointB - (*velocityOfCurrentObject * elapsedTime));
	direction = MF.normalizeVector(direction);


	size = MF.distanceBetween2points(pointA, pointB);

	// Loop on all the faces
	for(i = 0; i < nbNormals; i++)
	{
		distance = 0.0f;

		distance = MF.distanceBetween2points(pointA, otherObjectRigidBody->getListOfNormals()->at(i));

		//if(distance < 2 * size)
		//{
			// Check if the segment is parallel to the face
			dotProduct = MF.dotProduct(direction, otherObjectRigidBody->getListOfNormals()->at(i));

			if(dotProduct != 0.0f)
			{
				// Get the distance of the point of intersection between the pointA and the current face
				distance = (MF.dotProduct(otherObjectRigidBody->getListOfNormals()->at(i), otherObjectRigidBody->getListOfCenterOfGravities()->at(i) - (pointA - (*velocityOfCurrentObject * elapsedTime))))/dotProduct;
					
				// Get the position of the point of intersection
				vec3 posIntersection = direction * distance + pointA;

				unsigned int indexVertexA;
				unsigned int indexVertexB;
				unsigned int indexVertexC;

				vec3 vertexA;
				vec3 vertexB;
				vec3 vertexC;

				indexVertexA = otherObjectRigidBody->getListOfIndexes()->at(otherObjectRigidBody->getListIndexesFaces()->at(i).x);
				indexVertexB = otherObjectRigidBody->getListOfIndexes()->at(otherObjectRigidBody->getListIndexesFaces()->at(i).y);
				indexVertexC = otherObjectRigidBody->getListOfIndexes()->at(otherObjectRigidBody->getListIndexesFaces()->at(i).z);

				// Get the 3 points of the triangle defining the face
				vertexA = otherObjectRigidBody->getListOfVertices()->at(indexVertexA);
				vertexB = otherObjectRigidBody->getListOfVertices()->at(indexVertexB);
				vertexC = otherObjectRigidBody->getListOfVertices()->at(indexVertexC);

				// Check that the intersection point is on the segment and in the triangle
				if(MF.pointOnSegment(posIntersection, pointA, pointB) && MF.pointInTriangle(posIntersection, vertexA, vertexB, vertexC))
				{
					return vec4(1.0f, posIntersection.x, posIntersection.y, posIntersection.z);
				}
			}
		//}
	}

	return vec4(0.0f, 0.0f, 0.0f, 0.0f);
}

void ObjectManager::defineGameObjectAround(float elapsedTime)
{
	unsigned int i;
	unsigned int j;
	float distance;
	float allowedDistance;

	GameObject* currentObject;
	GameObject* otherObject;

	float coefDistance = 2;

	for(i = 0; i < (this->listOfObject.size() - 1); ++i)
	{
		currentObject  = this->listOfObject.at(i);
		currentObject->getComponents()->getRigidBody()->calculateNormalsAndCenterOfGravities();

		// Check if the rigidbody of the current object is activated
		if(currentObject->getComponents()->getRigidBody()->getRigidBodyStatus())
		{
			for(j = i+1; j < this->listOfObject.size(); ++j)
			{
				if(i != j)
				{
					otherObject = this->listOfObject.at(j);
					// Check if the rigidbody of the object to test is activated
					if(otherObject->getComponents()->getRigidBody()->getRigidBodyStatus())
					{

						//allowedDistance = abs(currentObject->getAxeSize() * coefDistance);
						allowedDistance = currentObject->getComponents()->getRigidBody()->getSize() * coefDistance;

						distance = abs(getDistanceBetweenGameObjects(&currentObject->getCenterOfObject(), &otherObject->getCenterOfObject(), currentObject->getVelocity(), otherObject->getVelocity(), 1.0f));

						if(distance <= allowedDistance)
						{
							currentObject->addGameObjectAround(otherObject);
							otherObject->addGameObjectAround(currentObject);
						}
						
					}
				}
			}
		}
	}

	currentObject  = this->listOfObject.at(i);
	currentObject->getComponents()->getRigidBody()->calculateNormalsAndCenterOfGravities();
}

void ObjectManager::render(GLint *positionAttrib, GLint *colorUniform)
{
	unsigned int i;
	
	for(i = 0; i < this->listOfObject.size(); ++i)
	{
		if(this->listOfObject.at(i)->getRenderObject())
			this->listOfObject.at(i)->render(positionAttrib, colorUniform);
		if(this->listOfObject.at(i)->getComponents()->getRigidBody()->getDisplayRigidBody())
			this->listOfObject.at(i)->getComponents()->getRigidBody()->renderRigidBody(positionAttrib, colorUniform);
	}
}

void ObjectManager::setGravity(float gravity)
{
	unsigned int i;

	for(i = 0; i < this->listOfObject.size(); ++i)
	{
		this->listOfObject.at(i)->getComponents()->getGravity()->setGravity(gravity);
	}
}

void ObjectManager::update(float elapsedTime)
{
	unsigned int i;
	unsigned int j;
	float distance;
	
	bool inside = false;
	unsigned int k;

	defineGameObjectAround(elapsedTime);

	GameObject* currentObject;


	for(i = 0; i < this->listOfObject.size(); ++i)
	{
		currentObject = this->listOfObject.at(i);

		if(currentObject->getComponents()->getGravity()->getUseGravity())
		{
			currentObject->getComponents()->getGravity()->addForce(vec3(0,5,0), currentObject->getVelocity(), elapsedTime);
		}

		if(!currentObject->getComponents()->getRigidBody()->getIsStatic())
		{
			switch(currentObject->getComponents()->getRigidBody()->getType())
			{
				/************/
				/*  SPHERE  */
				/************/
				case RIGID_SPHERE:
					updateCollisionWithSphere(currentObject, elapsedTime);
					break;

				/***********/
				/*  CUBE  */
				/***********/
				case RIGID_CUBE:
					updateCollisionWithCube(currentObject, elapsedTime);
					break;

				/*************/
				/*  GENERIC  */
				/*************/
				case RIGID_GENERIC:
					updateCollisionWithGeneric(currentObject, elapsedTime);
					break;
			}

			currentObject->update(elapsedTime);
		}
	}
}

void ObjectManager::updateCollisionWithSphere(GameObject* currentObject, float elapsedTime)
{
	GameObject* currentObjectAround;
	unsigned int i;
	float distance;

	for(i = 0; i < currentObject->getGameObjectAround()->size(); ++i)
	{
		currentObjectAround = currentObject->getGameObjectAround()->at(i);
					
		switch (currentObjectAround->getComponents()->getRigidBody()->getType())
		{
			//////////////////// PLANE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_PLANE:
				distance = getDistanceSphereToPlane(currentObject, currentObjectAround, elapsedTime);
							
				if(distance < currentObject->getComponents()->getRigidBody()->getRigidBodyObjectSphere()->getRadius())
				{
					float resistance;
					float bounciness;

					resistance = currentObject->getComponents()->getGravity()->getResistance();
					bounciness = currentObject->getComponents()->getGravity()->getBounciness();

					currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y / bounciness, currentObject->getVelocity()->z / resistance));
				}
				break;
						
			//////////////////// SPHERE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_SPHERE:

				distance = getDistanceSphereToSphere(currentObject, currentObjectAround, 0.0f);

				float radiusSum;

				radiusSum = currentObject->getComponents()->getRigidBody()->getRigidBodyObjectSphere()->getRadius() + 
							currentObject->getObjectsAround()->at(i)->getComponents()->getRigidBody()->getRigidBodyObjectSphere()->getRadius();

				if(distance - radiusSum < 0.001)
				{
					vec3 impactForce1 = vec3(0.0f, 0.0f, 0.0f);
					vec3 impactForce2 = vec3(0.0f, 0.0f, 0.0f);
					float impactTotal;

					vec3 direction;

					direction = currentObject->getCenterOfObject() - currentObjectAround->getCenterOfObject();
					direction.Normalize();
								
					impactTotal = currentObject->getMass() * currentObject->getVelocity()->Length() +
								currentObjectAround->getMass() * currentObjectAround->getVelocity()->Length();

					float ratio = 0.0f;

					//ratio = this->listOfObject.at(i)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
					ratio = currentObjectAround->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
					impactForce1.x = ratio * impactTotal * -direction.x;
					impactForce1.y = ratio * impactTotal * -direction.y; 
					impactForce1.z = ratio * impactTotal * -direction.z;

					//ratio = this->listOfObject.at(i)->getObjectsAround().at(j)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
					ratio = currentObject->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
					impactForce2.x = ratio * impactTotal * direction.x;
					impactForce2.y = ratio * impactTotal * direction.y; 
					impactForce2.z = ratio * impactTotal * direction.z;

					float resistance;
					float bounciness;

					bounciness = currentObject->getComponents()->getGravity()->getBounciness();
					resistance = currentObject->getComponents()->getGravity()->getResistance();
								
					currentObject->getComponents()->getGravity()->addForce(impactForce1 / ((resistance + bounciness) * 10),  currentObject->getVelocity(), elapsedTime);

					bounciness = currentObjectAround->getComponents()->getGravity()->getBounciness();
					resistance = currentObjectAround->getComponents()->getGravity()->getResistance();

					currentObjectAround->getComponents()->getGravity()->addForce(impactForce2 / ((resistance + bounciness) * 10), currentObjectAround->getVelocity(), elapsedTime);
				}

				break;
		}
	}
}

void ObjectManager::updateCollisionWithCube(GameObject* currentObject, float elapsedTime)
{
	//reorientObject(currentObject, elapsedTime);
	GameObject* currentObjectAround;
	unsigned int i;
	unsigned int j;
	float distance;

	RigidBody* currentObjectRigidBodyCube;
	currentObjectRigidBodyCube = currentObject->getComponents()->getRigidBody();
						
	for(i = 0; i < currentObject->getGameObjectAround()->size(); ++i)
	{
		currentObjectAround = currentObject->getGameObjectAround()->at(i);

		switch (currentObjectAround->getComponents()->getRigidBody()->getType())
		{
			//////////////////// PLANE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_PLANE:

				RigidBody_Plane* otherObjectRigidBodyPlane;
				otherObjectRigidBodyPlane = currentObjectAround->getComponents()->getRigidBody()->getRigidBodyObjectPlane();

				distance = getDistanceBetweenGameObjects(&currentObjectRigidBodyCube->getPosLowestVertex(), &otherObjectRigidBodyPlane->getCenterOfObject(),
														currentObject->getVelocity(), &otherObjectRigidBodyPlane->getNormal(), elapsedTime);
				if(distance < 0.0001f)
				{
					float resistance;
					float bounciness;

					resistance = currentObject->getComponents()->getGravity()->getResistance();
					bounciness = currentObject->getComponents()->getGravity()->getBounciness();

								
								
					currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y / bounciness, currentObject->getVelocity()->z / resistance));
							
				}

				break;
						
			//////////////////// CUBE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_CUBE:

				RigidBody* otherObjectRigidBody;
				otherObjectRigidBody = currentObjectAround->getComponents()->getRigidBody();

				unsigned int indexVertexA;
				unsigned int indexVertexB;
				unsigned int indexVertexC;

				vec3 vertexA;
				vec3 vertexB;
				vec3 vertexC;

				bool crossing = false;
				vec3 impactPoint;
				vec4 impact;
				
				float distance1 = 0.0f;
				float distance2 = 0.0f;

				distance1 = currentObjectRigidBodyCube->getSize() + otherObjectRigidBody->getSize();

				distance2 = MF.distanceBetween2points(currentObject->getCenterOfObject(), currentObjectAround->getCenterOfObject());
				crossing = false;
				if(distance1 >= (distance2 + 0.1f))
				{

					for(j = 0; j < currentObjectRigidBodyCube->getListIndexesFaces()->size(); ++j)
					{
						indexVertexA = currentObjectRigidBodyCube->getListOfIndexes()->at(currentObjectRigidBodyCube->getListIndexesFaces()->at(j).x);
						indexVertexB = currentObjectRigidBodyCube->getListOfIndexes()->at(currentObjectRigidBodyCube->getListIndexesFaces()->at(j).y);
						indexVertexC = currentObjectRigidBodyCube->getListOfIndexes()->at(currentObjectRigidBodyCube->getListIndexesFaces()->at(j).z);

						// Get the 3 points of the triangle defining the face
						vertexA = currentObjectRigidBodyCube->getListOfVertices()->at(indexVertexA);
						vertexB = currentObjectRigidBodyCube->getListOfVertices()->at(indexVertexB);
						vertexC = currentObjectRigidBodyCube->getListOfVertices()->at(indexVertexC);

								

						// Test the first segement of the triangle
						/*impact = isSegmentInsideObject(otherObjectRigidBody->getListOfNormals(), otherObjectRigidBody->getListOfCenterOfGravities(), otherObjectRigidBody->getListOfIndexes(),
							otherObjectRigidBody->getListIndexesFaces(), otherObjectRigidBody->getListOfVertices(),vertexA, vertexB,
							currentObject->getVelocity(), elapsedTime);*/
						impact = isSegmentInsideObject(otherObjectRigidBody, vertexA, vertexB,currentObject->getVelocity(), elapsedTime);

						crossing = (impact.x == 1.0f? true: false);
						impactPoint = vec3(impact.y, impact.z, impact.w);

						if(!crossing)
						{
							// Test the second segment of the triangle
							/*impact = isSegmentInsideObject(otherObjectRigidBody->getListOfNormals(), otherObjectRigidBody->getListOfCenterOfGravities(), otherObjectRigidBody->getListOfIndexes(),
															otherObjectRigidBody->getListIndexesFaces(), otherObjectRigidBody->getListOfVertices(),vertexB, vertexC,
															currentObject->getVelocity(), elapsedTime);*/
							impact = isSegmentInsideObject(otherObjectRigidBody, vertexB, vertexC,currentObject->getVelocity(), elapsedTime);
							crossing = (impact.x == 1.0f? true: false);
							impactPoint = vec3(impact.y, impact.z, impact.w);

							if(!crossing)
							{
								// Test the third segement of the triangle
								/*impact = isSegmentInsideObject(otherObjectRigidBody->getListOfNormals(), otherObjectRigidBody->getListOfCenterOfGravities(), otherObjectRigidBody->getListOfIndexes(),
																otherObjectRigidBody->getListIndexesFaces(), otherObjectRigidBody->getListOfVertices(),vertexC, vertexA,
																currentObject->getVelocity(), elapsedTime);*/
							
								impact = isSegmentInsideObject(otherObjectRigidBody, vertexC, vertexA,currentObject->getVelocity(), elapsedTime);
								crossing = (impact.x == 1.0f? true: false);
								impactPoint = vec3(impact.y, impact.z, impact.w);
										
							}
						}

						if(crossing)
							break;
					}
				}

				if(crossing)
				{
					float resistance;
					float bounciness;

					vec3 direction;

					direction = currentObject->getCenterOfObject() - currentObjectAround->getCenterOfObject();
					//direction = impactPoint - currentObjectAround->getCenterOfObject();
					direction.Normalize();
					
					if(otherObjectRigidBody->getIsStatic())
					{
						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						//currentObject->getComponents()->getGravity()->addForce(impactForce1/5 ,  currentObject->getVelocity(), elapsedTime);
						currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y /bounciness /* * ((bounciness+resistance)*0.001)*/, currentObject->getVelocity()->z / resistance));
					}
					else
					{
						vec3 impactForce1 = vec3(0.0f, 0.0f, 0.0f);
						vec3 impactForce2 = vec3(0.0f, 0.0f, 0.0f);
						float impactTotal;
						impactTotal = currentObject->getMass() * currentObject->getVelocity()->Length() +
									currentObjectAround->getMass() * currentObjectAround->getVelocity()->Length();

						float ratio = 0.0f;

						//ratio = this->listOfObject.at(i)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObjectAround->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce1.x = ratio * impactTotal * -direction.x;
						impactForce1.y = ratio * impactTotal * -direction.y; 
						impactForce1.z = ratio * impactTotal * -direction.z;

						//ratio = this->listOfObject.at(i)->getObjectsAround().at(j)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObject->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce2.x = ratio * impactTotal * direction.x;
						impactForce2.y = ratio * impactTotal * direction.y; 
						impactForce2.z = ratio * impactTotal * direction.z;

						

						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						currentObject->getComponents()->getGravity()->addForce(impactForce1 / ((resistance + bounciness) * 10),  currentObject->getVelocity(), elapsedTime);

						bounciness = currentObjectAround->getComponents()->getGravity()->getBounciness();
						resistance = currentObjectAround->getComponents()->getGravity()->getResistance();

						currentObjectAround->getComponents()->getGravity()->addForce(impactForce2 / ((resistance + bounciness) * 10), currentObjectAround->getVelocity(), elapsedTime);
				
					}
				}		
				break;
		}
	}
	reorientObject(currentObject, elapsedTime);
}

void ObjectManager::updateCollisionWithGeneric(GameObject* currentObject, float elapsedTime)
{
	//reorientObject(currentObject, elapsedTime);
	GameObject* currentObjectAround;
	float distance;
	unsigned int i;
	unsigned int j;

	unsigned int indexVertexA;
	unsigned int indexVertexB;
	unsigned int indexVertexC;

	vec3 vertexA;
	vec3 vertexB;
	vec3 vertexC;

	bool crossing = false;
	vec3 impactPoint;
	vec4 impact;

	float distance1 = 0.0f;
	float distance2 = 0.0f;

	RigidBody* currentObjectRigidBodyGeneric;
	currentObjectRigidBodyGeneric = currentObject->getComponents()->getRigidBody();
						
	for(i = 0; i < currentObject->getGameObjectAround()->size(); ++i)
	{
		currentObjectAround = currentObject->getGameObjectAround()->at(i);

		switch (currentObjectAround->getComponents()->getRigidBody()->getType())
		{
			//////////////////// PLANE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_PLANE:

				RigidBody_Plane* otherObjectRigidBodyPlane;
				otherObjectRigidBodyPlane = currentObjectAround->getComponents()->getRigidBody()->getRigidBodyObjectPlane();

				distance = getDistanceBetweenGameObjects(&currentObjectRigidBodyGeneric->getPosLowestVertex(), &otherObjectRigidBodyPlane->getCenterOfObject(),
														currentObject->getVelocity(), &otherObjectRigidBodyPlane->getNormal(), elapsedTime);
				if(distance < 0.0001f)
				{
					float resistance;
					float bounciness;

					resistance = currentObject->getComponents()->getGravity()->getResistance();
					bounciness = currentObject->getComponents()->getGravity()->getBounciness();

					currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y / bounciness, currentObject->getVelocity()->z / resistance));
				}

				break;
						
			//////////////////// CUBE \\\\\\\\\\\\\\\\\\\\\

			case RIGID_CUBE:

				RigidBody* otherObjectRigidBodyCube;
				otherObjectRigidBodyCube = currentObjectAround->getComponents()->getRigidBody();

				distance1 = currentObjectRigidBodyGeneric->getSize() + otherObjectRigidBodyCube->getSize();

				distance2 = MF.distanceBetween2points(currentObject->getCenterOfObject(), currentObjectAround->getCenterOfObject());
				crossing = false;
				if(distance1 >= (distance2 + 0.1f))
				{
					for(j = 0; j < currentObjectRigidBodyGeneric->getListIndexesFaces()->size(); ++j)
					{
						indexVertexA = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).x);
						indexVertexB = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).y);
						indexVertexC = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).z);

						// Get the 3 points of the triangle defining the face
						vertexA = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexA);
						vertexB = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexB);
						vertexC = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexC);

								

						// Test the first segement of the triangle
						/*impact = isSegmentInsideObject(otherObjectRigidBodyCube->getListOfNormals(), otherObjectRigidBodyCube->getListOfCenterOfGravities(), otherObjectRigidBodyCube->getListOfIndexes(),
							otherObjectRigidBodyCube->getListIndexesFaces(), otherObjectRigidBodyCube->getListOfVertices(),vertexA, vertexB,
							currentObject->getVelocity(), elapsedTime);*/

						impact = isSegmentInsideObject(otherObjectRigidBodyCube, vertexA, vertexB,currentObject->getVelocity(), elapsedTime);
						crossing = (impact.x == 1.0f? true: false);
						impactPoint = vec3(impact.y, impact.z, impact.w);

						if(!crossing)
						{
							// Test the second segment of the triangle
							/*impact = isSegmentInsideObject(otherObjectRigidBodyCube->getListOfNormals(), otherObjectRigidBodyCube->getListOfCenterOfGravities(), otherObjectRigidBodyCube->getListOfIndexes(),
															otherObjectRigidBodyCube->getListIndexesFaces(), otherObjectRigidBodyCube->getListOfVertices(),vertexB, vertexC,
															currentObject->getVelocity(), elapsedTime);*/
						
							impact = isSegmentInsideObject(otherObjectRigidBodyCube, vertexB, vertexC,currentObject->getVelocity(), elapsedTime);
							crossing = (impact.x == 1.0f? true: false);
							impactPoint = vec3(impact.y, impact.z, impact.w);

							if(!crossing)
							{
								// Test the third segement of the triangle
								/*impact = isSegmentInsideObject(otherObjectRigidBodyCube->getListOfNormals(), otherObjectRigidBodyCube->getListOfCenterOfGravities(), otherObjectRigidBodyCube->getListOfIndexes(),
																otherObjectRigidBodyCube->getListIndexesFaces(), otherObjectRigidBodyCube->getListOfVertices(),vertexC, vertexA,
																currentObject->getVelocity(), elapsedTime);*/
							
								impact = isSegmentInsideObject(otherObjectRigidBodyCube, vertexC, vertexA,currentObject->getVelocity(), elapsedTime);
								crossing = (impact.x == 1.0f? true: false);
								impactPoint = vec3(impact.y, impact.z, impact.w);
										
							}
						}

						if(crossing)
							break;
					}
				}

				if(crossing)
				{
					float resistance;
					float bounciness;

					vec3 direction;

					direction = currentObject->getCenterOfObject() - currentObjectAround->getCenterOfObject();
					//direction = impactPoint - currentObjectAround->getCenterOfObject();
					direction.Normalize();

					if(otherObjectRigidBodyCube->getIsStatic())
					{
						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						//currentObject->getComponents()->getGravity()->addForce(impactForce1/5 ,  currentObject->getVelocity(), elapsedTime);
						currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y /bounciness /* * ((bounciness+resistance)*0.001)*/, currentObject->getVelocity()->z / resistance));
					}
					else
					{
						vec3 impactForce1 = vec3(0.0f, 0.0f, 0.0f);
						vec3 impactForce2 = vec3(0.0f, 0.0f, 0.0f);
						float impactTotal;
	
						impactTotal = currentObject->getMass() * currentObject->getVelocity()->Length() +
									currentObjectAround->getMass() * currentObjectAround->getVelocity()->Length();

						float ratio = 0.0f;

						//ratio = this->listOfObject.at(i)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObjectAround->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce1.x = ratio * impactTotal * -direction.x;
						impactForce1.y = ratio * impactTotal * -direction.y; 
						impactForce1.z = ratio * impactTotal * -direction.z;

						//ratio = this->listOfObject.at(i)->getObjectsAround().at(j)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObject->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce2.x = ratio * impactTotal * direction.x;
						impactForce2.y = ratio * impactTotal * direction.y; 
						impactForce2.z = ratio * impactTotal * direction.z;

						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						currentObject->getComponents()->getGravity()->addForce(impactForce1 / ((resistance + bounciness) * 10),  currentObject->getVelocity(), elapsedTime);

						bounciness = currentObjectAround->getComponents()->getGravity()->getBounciness();
						resistance = currentObjectAround->getComponents()->getGravity()->getResistance();

						currentObjectAround->getComponents()->getGravity()->addForce(impactForce2 / ((resistance + bounciness) * 10), currentObjectAround->getVelocity(), elapsedTime);
				
					}
				}		
				break;
			//////////////////// GENERIC \\\\\\\\\\\\\\\\\\\\\

			case RIGID_GENERIC:

				RigidBody* otherObjectRigidBodyGeneric;
				otherObjectRigidBodyGeneric = currentObjectAround->getComponents()->getRigidBody();

				float size = currentObjectRigidBodyGeneric->getSize();

				distance1 = size + otherObjectRigidBodyGeneric->getSize();

				distance2 = MF.distanceBetween2points(currentObject->getCenterOfObject(), currentObjectAround->getCenterOfObject());
				crossing = false;
				if(distance1 >= (distance2 + 0.1f))
				{

					for(j = 0; j < currentObjectRigidBodyGeneric->getListIndexesFaces()->size(); ++j)
					{

						indexVertexA = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).x);
						indexVertexB = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).y);
						indexVertexC = currentObjectRigidBodyGeneric->getListOfIndexes()->at(currentObjectRigidBodyGeneric->getListIndexesFaces()->at(j).z);

						// Get the 3 points of the triangle defining the face
						vertexA = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexA);
						vertexB = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexB);
						vertexC = currentObjectRigidBodyGeneric->getListOfVertices()->at(indexVertexC);

								

						// Test the first segement of the triangle
						/*impact = isSegmentInsideObject(otherObjectRigidBodyGeneric->getListOfNormals(), otherObjectRigidBodyGeneric->getListOfCenterOfGravities(), otherObjectRigidBodyGeneric->getListOfIndexes(),
							otherObjectRigidBodyGeneric->getListIndexesFaces(), otherObjectRigidBodyGeneric->getListOfVertices(),vertexA, vertexB,
							currentObject->getVelocity(), elapsedTime);*/
					
						impact = isSegmentInsideObject(otherObjectRigidBodyGeneric, vertexA, vertexB,currentObject->getVelocity(), elapsedTime);
						crossing = (impact.x == 1.0f? true: false);
						impactPoint = vec3(impact.y, impact.z, impact.w);

						if(!crossing)
						{
							// Test the second segment of the triangle
							/*impact = isSegmentInsideObject(otherObjectRigidBodyGeneric->getListOfNormals(), otherObjectRigidBodyGeneric->getListOfCenterOfGravities(), otherObjectRigidBodyGeneric->getListOfIndexes(),
															otherObjectRigidBodyGeneric->getListIndexesFaces(), otherObjectRigidBodyGeneric->getListOfVertices(),vertexB, vertexC,
															currentObject->getVelocity(), elapsedTime);*/
						
							impact = isSegmentInsideObject(otherObjectRigidBodyGeneric, vertexB, vertexC,currentObject->getVelocity(), elapsedTime);
							crossing = (impact.x == 1.0f? true: false);
							impactPoint = vec3(impact.y, impact.z, impact.w);

							if(!crossing)
							{
								// Test the third segement of the triangle
								/*impact = isSegmentInsideObject(otherObjectRigidBodyGeneric->getListOfNormals(), otherObjectRigidBodyGeneric->getListOfCenterOfGravities(), otherObjectRigidBodyGeneric->getListOfIndexes(),
																otherObjectRigidBodyGeneric->getListIndexesFaces(), otherObjectRigidBodyGeneric->getListOfVertices(),vertexC, vertexA,
																currentObject->getVelocity(), elapsedTime);*/

								impact = isSegmentInsideObject(otherObjectRigidBodyGeneric, vertexC, vertexA,currentObject->getVelocity(), elapsedTime);
								crossing = (impact.x == 1.0f? true: false);
								impactPoint = vec3(impact.y, impact.z, impact.w);
										
							}
						}

						if(crossing)
							break;
					
					}
				}

				if(crossing)
				{
					float resistance;
					float bounciness;
					int id = currentObject->getId();

					

					vec3 direction;

					direction = currentObject->getCenterOfObject() - currentObjectAround->getCenterOfObject();
					//direction = impactPoint - currentObjectAround->getCenterOfObject();
					direction.Normalize();
					
					if(otherObjectRigidBodyGeneric->getIsStatic())
					{
						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						//currentObject->getComponents()->getGravity()->addForce(impactForce1/5 ,  currentObject->getVelocity(), elapsedTime);
						currentObject->setVelocity(vec3(currentObject->getVelocity()->x / resistance, - currentObject->getVelocity()->y /bounciness /* * ((bounciness+resistance)*0.001)*/, currentObject->getVelocity()->z / resistance));
					}
					else
					{

						vec3 impactForce1 = vec3(0.0f, 0.0f, 0.0f);
						vec3 impactForce2 = vec3(0.0f, 0.0f, 0.0f);
						float impactTotal;
						impactTotal = currentObject->getMass() * currentObject->getVelocity()->Length() +
									currentObjectAround->getMass() * currentObjectAround->getVelocity()->Length();

						float ratio = 0.0f;

						//ratio = this->listOfObject.at(i)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObjectAround->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce1.x = ratio * impactTotal * -direction.x;
						impactForce1.y = ratio * impactTotal * -direction.y; 
						impactForce1.z = ratio * impactTotal * -direction.z;

						//ratio = this->listOfObject.at(i)->getObjectsAround().at(j)->getMass() / (this->listOfObject.at(i)->getMass() + this->listOfObject.at(i)->getObjectsAround().at(j)->getMass());
						ratio = currentObject->getMass() / (currentObject->getMass() + currentObjectAround->getMass());
						impactForce2.x = ratio * impactTotal * direction.x;
						impactForce2.y = ratio * impactTotal * direction.y; 
						impactForce2.z = ratio * impactTotal * direction.z;

					

						bounciness = currentObject->getComponents()->getGravity()->getBounciness();
						resistance = currentObject->getComponents()->getGravity()->getResistance();
								
						currentObject->getComponents()->getGravity()->addForce(impactForce1 / ((resistance + bounciness) * 1),  currentObject->getVelocity(), elapsedTime);

						//if(currentObject->getComponents()
						bounciness = currentObjectAround->getComponents()->getGravity()->getBounciness();
						resistance = currentObjectAround->getComponents()->getGravity()->getResistance();

						currentObjectAround->getComponents()->getGravity()->addForce(impactForce2 / ((resistance + bounciness) * 1), currentObjectAround->getVelocity(), elapsedTime);
				
					}
				}		
				break;
		}
	}
	reorientObject(currentObject, elapsedTime);
}

void ObjectManager::reorientObject(GameObject* object, float elapsedTime)
{

	float threshold = 0.1f;
	RigidBody* objectRigidBody;
	objectRigidBody = object->getComponents()->getRigidBody();

	vec4 collidingFace;
	int collidingFaceIndex;
	int lowestvertex;
	vec3 positionOfIntersection;

	lowestvertex = objectRigidBody->getLowestVertex();

	collidingFace = objectRigidBody->getCollidingFace();
	collidingFaceIndex = collidingFace.x;
	positionOfIntersection = vec3(collidingFace.y, collidingFace.z, collidingFace.w);

	if(collidingFaceIndex != -1)
	{
		vec3 gap = objectRigidBody->getListOfNormals()->at(collidingFaceIndex)- vec3(0,-1,0);

		gap.x = abs(gap.x);
		gap.y = abs(gap.y);
		gap.z = abs(gap.z);

		if(gap.x >= 0.01f || gap.y >= 0.01f || gap.z >= 0.01f)
		{
			//cout << "gap" <<gap.x << " ,  " << gap.y << " ,  " << gap.z <<  endl;
			unsigned int indexVertexA;
			unsigned int indexVertexB;
			unsigned int indexVertexC;

			vec3 vertexA;
			vec3 vertexB;
			vec3 vertexC;

			indexVertexA = objectRigidBody->getListOfIndexes()->at(objectRigidBody->getListIndexesFaces()->at(collidingFaceIndex).x);
			indexVertexB = objectRigidBody->getListOfIndexes()->at(objectRigidBody->getListIndexesFaces()->at(collidingFaceIndex).y);
			indexVertexC = objectRigidBody->getListOfIndexes()->at(objectRigidBody->getListIndexesFaces()->at(collidingFaceIndex).z);

			// Get the 3 points of the triangle defining the face
			vertexA = objectRigidBody->getListOfVertices()->at(indexVertexA);
			vertexB = objectRigidBody->getListOfVertices()->at(indexVertexB);
			vertexC = objectRigidBody->getListOfVertices()->at(indexVertexC);

			float distance1;
			float distance2;

		

			if(indexVertexA == lowestvertex)
			{

				distance1 = abs(vertexA.y - vertexB.y);
				distance2 = abs(vertexA.y - vertexC.y);

				if((distance1 < distance2 || distance2 < threshold) && distance1 > threshold)
				{
					vec3 direction = MF.getVector(vertexB, vertexA);// * object->getMass() * elapsedTime;
					object->rotate(direction, elapsedTime);

				}
				else
				{
					if(distance2 > threshold)
					{
						vec3 direction = MF.getVector(vertexB, vertexA);// * object->getMass() * elapsedTime;
						object->rotate(direction, elapsedTime);

					}
				}
			}

			if(indexVertexB == lowestvertex)
			{

				distance1 = abs(vertexB.y - vertexA.y);
				distance2 = abs(vertexB.y - vertexC.y);

				if((distance1 < distance2 || distance2 < threshold) && distance1 > threshold)
				{
					vec3 direction = MF.getVector(vertexA, vertexB);// * object->getMass() * elapsedTime;
					object->rotate(direction, elapsedTime);

				}
				else
				{
					if(distance2 > threshold)
					{
						vec3 direction = MF.getVector(vertexC, vertexB);// * object->getMass() * elapsedTime;
						object->rotate(direction, elapsedTime);

					}
				}
			}

			if(indexVertexC == lowestvertex)
			{
				distance1 = abs(vertexC.y - vertexB.y);
				distance2 = abs(vertexC.y - vertexA.y);

				if((distance1 < distance2 || distance2 < threshold) && distance1 > threshold)
				{
					vec3 direction = MF.getVector(vertexB, vertexC);// * object->getMass() * elapsedTime;
					object->rotate(direction, elapsedTime);
				}
				else
				{
					if(distance2 > threshold)
					{
						vec3 direction = MF.getVector(vertexA, vertexC);// * object->getMass() * elapsedTime;
						object->rotate(direction, elapsedTime);
					}
				}
			}
		}
	}

}

GameObject* ObjectManager::getGameObjectById(int id)
{
	for(unsigned int i = 0; i < this->listOfObject.size() ; ++i)
	{
		if(this->listOfObject.at(i)->getId() == id)
		{
			return this->listOfObject.at(i);
		}
	}

	return NULL;
}

ObjectManager::~ObjectManager(void)
{
}
