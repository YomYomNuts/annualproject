#include "GameObject.h"

#include "../../EsgiGL/Common/matrix.h"
#include "DelaunayGenerator.h"

#include <map>

GameObject::GameObject(void)
{
	this->listVertices = new std::vector<vec3>();
	this->listEdges = new std::vector<Edge*>();
	this->listFaces = new std::vector<Face*>();
	this->listIndexes = new std::vector<unsigned short>();
	this->listIndexesWireframe = new std::vector<unsigned short>();
	this->listIndexesPoint = new std::vector<unsigned short>();
	this->color = new vec4(0.8f, 0.8f, 0.8f, 1.0f);
}

GameObject::GameObject(const GameObject * go)
{
	this->listVertices = new std::vector<vec3>(go->listVertices->begin(), go->listVertices->end());
	this->listEdges = new std::vector<Edge*>();
	for (unsigned int i = 0; i < go->listEdges->size(); ++i)
	{
		this->listEdges->push_back(new Edge(go->listEdges->at(i)));
	}
	this->listFaces = new std::vector<Face*>();
	for (unsigned int i = 0; i < go->listFaces->size(); ++i)
	{
		this->listFaces->push_back(new Face(go->listFaces->at(i)));
	}
	this->listIndexes = new std::vector<unsigned short>(go->listIndexes->begin(), go->listIndexes->end());
	this->listIndexesWireframe = new std::vector<unsigned short>(go->listIndexesWireframe->begin(), go->listIndexesWireframe->end());
	this->listIndexesPoint = new std::vector<unsigned short>(go->listIndexesPoint->begin(), go->listIndexesPoint->end());
	this->color = new vec4(go->color->x, go->color->y, go->color->z, go->color->w);
}

GameObject::~GameObject(void)
{
	delete this->listVertices;
	delete this->listEdges;
	delete this->listFaces;
	delete this->listIndexes;
	delete this->listIndexesWireframe;
	delete this->listIndexesPoint;
	delete this->color;
}

void GameObject::TranslateAndLookAtAndScale(const vec3 & positionCenterRotation, const vec3 & positionLookAt, const vec3 & positionTranlate, float valueScale)
{
	mat4 matrixRotation = esgiLookAt(positionCenterRotation, positionLookAt, vec3(0, -1, 0));
	mat4 matrixScale;
	matrixScale.IdentityScale(valueScale);
	mat4 viewMatrix = esgiMultiplyMatrix(matrixRotation, matrixScale);

	for (unsigned int i = 0; i < this->listVertices->size(); ++i)
	{
		// Rotation + Translation + Scale
		vec4 newVertex = viewMatrix * vec4(this->listVertices->at(i), 0);
		this->listVertices->at(i).x = newVertex.x + positionTranlate.x;
		this->listVertices->at(i).y = newVertex.y + positionTranlate.y;
		this->listVertices->at(i).z = newVertex.z + positionTranlate.z;
	}
}

void GameObject::Scale(float valueScale)
{
	mat4 matrixScale;
	matrixScale.IdentityScale(valueScale);

	for (unsigned int i = 0; i < this->listVertices->size(); ++i)
	{
		// Scale
		vec4 newVertex = (matrixScale * vec4(this->listVertices->at(i), 0));
		this->listVertices->at(i).x = newVertex.x;
		this->listVertices->at(i).y = newVertex.y;
		this->listVertices->at(i).z = newVertex.z;
	}
}

void GameObject::Move(vec3 positionToAdd)
{
	for (unsigned int i = 0; i < this->listVertices->size(); ++i)
	{
		this->listVertices->at(i) += positionToAdd;
	}
}

