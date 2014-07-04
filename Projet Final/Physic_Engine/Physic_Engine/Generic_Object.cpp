#include "Generic_Object.h"


Generic_Object::Generic_Object(void):GameObject(1.0f, false)
{
	this->wireframeMode = false;

	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	this->mass = MF.getVolumeOfACube(1.0f);

	this->size = 1.0f;

	this->verticesList = new vector<vec3>();
	this->indexesList = new vector<GLushort>();
	this->listEdges = new vector<Edge*>();
	this->listFaces = new vector<Face*>();

	this->listIndexesWireframe = new vector<unsigned short>();
	this->listIndexesPoint = new vector<unsigned short>();
}

Generic_Object::Generic_Object(vector<vec3> &verticesList, vector<GLushort> &indexesList, float posX, float posY, float posZ, bool worldPosition)
{
	this->verticesList = &verticesList;
	this->indexesList = &indexesList;

	this->listEdges = new vector<Edge*>();
	this->listFaces = new vector<Face*>();

	this->listIndexesWireframe = new vector<unsigned short>();
	this->listIndexesPoint = new vector<unsigned short>();

	this->wireframeMode = false;

	this->color = vec4(0.5f, 0.5f, 0.5f, 1.0f);

	vec3 highestPoint = verticesList.at(0);
	vec3 lowestPoint = verticesList.at(0);
	vec3 leftPoint = verticesList.at(0);
	vec3 rightPoint = verticesList.at(0);
	vec3 frontPoint = verticesList.at(0);
	vec3 backPoint = verticesList.at(0);

	for(int i = 1; i < this->verticesList->size(); ++i)
	{
		if(this->verticesList->at(i).y > highestPoint.y)
			highestPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).y < lowestPoint.y)
			lowestPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).x > rightPoint.x)
			rightPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).x < leftPoint.x)
			leftPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).z > frontPoint.z)
			frontPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).z < backPoint.z)
			backPoint = this->verticesList->at(i);
	}

	this->centerOfObject = vec3((rightPoint.x + leftPoint.x)/2.0f, (highestPoint.y + lowestPoint.y)/2.0f, (frontPoint.z + backPoint.z)/2.0f);
	
	this->size = MF.distanceBetween2points(highestPoint, centerOfObject);

	this->mass = MF.getVolumeOfACube(size);

	if(worldPosition)
		changePosition(-this->centerOfObject.x, -this->centerOfObject.y, -this->centerOfObject.z);

	changePosition(posX, posY, posZ);
}

void Generic_Object::setColor(float r, float g, float b, float a)
{
	this->color = vec4(r,g,b,a);
}

void Generic_Object::setWireframeMode(bool active)
{
	this->wireframeMode = active;
}

void Generic_Object::render(GLint *positionAttrib, GLint *colorUniform, GLint *texture_uniform)
{
	if(alternateFacesAndNormals)
	{
		glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &verticesList->at(0).x);
		glUniform4f(*colorUniform,color.x, color.y, color.z, color.w);
		glUniform1f(*texture_uniform, this->useTexture);
		glDrawElements(GL_TRIANGLE_STRIP,this->indexesList->size(),GL_UNSIGNED_SHORT,&this->indexesList->at(0));

		if(this->wireframeMode)
		{
			glEnable( GL_POLYGON_OFFSET_FILL );
			glPolygonOffset( 1.0, 1.0 );
			glUniform4f(*colorUniform,0.f, 0.f, 0.f, 1.0f);
			glUniform1f(*texture_uniform, 0.0f);
			glDrawElements(GL_LINE_LOOP,this->indexesList->size(),GL_UNSIGNED_SHORT,&this->indexesList->at(0));
		}
	}
	else
	{
		glVertexAttribPointer(*positionAttrib, 3, GL_FLOAT, false, 0, &this->verticesList->at(0).x);
		glUniform4f(*colorUniform,color.x, color.y, color.z, color.w);
		glUniform1f(*texture_uniform, this->useTexture);
		glDrawElements(GL_TRIANGLES, this->indexesList->size(), GL_UNSIGNED_SHORT, &this->indexesList->at(0));

		if(this->wireframeMode)
		{
			glEnable( GL_POLYGON_OFFSET_FILL );
			glPolygonOffset( 1.0, 1.0 );
			glUniform4f(*colorUniform,0.f, 0.f, 0.f, 1.0f);
			glUniform1f(*texture_uniform, 0.0f);
			glDrawElements(GL_LINES,this->listIndexesWireframe->size(),GL_UNSIGNED_SHORT,&this->listIndexesWireframe->at(0));
		}
		/*glDrawElements(GL_LINES, this->listIndexesWireframe->size(), GL_UNSIGNED_SHORT, &this->listIndexesWireframe->at(0));
		glUniform4f(*colorUniform,  0.f, 0.f, 1.f, 1.f);*/
		//glDrawElements(GL_POINTS, this->listIndexesPoint->size(), GL_UNSIGNED_SHORT, &this->listIndexesPoint->at(0));
	}

	if(this->displayAxesBool)
		displayAxes(positionAttrib,colorUniform);
}

