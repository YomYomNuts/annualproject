#include "DelaunayGenerator.h"
#include "ConvexHull.h"

#include <map>

DelaunayGenerator::DelaunayGenerator(void)
{
}

DelaunayGenerator::~DelaunayGenerator(void)
{
}

float RandomFloatTemp(float Low, float High)
{
  return (float)(((double) rand() / RAND_MAX) * (High-Low) + Low);
}

std::vector<unsigned short>* DelaunayGenerator::LaunchCircleGeneration(float radiusZone, int nbVerticesGenerate, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces)
{
	int i = 0;
	std::vector<vec3*> * listVerticesConvexPolygon = new std::vector<vec3*>();
	float powRadiusZone = radiusZone * radiusZone;
	while (i < nbVerticesGenerate)
	{
		vec3 newPosition(RandomFloatTemp(-radiusZone, radiusZone), 0, 0);
		float maxValueY = sqrt(powRadiusZone - newPosition.x * newPosition.x);
		newPosition.y = RandomFloatTemp(-maxValueY, maxValueY);
		bool find = false;
		for (unsigned int j = 0; j < listVertex->size(); ++j)
		{
			vec3 * tempPos = &listVertex->at(j);
			if (tempPos->x == newPosition.x && tempPos->y == newPosition.y)
			{
				find = true;
				break;
			}
		}
		if (!find)
		{
			listVertex->push_back(newPosition);
			DelaunayGenerator::AddPoint(&listVertex->at(listVertex->size() - 1), listVertex, listEdges, listFaces, listVerticesConvexPolygon);
			ConvexHull::DivideAndConquer(listVertex, listVerticesConvexPolygon);
			++i;
		}
	}
	Face::VerifyFaces(listVertex, listFaces, vec3(0,0,1));
	printf("nb vertex %d\nnb edges %d\nnb faces %d\n", listVertex->size(), listEdges->size(), listFaces->size());
	
	std::vector<unsigned short> * listIndexConvexPolygon = new std::vector<unsigned short>();
	while (listVerticesConvexPolygon->size() > 0)
	{
		vec3 vertex = *listVerticesConvexPolygon->at(0);
		for (unsigned int i = 0; i < listVertex->size(); ++i)
		{
			if (listVertex->at(i) == vertex)
			{
				listIndexConvexPolygon->push_back(i);
				listVerticesConvexPolygon->erase(listVerticesConvexPolygon->begin());
				break;
			}
		}
	}

	delete listVerticesConvexPolygon;

	return listIndexConvexPolygon;
}

void DelaunayGenerator::LaunchTriangulationGeneration(GameObject * go, std::vector<unsigned short> * listIndexVerticesFaces, vec3 & normalFace)
{
	std::vector<vec3*> * listVerticesConvexPolygon = new std::vector<vec3*>();
	std::vector<vec3> * listVertex = new std::vector<vec3>();
	std::vector<Edge*> * listEdges = new std::vector<Edge*>();
	std::vector<Face*> * listFaces = new std::vector<Face*>();
	std::map<unsigned short, unsigned short> * listIndexCorresponding = new std::map<unsigned short, unsigned short>();

	for (unsigned int i = 0; i < listIndexVerticesFaces->size(); ++i)
	{
		listIndexCorresponding->insert(std::pair<unsigned short, unsigned int>(i, listIndexVerticesFaces->at(i)));
		listVertex->push_back(go->listVertices->at(listIndexVerticesFaces->at(i)));
		DelaunayGenerator::AddPoint(&listVertex->at(i), listVertex, listEdges, listFaces, listVerticesConvexPolygon);
		ConvexHull::DivideAndConquer(listVertex, listVerticesConvexPolygon);
	}
	Face::VerifyFaces(listVertex, listFaces, normalFace);

	for (unsigned int i = 0; i < listEdges->size(); ++i)
	{
		Edge * edge = listEdges->at(i);
		std::map<unsigned short,unsigned short>::iterator it = listIndexCorresponding->find(edge->indexVertex1);
		edge->indexVertex1 = it->second;
		it = listIndexCorresponding->find(edge->indexVertex2);
		edge->indexVertex2 = it->second;
		bool find = false;
		for (unsigned int j = 0; j < go->listEdges->size(); ++j)
		{
			Edge * tempEdge = go->listEdges->at(j);
			if ((edge->indexVertex1 == tempEdge->indexVertex1 && edge->indexVertex2 == tempEdge->indexVertex2) ||
				(edge->indexVertex2 == tempEdge->indexVertex1 && edge->indexVertex1 == tempEdge->indexVertex2))
			{
				find = true;
				break;
			}
		}
		if (!find)
			go->listEdges->push_back(edge);
	}
	go->listFaces->insert(go->listFaces->begin(), listFaces->begin(), listFaces->end());

	delete listVerticesConvexPolygon;
	delete listVertex;
	delete listEdges;
	delete listFaces;
	delete listIndexCorresponding;
}