std::vector<unsigned short> * GameObject::RemoveVertexAround(vec3 positionImpact, float radius)
{
	std::vector<vec3> listVectorNeighbour;
	float powRadius = radius * radius;
	
	for (int i = 0; i < this->listVertices->size(); ++i)
	{
		vec3 vertex = this->listVertices->at(i);
		float size = (positionImpact - vertex).Length();
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
							this->listIndexes->erase(this->listIndexes->begin() + k, this->listIndexes->begin() + k + 3);
							--k;
						}
					}
					if (edge->indexVertex1 == i)
						listVectorNeighbour.push_back(this->listVertices->at(edge->indexVertex2));
					else
						listVectorNeighbour.push_back(this->listVertices->at(edge->indexVertex1));
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
			this->listVertices->erase(this->listVertices->begin() + i);
			--i;
		}
	}
	
	// Get only the new index
	std::vector<unsigned short> * listIndexNeighbour = new std::vector<unsigned short>();
	for (unsigned int i = 0; i < this->listVertices->size(); ++i)
	{
		vec3 vertex = this->listVertices->at(i);
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

void GameObject::Merge(vec3 positionImpact, GameObject * go, std::vector<unsigned short> * listVerticesConvexPolygon, std::vector<unsigned short> * listIndexNeighbour)
{
	// Add the vertex, edge and faces already calculate
	unsigned int numberVertices = this->listVertices->size();
	this->listVertices->insert(this->listVertices->end(), go->listVertices->begin(), go->listVertices->end());
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
	for (unsigned int i = 0; i < go->listIndexes->size(); ++i)
	{
		this->listIndexes->push_back(go->listIndexes->at(i) + numberVertices);
	}

	// Calculate the new edge, face
	std::vector<Edge*> listNewEdges;
	for (unsigned int i = 0; i < listIndexNeighbour->size(); ++i)
	{
		unsigned short currentIndex = listIndexNeighbour->at(i);
		vec3 currentVertex = this->listVertices->at(currentIndex);
		int closestVertices = -1;
		float closestDistance = 1000000;

		for (unsigned int j = 0; j < listVerticesConvexPolygon->size(); ++j)
		{
			unsigned short tempIndex = listVerticesConvexPolygon->at(j);
			vec3 tempVertex = go->listVertices->at(tempIndex);
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
		}
	}

	// Add the edge of jonction and create the faces
	std::vector<Face*> listNewFaces;
	for (unsigned int i = 0; i < listVerticesConvexPolygon->size(); ++i)
	{
		unsigned short currentIndex = listVerticesConvexPolygon->at(i) + numberVertices;
		unsigned short nextIndex = listVerticesConvexPolygon->at(0) + numberVertices;
		if (i + 1 < listVerticesConvexPolygon->size())
			nextIndex = listVerticesConvexPolygon->at(i + 1) + numberVertices;
		vec3 currentVertex = this->listVertices->at(currentIndex);
		vec3 nextVertex = this->listVertices->at(nextIndex);

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
								face->VerifyFace(this->listVertices, vec3(0.0f, 1.0f, 0.0f));
								listNewFaces.push_back(face);
								this->listFaces->push_back(face);
								
								// Add the indixes
								unsigned short index0 = tempEdge->indexVertex1, index1 = tempEdge->indexVertex2;
								if (!face->normalInverse)
								{
									this->listIndexes->push_back(index0);
									this->listIndexes->push_back(index1);
								}
								else
								{
									this->listIndexes->push_back(index1);
									this->listIndexes->push_back(index0);
								}
								if (index0 != tempEdge1->indexVertex1 && index1 != tempEdge1->indexVertex1)
									this->listIndexes->push_back(tempEdge1->indexVertex1);
								else
									this->listIndexes->push_back(tempEdge1->indexVertex2);
							}
						}
					}
				}

				// Creation of the specialize face
				vec3 currentVertexNeighbour = this->listVertices->at(tempEdge->indexVertex1);
				for (unsigned int k = 0; k < listNewEdges.size(); ++k)
				{
					Edge * tempEdge1 = listNewEdges[k];
					if (tempEdge1->indexVertex2 == nextIndex)
					{
						vec3 nextVertexNeighbour = this->listVertices->at(tempEdge1->indexVertex1);
						for (unsigned int l = 0; l < this->listEdges->size(); ++l)
						{
							Edge * tempEdge2 = this->listEdges->at(l);
							if ((tempEdge2->indexVertex1 == tempEdge->indexVertex1 && tempEdge2->indexVertex2 == tempEdge1->indexVertex1) ||
								(tempEdge2->indexVertex2 == tempEdge->indexVertex1 && tempEdge2->indexVertex1 == tempEdge1->indexVertex1))
							{
								// Create the nex Edge
								float size1 = (currentVertex - nextVertexNeighbour).Length();
								float size2 = (currentVertexNeighbour - nextVertex).Length();
								Edge * edge;
								if (size1 < size2)
								{
									edge = new Edge(currentIndex, tempEdge1->indexVertex1);
									this->listIndexesWireframe->push_back(currentIndex);
									this->listIndexesWireframe->push_back(tempEdge1->indexVertex1);
								}
								else
								{
									edge = new Edge(tempEdge->indexVertex1, nextIndex);
									this->listIndexesWireframe->push_back(tempEdge->indexVertex1);
									this->listIndexesWireframe->push_back(nextIndex);
								}

								// Create the first face
								Face * face1 = new Face(tempEdge, tempEdge2, edge);
								face1->VerifyFace(this->listVertices, vec3(0.0f, 1.0f, 0.0f));
								listNewFaces.push_back(face1);
								this->listFaces->push_back(face1);
								
								// Add the indixes
								unsigned short index0 = tempEdge->indexVertex1, index1 = tempEdge->indexVertex2;
								if (!face1->normalInverse)
								{
									this->listIndexes->push_back(index0);
									this->listIndexes->push_back(index1);
								}
								else
								{
									this->listIndexes->push_back(index1);
									this->listIndexes->push_back(index0);
								}
								if (index0 != tempEdge2->indexVertex1 && index1 != tempEdge2->indexVertex1)
									this->listIndexes->push_back(tempEdge2->indexVertex1);
								else
									this->listIndexes->push_back(tempEdge2->indexVertex2);
								
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
								face2->VerifyFace(this->listVertices, vec3(0.0f, 1.0f, 0.0f));
								listNewFaces.push_back(face2);
								this->listFaces->push_back(face2);
								
								// Add the indixes
								index0 = tempEdge1->indexVertex1;
								index1 = tempEdge1->indexVertex2;
								if (!face2->normalInverse)
								{
									this->listIndexes->push_back(index0);
									this->listIndexes->push_back(index1);
								}
								else
								{
									this->listIndexes->push_back(index1);
									this->listIndexes->push_back(index0);
								}
								if (index0 != tempEdge3->indexVertex1 && index1 != tempEdge3->indexVertex1)
									this->listIndexes->push_back(tempEdge3->indexVertex1);
								else
									this->listIndexes->push_back(tempEdge3->indexVertex2);
							}
						}
					}
				}
			}
		}
	}
}