void Generic_Object::update(float elapsedTime)
{
	changePosition(-1 * (this->velocity.x * elapsedTime), -1 * (this->velocity.y * elapsedTime), -1 * (this->velocity.z * elapsedTime));
	vec3 velocity;

	velocity = MF.roundf(*getVelocity(), 1);

	if(velocity.x != 0.0f || velocity.z != 0.0f)
		this->rotate(velocity, elapsedTime);
}

void Generic_Object::changePosition(float posX, float posY, float posZ)
{
	unsigned int i;

	this->centerOfObject.x += posX;
	this->centerOfObject.y += posY;
	this->centerOfObject.z += posZ;


	for(i = 0; i < this->verticesList->size(); ++i)
	{
		this->verticesList->at(i).x += posX;
		this->verticesList->at(i).y += posY;
		this->verticesList->at(i).z += posZ;
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

void Generic_Object::rotate(vec3 velocity, float elapsedTime)
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
			this->verticesList->at(i) -= this->centerOfObject;
			result = this->components.getQuaternion()->multiplyQuaternion4f(q, vec4(0,this->verticesList->at(i).x, this->verticesList->at(i).y, this->verticesList->at(i).z));
			this->verticesList->at(i) = this->components.getQuaternion()->multiplyQuaternion3f(result, qPrime);
			this->verticesList->at(i) += this->centerOfObject;
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

vector<vec3>* Generic_Object::getListOfVertices()
{
	return this->verticesList;
}

float Generic_Object::getSize()
{
	return this->size;
}

vector<GLushort>* Generic_Object::getListOfIndexes()
{
	return this->indexesList;
}

void Generic_Object::CalculateIndexes()
{
	this->listIndexesWireframe->clear();
	this->indexesList->clear();
	for (unsigned int i = 0; i < this->verticesList->size(); ++i)
	{
		this->listIndexesPoint->push_back(i);
	}
	for (unsigned int i = 0; i < this->listEdges->size(); ++i)
	{
		Edge * edge = this->listEdges->at(i);
		this->listIndexesWireframe->push_back(edge->indexVertex1);
		this->listIndexesWireframe->push_back(edge->indexVertex2);
	}
	for (unsigned int i = 0; i < this->listFaces->size(); ++i)
	{
		Face * face = this->listFaces->at(i);
		Edge * edge1 = face->edge1;
		Edge * edge2 = face->edge2;
		unsigned short index0 = edge1->indexVertex1, index1 = edge1->indexVertex2;
		if (!face->normalInverse)
		{
			this->indexesList->push_back(index0);
			this->indexesList->push_back(index1);
		}
		else
		{
			this->indexesList->push_back(index1);
			this->indexesList->push_back(index0);
		}
		if (index0 != edge2->indexVertex1 && index1 != edge2->indexVertex1)
			this->indexesList->push_back(edge2->indexVertex1);
		else
			this->indexesList->push_back(edge2->indexVertex2);
	}
}

void Generic_Object::Scale(float valueScale)
{
	mat4 matrixScale;
	matrixScale.IdentityScale(valueScale);

	for (unsigned int i = 0; i < this->verticesList->size(); ++i)
	{
		// Scale
		vec4 newVertex = (matrixScale * vec4(this->verticesList->at(i), 0));
		this->verticesList->at(i).x = newVertex.x;
		this->verticesList->at(i).y = newVertex.y;
		this->verticesList->at(i).z = newVertex.z;
	}
}

vector<unsigned short> * Generic_Object::RemoveVertexAround(vec3 positionImpact, float radius)
{
	std::vector<vec3> listVectorNeighbour;
	float powRadius = radius * radius;
	
	for (int i = 0; i < this->verticesList->size(); ++i)
	{
		vec3 vertex = this->verticesList->at(i);
		float size = (positionImpact - vertex).Dot(positionImpact - vertex);
		if (size < powRadius)
		{
			// Remove this vertex, edges and faces
			for (int j = 0; j < this->listEdges->size(); ++j)
			{
				Edge * edge = this->listEdges->at(j);
				if (edge->indexVertex1 == i || edge->indexVertex2 == i)
				{
					for (int k = 0; k < this->listFaces->size(); ++k)
					{
						Face * face = this->listFaces->at(k);
						if (face->edge1 == edge || face->edge2 == edge || face->edge3 == edge)
						{
							this->listFaces->erase(this->listFaces->begin() + k);
							this->indexesList->erase(this->indexesList->begin() + k, this->indexesList->begin() + k + 3);
							--k;
						}
					}
					if (edge->indexVertex1 == i)
						listVectorNeighbour.push_back(this->verticesList->at(edge->indexVertex2));
					else
						listVectorNeighbour.push_back(this->verticesList->at(edge->indexVertex1));
					this->listEdges->erase(this->listEdges->begin() + j);
					this->listIndexesWireframe->erase(this->listIndexesWireframe->begin() + j, this->listIndexesWireframe->begin() + j + 1);
					--j;
				}
				else
				{
					if (edge->indexVertex1 > i)
						--edge->indexVertex1;
					if (edge->indexVertex2 > i)
						--edge->indexVertex2;
				}
			}
			this->listIndexesPoint->erase(this->listIndexesPoint->begin() + i);
			this->verticesList->erase(this->verticesList->begin() + i);
			--i;
		}
	}
	
	// Get only the new index
	std::vector<unsigned short> * listIndexNeighbour = new std::vector<unsigned short>();
	for (unsigned int i = 0; i < this->verticesList->size(); ++i)
	{
		vec3 vertex = this->verticesList->at(i);
		for (unsigned int j = 0; j < listVectorNeighbour.size(); ++j)
		{
			if (listVectorNeighbour.at(j) == vertex)
			{
				listIndexNeighbour->push_back(i);
				break;
			}
		}
	}
	return listIndexNeighbour;
}

void Generic_Object::Merge(vec3 positionImpact, vec3 positionTranslate, Generic_Object * go, std::vector<unsigned short> * listVerticesConvexPolygon, std::vector<unsigned short> * listIndexNeighbour)
{
	mat4 matrixRotation = esgiLookAt(positionImpact, vec3(0.f, 30.f, 1.0f), vec3(0, -1, 0));

	// Add the vertex, edge and faces already calculate
	unsigned int numberVertices = this->verticesList->size();
	this->verticesList->insert(this->verticesList->end(), go->verticesList->begin(), go->verticesList->end());
	this->listIndexesPoint->insert(this->listIndexesPoint->end(), go->listIndexesPoint->begin(), go->listIndexesPoint->end());
	for (unsigned int i = 0; i < go->listEdges->size(); ++i)
	{
		Edge * edge = go->listEdges->at(i);
		edge->indexVertex1 += numberVertices;
		edge->indexVertex2 += numberVertices;
		this->listEdges->push_back(edge);
		this->listIndexesWireframe->push_back(go->listIndexesWireframe->at(i * 2) + numberVertices);
		this->listIndexesWireframe->push_back(go->listIndexesWireframe->at(i * 2 + 1) + numberVertices);
	}
	this->listFaces->insert(this->listFaces->end(), go->listFaces->begin(), go->listFaces->end());
	for (unsigned int i = 0; i < go->indexesList->size(); ++i)
	{
		this->indexesList->push_back(go->indexesList->at(i) + numberVertices);
	}


	// Calculate the new edge, face
	std::vector<Edge*> listNewEdges;
	std::vector<unsigned short> listVertexUse;
	for (unsigned int i = 0; i < listIndexNeighbour->size(); ++i)
	{
		unsigned short currentIndex = listIndexNeighbour->at(i);
		vec3 currentVertex = this->verticesList->at(currentIndex);
		int closestVertices = -1;
		float closestDistance = 1000000;

		for (unsigned int j = 0; j < listVerticesConvexPolygon->size(); ++j)
		{
			unsigned short tempIndex = listVerticesConvexPolygon->at(j);
			vec3 tempVertex = go->verticesList->at(tempIndex);
			//vec3 tempVertex = matrixRotation * (go->verticesList->at(tempIndex) - positionTranslate);
			float size = (currentVertex - tempVertex).Length();
			if (size < closestDistance)
			{
				closestVertices = tempIndex;
				closestDistance = size;
			}
		}
		if (closestVertices != -1)
		{
			Edge * edge = new Edge(currentIndex, closestVertices + numberVertices);
			this->listEdges->push_back(edge);
			listNewEdges.push_back(edge);
			this->listIndexesWireframe->push_back(currentIndex);
			this->listIndexesWireframe->push_back(closestVertices + numberVertices);
			listVertexUse.push_back(closestVertices + numberVertices);
		}
	}

	// Add the external face
	for (int i = 0; i < listVerticesConvexPolygon->size(); ++i)
	{
		unsigned short index = listVerticesConvexPolygon->at(i) + numberVertices;
		bool findIndex = false;
		for (unsigned int j = 0; j < listVertexUse.size(); ++j)
		{
			if (listVertexUse[j] == index)
			{
				findIndex = true;
				break;
			}
		}
		if (!findIndex)
		{
			vec3 vertex0 = this->verticesList->at(index);
			//vec3 vertex0 = matrixRotation * (this->verticesList->at(index) - positionTranslate);
			int closestVertices = -1;
			float closestDistance = 1000000;
			for (unsigned int j = 0; j < listIndexNeighbour->size(); ++j)
			{
				unsigned short tempIndex = listIndexNeighbour->at(j);
				vec3 vertex1 = this->verticesList->at(tempIndex);
				float dist = (vertex0 - vertex1).Length();
				if (dist < closestDistance)
				{
					closestDistance = dist;
					closestVertices = tempIndex;
				}
			}
			if (closestVertices != -1)
			{
				Edge * edge = new Edge(closestVertices, index);
				this->listEdges->push_back(edge);
				listNewEdges.push_back(edge);
				this->listIndexesWireframe->push_back(closestVertices);
				this->listIndexesWireframe->push_back(index);
			}
		}
	}

	// Add the edge of jonction and create the faces
	for (unsigned int i = 0; i < listVerticesConvexPolygon->size(); ++i)
	{
		unsigned short currentIndex = listVerticesConvexPolygon->at(i) + numberVertices;
		unsigned short nextIndex = listVerticesConvexPolygon->at(0) + numberVertices;
		if (i + 1 < listVerticesConvexPolygon->size())
			nextIndex = listVerticesConvexPolygon->at(i + 1) + numberVertices;
		vec3 currentVertex = this->verticesList->at(currentIndex);
		vec3 nextVertex = this->verticesList->at(nextIndex);
		//vec3 currentVertex = matrixRotation * (this->verticesList->at(currentIndex) - positionTranslate);
		//vec3 nextVertex = matrixRotation * (this->verticesList->at(nextIndex) - positionTranslate);

		for (unsigned int j = 0; j < listNewEdges.size(); ++j)
		{
			Edge * tempEdge = listNewEdges[j];
			if (tempEdge->indexVertex2 == currentIndex)
			{
				// Creation of the triangle face
				for (unsigned int k = j + 1; k < listNewEdges.size(); ++k)
				{
					Edge * tempEdge1 = listNewEdges[k];
					if (tempEdge1->indexVertex2 == currentIndex)
					{
						for (unsigned int l = 0; l < this->listEdges->size(); ++l)
						{
							Edge * tempEdge2 = this->listEdges->at(l);
							if ((tempEdge2->indexVertex1 == tempEdge->indexVertex1 && tempEdge2->indexVertex2 == tempEdge1->indexVertex1) ||
								(tempEdge2->indexVertex2 == tempEdge->indexVertex1 && tempEdge2->indexVertex1 == tempEdge1->indexVertex1))
							{
								Face * face = new Face(tempEdge, tempEdge1, tempEdge2);
								face->VerifyFace(this->verticesList, vec3(0.0f, 1.0f, 0.0f));
								this->listFaces->push_back(face);
								
								// Add the indexes
								unsigned short index0 = tempEdge->indexVertex1, index1 = tempEdge->indexVertex2;
								if (!face->normalInverse)
								{
									this->indexesList->push_back(index0);
									this->indexesList->push_back(index1);
								}
								else
								{
									this->indexesList->push_back(index1);
									this->indexesList->push_back(index0);
								}
								if (index0 != tempEdge1->indexVertex1 && index1 != tempEdge1->indexVertex1)
									this->indexesList->push_back(tempEdge1->indexVertex1);
								else
									this->indexesList->push_back(tempEdge1->indexVertex2);
							}
						}
					}
				}

				// Creation of the triangle face
				for (unsigned int k = 0; k < listNewEdges.size(); ++k)
				{
					Edge * tempEdge1 = listNewEdges[k];
					if (tempEdge1->indexVertex2 == nextIndex && tempEdge1->indexVertex1 == tempEdge->indexVertex1)
					{
						for (unsigned int l = 0; l < this->listEdges->size(); ++l)
						{
							Edge * tempEdge2 = this->listEdges->at(l);
							if ((tempEdge2->indexVertex1 == currentIndex && tempEdge2->indexVertex2 == nextIndex) ||
								(tempEdge2->indexVertex2 == currentIndex && tempEdge2->indexVertex1 == nextIndex))
							{
								Face * face = new Face(tempEdge, tempEdge1, tempEdge2);
								face->VerifyFace(this->verticesList, vec3(0.0f, 1.0f, 0.0f));
								this->listFaces->push_back(face);
								
								// Add the indexes
								unsigned short index0 = tempEdge->indexVertex1, index1 = tempEdge->indexVertex2;
								if (!face->normalInverse)
								{
									this->indexesList->push_back(index0);
									this->indexesList->push_back(index1);
								}
								else
								{
									this->indexesList->push_back(index1);
									this->indexesList->push_back(index0);
								}
								if (index0 != tempEdge1->indexVertex1 && index1 != tempEdge1->indexVertex1)
									this->indexesList->push_back(tempEdge1->indexVertex1);
								else
									this->indexesList->push_back(tempEdge1->indexVertex2);
							}
						}
					}
				}

				// Creation of the specialize face
				vec3 currentVertexNeighbour = this->verticesList->at(tempEdge->indexVertex1);
				for (unsigned int k = 0; k < listNewEdges.size(); ++k)
				{
					Edge * tempEdge1 = listNewEdges[k];
					if (tempEdge1->indexVertex2 == nextIndex)
					{
						vec3 nextVertexNeighbour = this->verticesList->at(tempEdge1->indexVertex1);
						for (unsigned int l = 0; l < this->listEdges->size(); ++l)
						{
							Edge * tempEdge2 = this->listEdges->at(l);
							if ((tempEdge2->indexVertex1 == tempEdge->indexVertex1 && tempEdge2->indexVertex2 == tempEdge1->indexVertex1) ||
								(tempEdge2->indexVertex2 == tempEdge->indexVertex1 && tempEdge2->indexVertex1 == tempEdge1->indexVertex1))
							{
								// Create the new Edge
								float size1 = (currentVertex - nextVertexNeighbour).Length();
								float size2 = (currentVertexNeighbour - nextVertex).Length();
								Edge * edge;
								if(size1 < size2)
								{
									edge = new Edge(currentIndex, tempEdge1->indexVertex1);
									this->listEdges->push_back(edge);
									this->listIndexesWireframe->push_back(currentIndex);
									this->listIndexesWireframe->push_back(tempEdge1->indexVertex1);
								}
								else
								{
									edge = new Edge(tempEdge->indexVertex1, nextIndex);
									this->listEdges->push_back(edge);
									this->listIndexesWireframe->push_back(tempEdge->indexVertex1);
									this->listIndexesWireframe->push_back(nextIndex);

									// Change the edge
									Edge * temp = tempEdge;
									tempEdge = tempEdge1;
									tempEdge1 = temp;
								}

								// Create the first face
								Face * face1 = new Face(tempEdge, tempEdge2, edge);
								face1->VerifyFace(this->verticesList, vec3(0.0f, 1.0f, 0.0f));
								this->listFaces->push_back(face1);

								// Add the indexes
								unsigned short index0 = tempEdge->indexVertex1, index1 = tempEdge->indexVertex2;
								if (!face1->normalInverse)
								{
									this->indexesList->push_back(index0);
									this->indexesList->push_back(index1);
								}
								else
								{
									this->indexesList->push_back(index1);
									this->indexesList->push_back(index0);
								}
								if (index0 != tempEdge2->indexVertex1 && index1 != tempEdge2->indexVertex1)
									this->indexesList->push_back(tempEdge2->indexVertex1);
								else
									this->indexesList->push_back(tempEdge2->indexVertex2);
								
								// Create the second face
								Edge * tempEdge3 = NULL;
								for (unsigned int m = 0; m < this->listEdges->size(); ++m)
								{
									tempEdge3 = this->listEdges->at(m);
									if ((tempEdge3->indexVertex1 == currentIndex && tempEdge3->indexVertex2 == nextIndex) ||
										(tempEdge3->indexVertex2 == currentIndex && tempEdge3->indexVertex1 == nextIndex))
										break;
								}
								Face * face2 = new Face(tempEdge1, tempEdge3, edge);
								face2->VerifyFace(this->verticesList, vec3(0.0f, 1.0f, 0.0f));
								this->listFaces->push_back(face2);
								
								// Add the indexes
								index0 = tempEdge1->indexVertex1;
								index1 = tempEdge1->indexVertex2;
								if (!face2->normalInverse)
								{
									this->indexesList->push_back(index0);
									this->indexesList->push_back(index1);
								}
								else
								{
									this->indexesList->push_back(index1);
									this->indexesList->push_back(index0);
								}
								if (index0 != tempEdge3->indexVertex1 && index1 != tempEdge3->indexVertex1)
									this->indexesList->push_back(tempEdge3->indexVertex1);
								else
									this->indexesList->push_back(tempEdge3->indexVertex2);

								if(size1 >= size2)
								{
									// Revert the edge changement
									Edge * temp = tempEdge;
									tempEdge = tempEdge1;
									tempEdge1 = temp;
								}
							}
						}
					}
				}
			}
		}
	}
}

void Generic_Object::TranslateAndLookAtAndScale(const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float valueScale)
{
	mat4 matrixRotation = esgiLookAt(positionCenterRotation, positionLookAt, vec3(0, -1, 0));
	mat4 matrixScale;
	matrixScale.IdentityScale(valueScale);
	mat4 viewMatrix = esgiMultiplyMatrix(matrixRotation, matrixScale);

	for (unsigned int i = 0; i < this->verticesList->size(); ++i)
	{
		// Rotation + Translation + Scale
		vec4 newVertex = viewMatrix * vec4(this->verticesList->at(i), 0);
		this->verticesList->at(i).x = newVertex.x + positionTranlate.x;
		this->verticesList->at(i).y = newVertex.y + positionTranlate.y;
		this->verticesList->at(i).z = newVertex.z + positionTranlate.z;
	}
}

void Generic_Object::calculateCenterOfObject()
{
	vec3 highestPoint = this->verticesList->at(0);
	vec3 lowestPoint = this->verticesList->at(0);
	vec3 leftPoint = this->verticesList->at(0);
	vec3 rightPoint = this->verticesList->at(0);
	vec3 frontPoint = this->verticesList->at(0);
	vec3 backPoint = this->verticesList->at(0);

	for(int i = 1; i < this->verticesList->size(); ++i)
	{
		if(this->verticesList->at(i).y > highestPoint.y)
			highestPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).y < lowestPoint.y)
			lowestPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).x > rightPoint.x)
			rightPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).x < leftPoint.x)
			leftPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).z > frontPoint.z)
			frontPoint = this->verticesList->at(i);

		if(this->verticesList->at(i).z < backPoint.z)
			backPoint = this->verticesList->at(i);
	}

	this->centerOfObject = vec3((rightPoint.x + leftPoint.x)/2.0f, (highestPoint.y + lowestPoint.y)/2.0f, (frontPoint.z + backPoint.z)/2.0f);
	
	this->size = MF.distanceBetween2points(highestPoint, centerOfObject);

	this->mass = MF.getVolumeOfACube(size);
}

void Generic_Object::clearLists()
{
	this->indexesList->clear();
	this->verticesList->clear();
	this->listIndexesWireframe->clear();
	this->listIndexesPoint->clear();

	for(int i = 0; i < this->listEdges->size(); ++i)
		delete this->listEdges->at(i);

	this->listEdges->clear();

	for(int i = 0; i < this->listFaces->size(); ++i)
		delete this->listFaces->at(i);

	this->listFaces->clear();
}

void Generic_Object::deleteLists()
{
	this->indexesList->clear();
	this->verticesList->clear();
	this->listIndexesWireframe->clear();
	this->listIndexesPoint->clear();

	for(int i = 0; i < this->listEdges->size(); ++i)
		delete this->listEdges->at(i);

	delete this->listEdges;

	for(int i = 0; i < this->listFaces->size(); ++i)
		delete this->listFaces->at(i);

	delete this->listFaces;
}

Generic_Object::~Generic_Object(void)
{
	deleteLists();
}