void DelaunayGenerator::AddPoint(vec3 * pos, std::vector<vec3> * listVertex, std::vector<Edge*> * listEdges, std::vector<Face*> * listFaces, std::vector<vec3*> * listVerticesConvexPolygon)
{
	int indexPosition = listVertex->size() - 1;
	if (listFaces->size() == 0)
	{
		switch (listVertex->size())
		{
			case 1:
			{
				listEdges->push_back(new Edge(indexPosition));
				break;
			}
			case 2:
			{
				listEdges->at(0)->indexVertex2 = indexPosition;
				break;
			}
			default:
			{
				vec3 ab(*pos - listVertex->at(0));
				vec3 ac(*pos - listVertex->at(1));
				bool isColineaire = (ab.Cross(ac).Length() == 0);
				
				if (isColineaire)
				{
					if (pos->x == listVertex->at(0).x)
					{
						vec3 * posTop = &listVertex->at(0), * posBottom = &listVertex->at(0);
						unsigned short indexTop = 0, indexBottom = 0;
						for (unsigned int i = 1; i < listVertex->size(); ++i)
						{
							vec3 * tempPos = &listVertex->at(i);
							if (tempPos->y > posTop->y)
							{
								posTop = tempPos;
								indexTop = i;
							}
							if (tempPos->y < posBottom->y)
							{
								posBottom = tempPos;
								indexBottom = i;
							}
						}
						
						if (pos->y > posTop->y)
							listEdges->push_back(new Edge(indexPosition, indexTop));
						else if (pos->y < posBottom->y)
							listEdges->push_back(new Edge(indexBottom, indexPosition));
						else
						{
							for (unsigned int i = 0; i < listEdges->size(); ++i)
							{
								Edge * edgeSearch = listEdges->at(i);
								vec3 * p0 = &listVertex->at(edgeSearch->indexVertex1);
								vec3 * p1 = &listVertex->at(edgeSearch->indexVertex2);
								if ((pos->y < p0->y && pos->y > p1->y) ||
									(pos->y < p1->y && pos->y > p0->y))
								{
									listEdges->push_back(new Edge(indexPosition, edgeSearch->indexVertex2));
									edgeSearch->indexVertex2 = indexPosition;
									break;
								}
							}
						}
					}
					else if (pos->y == listVertex->at(0).y)
					{
						vec3 * posRight = &listVertex->at(0), * posLeft = &listVertex->at(0);
						unsigned short indexRight = 0, indexLeft = 0;
						for (unsigned int i = 1; i < listVertex->size(); ++i)
						{
							vec3 * tempPos = &listVertex->at(i);
							if (tempPos->x > posRight->x)
							{
								posRight = tempPos;
								indexRight = i;
							}
							if (tempPos->x < posLeft->x)
							{
								posLeft = tempPos;
								indexLeft = i;
							}
						}
						
						if (pos->x > posRight->x)
							listEdges->push_back(new Edge(indexRight, indexPosition));
						else if (pos->x < posLeft->x)
							listEdges->push_back(new Edge(indexPosition, indexLeft));
						else
						{
							for (unsigned int i = 0; i < listEdges->size(); ++i)
							{
								Edge * edgeSearch = listEdges->at(i);
								vec3 * p0 = &listVertex->at(edgeSearch->indexVertex1);
								vec3 * p1 = &listVertex->at(edgeSearch->indexVertex2);
								if ((pos->x < p0->x && pos->x > p1->x) ||
									(pos->x < p1->x && pos->x > p0->x))
								{
									listEdges->push_back(new Edge(indexPosition, edgeSearch->indexVertex2));
									edgeSearch->indexVertex2 = indexPosition;
									break;
								}
							}
						}
					}
					else
					{
						for (unsigned int i = 0; i < listEdges->size(); ++i)
						{
							Edge * edgeSearch = listEdges->at(i);
							vec3 * p0 = &listVertex->at(edgeSearch->indexVertex1);
							vec3 * p1 = &listVertex->at(edgeSearch->indexVertex2);
							if ((pos->x < p0->x && pos->x > p1->x) || (pos->x < p1->x && pos->x > p0->x) ||
								(pos->y < p0->y && pos->y > p1->y) || (pos->y < p1->y && pos->y > p0->y))
							{
								listEdges->push_back(new Edge(indexPosition, edgeSearch->indexVertex2));
								edgeSearch->indexVertex2 = indexPosition;
								break;
							}
						}
					}
				}
				else
				{
					unsigned int size = listEdges->size();
					for (unsigned int i = 0; i < size; ++i)
					{
						Edge * edge1 = listEdges->at(i);
						Edge * edge2 = NULL, * edge3 = NULL;
						unsigned short indexVertex0 = edge1->indexVertex1;
						unsigned short indexVertex1 = edge1->indexVertex2;
						vec3 * p0 = &listVertex->at(indexVertex0);
						vec3 * p1 = &listVertex->at(indexVertex1);
						bool findEdge2 = false, findEdge3 = false;

						for (unsigned int j = size; j < listEdges->size(); ++j)
						{
							Edge * edgeSearch = listEdges->at(j);
							if ((edgeSearch->indexVertex1 == indexPosition && edgeSearch->indexVertex2 == indexVertex0) ||
								(edgeSearch->indexVertex1 == indexVertex0 && edgeSearch->indexVertex2 == indexPosition))
							{
								edge2 = edgeSearch;
								findEdge2 = true;
							}
							if ((edgeSearch->indexVertex1 == indexPosition && edgeSearch->indexVertex2 == indexVertex1) ||
								(edgeSearch->indexVertex1 == indexVertex1 && edgeSearch->indexVertex2 == indexPosition))
							{
								edge3 = edgeSearch;
								findEdge3 = true;
							}
						}
						if (!findEdge2)
						{
							listEdges->push_back(new Edge(indexPosition, listEdges->at(i)->indexVertex1));
							edge2 = listEdges->at(listEdges->size() - 1);
						}
						if (!findEdge3)
						{
							listEdges->push_back(new Edge(indexPosition, listEdges->at(i)->indexVertex2));
							edge3 = listEdges->at(listEdges->size() - 1);
						}
					
						if (p1->Convex(p0, pos) >= 0) // Left
						{
							Edge * edgeTemp = edge3;
							edge3 = edge2;
							edge2 = edgeTemp;
						}
						listFaces->push_back(new Face(edge1, edge2, edge3));
					}
				}
				break;
			}
		}
	}
	else
	{
		std::vector<Edge*> * listEdgesRemove = new std::vector<Edge*>();
		bool inTriangle = false;
		for (unsigned int i = 0; i < listFaces->size(); ++i)
		{
			Face * face = listFaces->at(i);
			Edge * edge1 = face->edge1, * edge2 = face->edge2, * edge3 = face->edge3;
			vec3 * pos0 = &listVertex->at(edge1->indexVertex1), * pos1 = &listVertex->at(edge1->indexVertex2);
			vec3 * pos2 = &listVertex->at(edge2->indexVertex1), * pos3 = &listVertex->at(edge2->indexVertex2);

			if (Face::VertexInFace(pos, pos0, pos1, pos2, pos3))
			{
				inTriangle = true;
				listEdgesRemove->push_back(edge1);
				listEdgesRemove->push_back(edge2);
				listEdgesRemove->push_back(edge3);
				listFaces->erase(listFaces->begin() + i);
				delete face;
				break;
			}
		}

		if (!inTriangle)
		{
			for (unsigned int i = 0; i < listVerticesConvexPolygon->size(); ++i)
			{
				vec3 * pos1 = listVerticesConvexPolygon->at(i), * pos2 = NULL;
				if (i < listVerticesConvexPolygon->size() - 1)
					pos2 = listVerticesConvexPolygon->at(i + 1);
				else
					pos2 = listVerticesConvexPolygon->at(0);

				if (pos1->Convex(pos2, pos) > 0)
				{
					for (unsigned int j = 0; j < listEdges->size(); ++j)
					{
						Edge * edge = listEdges->at(j);
						vec3 p0 = listVertex->at(edge->indexVertex1);
						vec3 p1 = listVertex->at(edge->indexVertex2);
						if ((p0 == *pos1 && p1 == *pos2) || (p1 == *pos1 && p0 == *pos2))
						{
							listEdgesRemove->push_back(edge);
							break;
						}
					}
				}
			}
		}
			
		while (listEdgesRemove->size() > 0)
		{
			Edge * edge = listEdgesRemove->at(0);
			vec3 * p0 = &listVertex->at(edge->indexVertex1);
			vec3 * p1 = &listVertex->at(edge->indexVertex2);
			vec3 * otherPoint = NULL;
			Face * face = NULL;
			Edge * edge1 = NULL, * edge2 = NULL, * edge3 = NULL;
			unsigned int indexTriangle;
				
			for (indexTriangle = 0; indexTriangle < listFaces->size(); ++indexTriangle)
			{
				face = listFaces->at(indexTriangle);
				edge1 = face->edge1, edge2 = face->edge2, edge3 = face->edge3;
				if (edge1 == edge)
				{
					if (edge2->indexVertex1 == edge->indexVertex1 || edge2->indexVertex1 == edge->indexVertex2)
						otherPoint = &listVertex->at(edge2->indexVertex2);
					else
						otherPoint = &listVertex->at(edge2->indexVertex1);
					break;
				}
				else if (edge2 == edge)
				{
					if (edge1->indexVertex1 == edge->indexVertex1 || edge1->indexVertex1 == edge->indexVertex2)
						otherPoint = &listVertex->at(edge1->indexVertex2);
					else
						otherPoint = &listVertex->at(edge1->indexVertex1);
					break;
				}
				else if (edge3 == edge)
				{
					if (edge1->indexVertex1 == edge->indexVertex1 || edge1->indexVertex1 == edge->indexVertex2)
						otherPoint = &listVertex->at(edge1->indexVertex2);
					else
						otherPoint = &listVertex->at(edge1->indexVertex1);
					break;
				}
			}

			if (otherPoint != NULL)
			{
				vec3 * circumcenter = new vec3();
				Face::Circumcenter3D(otherPoint, p0, p1, circumcenter);
				double radius = sqrt(pow(p0->x - circumcenter->x, 2) + pow(p0->y - circumcenter->y, 2) + pow(p0->z - circumcenter->z, 2));
				double dist = sqrt(pow(pos->x - circumcenter->x, 2) + pow(pos->y - circumcenter->y, 2) + pow(pos->z - circumcenter->z, 2));
				if (dist <= radius)
				{
					if (edge1 == edge)
					{
						listEdgesRemove->push_back(edge2);
						listEdgesRemove->push_back(edge3);
					}
					else if (edge2 == edge)
					{
						listEdgesRemove->push_back(edge1);
						listEdgesRemove->push_back(edge3);
					}
					else if (edge3 == edge)
					{
						listEdgesRemove->push_back(edge1);
						listEdgesRemove->push_back(edge2);
					}

					listFaces->erase(listFaces->begin() + indexTriangle);
					for (unsigned int j = 0; j < listEdges->size(); ++j)
					{
						if (listEdges->at(j) == edge)
						{
							listEdges->erase(listEdges->begin() + j);
							break;
						}
					}
				}
				else
					otherPoint = NULL;
				delete circumcenter;
			}
				
			if (otherPoint == NULL)
			{
				Edge * edge2 = new Edge(edge->indexVertex2, indexPosition), * edge3 = new Edge(indexPosition, edge->indexVertex1);

				Edge * edge2Search = NULL, * edge3Search = NULL;
				for (unsigned int j = 0; j < listEdges->size(); ++j)
				{
					Edge * edgeSearch = listEdges->at(j);
					if ((edge2->indexVertex1 == edgeSearch->indexVertex1 && edge2->indexVertex2 == edgeSearch->indexVertex2 ) ||
						(edge2->indexVertex2 == edgeSearch->indexVertex1 && edge2->indexVertex1 == edgeSearch->indexVertex2))
					{
						edge2Search = edgeSearch;
						if (edge3Search != NULL)
							break;
					}
					if ((edge3->indexVertex1 == edgeSearch->indexVertex1 && edge3->indexVertex2 == edgeSearch->indexVertex2) ||
						(edge3->indexVertex2 == edgeSearch->indexVertex1 && edge3->indexVertex1 == edgeSearch->indexVertex2))
					{
						edge3Search = edgeSearch;
						if (edge2Search != NULL)
							break;
					}
				}
				
				Face * face = new Face();
				face->edge1 = edge;
				if (edge2Search == NULL)
					face->edge2 = edge2;
				else
				{
					delete edge2;
					face->edge2 = edge2Search;
				}

				if (edge3Search == NULL)
					face->edge3 = edge3;
				else
				{
					delete edge3;
					face->edge3 = edge3Search;
				}

				
				vec3 * pos0 = &listVertex->at(edge->indexVertex1), * pos1 = &listVertex->at(edge->indexVertex2);
				vec3 * pos2 = &listVertex->at(face->edge2->indexVertex1);
				if (pos0 == pos2 || pos1 == pos2)
					pos2 = &listVertex->at(face->edge2->indexVertex2);
				vec3 ab(*pos0 - *pos1);
				vec3 ac(*pos0 - *pos2);
				bool isColineaire = (ab.Cross(ac).Length() == 0);
				if (isColineaire)
				{
					for (unsigned int j = 0; j < listEdges->size(); ++j)
					{
						if (listEdges->at(j) == edge)
						{
							listEdges->erase(listEdges->begin() + j);
							break;
						}
					}
					delete face;
				}
				else
				{
					if (edge2Search == NULL)
						listEdges->push_back(edge2);
					if (edge3Search == NULL)
						listEdges->push_back(edge3);
					if (p1->Convex(p0, pos) <= 0) // Right
					{
						Edge * edgeTemp = face->edge3;
						face->edge3 = face->edge2;
						face->edge2 = edgeTemp;
					}
					listFaces->push_back(face);
				}
			}
			listEdgesRemove->erase(listEdgesRemove->begin());
		}
		delete listEdgesRemove;
	}
}