void GameObject::CalculateIndexes()
{
	this->listIndexesWireframe->clear();
	this->listIndexes->clear();
	for (unsigned int i = 0; i < this->listVertices->size(); ++i)
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
			this->listIndexes->push_back(index0);
			this->listIndexes->push_back(index1);
		}
		else
		{
			this->listIndexes->push_back(index1);
			this->listIndexes->push_back(index0);
		}
		if (index0 != edge2->indexVertex1 && index1 != edge2->indexVertex1)
			this->listIndexes->push_back(edge2->indexVertex1);
		else
			this->listIndexes->push_back(edge2->indexVertex2);
	}
}

void GameObject::RenderObject(GLint & position_attrib, GLint & color_attrib)
{
	glVertexAttribPointer(position_attrib, 3, GL_FLOAT, false, 0, &this->listVertices->at(0).x);
	glUniform4f(color_attrib,  this->color->x, this->color->y, this->color->z, this->color->w);
	glDrawElements(GL_TRIANGLES, this->listIndexes->size(), GL_UNSIGNED_SHORT, &this->listIndexes->at(0));
	glUniform4f(color_attrib,  1.f, 0.f, 0.f, 1.f);
	glDrawElements(GL_LINES, this->listIndexesWireframe->size(), GL_UNSIGNED_SHORT, &this->listIndexesWireframe->at(0));
	glUniform4f(color_attrib,  0.f, 0.f, 1.f, 1.f);
	glDrawElements(GL_POINTS, this->listIndexesPoint->size(), GL_UNSIGNED_SHORT, &this->listIndexesPoint->at(0));
}