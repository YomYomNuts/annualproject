#include "ConvexHull.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>


bool FunctionCompvec3PointerLeft(vec3 * pos1, vec3 * pos2)
{
	if(pos1->x < pos2->x)
		return true;
	else if(pos1->x == pos2->x && pos1->y < pos2->y)
		return true;
	return false;
}
bool FunctionCompvec3Left(vec3 pos1, vec3 pos2)
{
	if(pos1.x < pos2.x)
		return true;
	else if(pos1.x == pos2.x && pos1.y < pos2.y)
		return true;
	return false;
}

void ConvexHull::ConvexHullTopSegment(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<int> * listTopVerticesConvexPolygon)
{
	int i = 0, j = 0;
	
	for (unsigned int m = 1; m < r1->size(); ++m)
	{
		if (r1->at(m)->y > r1->at(i)->y)
			i = m;
		else if (r1->at(m)->y == r1->at(i)->y && r1->at(m)->x < r1->at(i)->x)
			i = m;
	}
	for (unsigned int m = 1; m < r2->size(); ++m)
	{
		if (r2->at(m)->y > r2->at(j)->y)
			j = m;
		else if (r2->at(m)->y == r2->at(j)->y && r2->at(m)->x > r2->at(j)->x)
			j = m;
	}
	
	bool modif;
	do
	{
		modif = false;

		if (r1->size() > 1)
		{
			int indexPrevR1 = i - 1;
			if (indexPrevR1 < 0)
				indexPrevR1 = r1->size() - 1;
			int indexNextR1 = i + 1;
			if (indexNextR1 > (int)r1->size() - 1)
				indexNextR1 = 0;
			double convexR1Prev = r1->at(i)->Convex(r1->at(indexPrevR1), r2->at(j));
			double convexR1Next = r1->at(i)->Convex(r1->at(indexNextR1), r2->at(j));
			if (convexR1Prev < 0 || (convexR1Prev == 0 && r1->at(indexPrevR1)->x < r1->at(i)->x))
			{
				i = indexPrevR1;
				modif = true;
			}
			else if (convexR1Next < 0 || (convexR1Next == 0 && r1->at(indexNextR1)->x < r1->at(i)->x))
			{
				i = indexNextR1;
				modif = true;
			}
		}
		
		if (r2->size() > 1)
		{
			int indexPrevR2 = j - 1;
			if (indexPrevR2 < 0)
				indexPrevR2 = r2->size() - 1;
			int indexNextR2 = j + 1;
			if (indexNextR2 > (int)r2->size() - 1)
				indexNextR2 = 0;
			double convexR2Prev = r2->at(j)->Convex(r2->at(indexPrevR2), r1->at(i));
			double convexR2Next = r2->at(j)->Convex(r2->at(indexNextR2), r1->at(i));
			if (convexR2Prev > 0 || (convexR2Prev == 0 && r2->at(indexPrevR2)->x > r2->at(j)->x))
			{
				j = indexPrevR2;
				modif = true;
			}
			else if (convexR2Next > 0 || (convexR2Next == 0 && r2->at(indexNextR2)->x > r2->at(j)->x))
			{
				j = indexNextR2;
				modif = true;
			}
		}
	} while(modif);

	listTopVerticesConvexPolygon->push_back(i);
	listTopVerticesConvexPolygon->push_back(j);
}

void ConvexHull::ConvexHullBottomSegment(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<int> * listBottomVerticesConvexPolygon)
{
	int i = 0, j = 0;
	
	for (unsigned int m = 1; m < r1->size(); ++m)
	{
		if (r1->at(m)->y < r1->at(i)->y)
			i = m;
		else if (r1->at(m)->y == r1->at(i)->y && r1->at(m)->x > r1->at(i)->x)
			i = m;
	}
	for (unsigned int m = 1; m < r2->size(); ++m)
	{
		if (r2->at(m)->y < r2->at(j)->y)
			j = m;
		else if (r2->at(m)->y == r2->at(j)->y && r2->at(m)->x < r2->at(j)->x)
			j = m;
	}
	
	bool modif;
	do
	{
		modif = false;
		
		if (r1->size() > 1)
		{
			int indexPrevR1 = i - 1;
			if (indexPrevR1 < 0)
				indexPrevR1 = r1->size() - 1;
			int indexNextR1 = i + 1;
			if (indexNextR1 > (int)r1->size() - 1)
				indexNextR1 = 0;
			double convexR1Prev = r1->at(i)->Convex(r1->at(indexPrevR1), r2->at(j));
			double convexR1Next = r1->at(i)->Convex(r1->at(indexNextR1), r2->at(j));
			if (convexR1Prev > 0 || (convexR1Prev == 0 && r1->at(indexPrevR1)->x > r1->at(i)->x))
			{
				i = indexPrevR1;
				modif = true;
			}
			else if (convexR1Next > 0 || (convexR1Next == 0 && r1->at(indexNextR1)->x > r1->at(i)->x))
			{
				i = indexNextR1;
				modif = true;
			}
		}
		
		if (r2->size() > 1)
		{
			int indexPrevR2 = j - 1;
			if (indexPrevR2 < 0)
				indexPrevR2 = r2->size() - 1;
			int indexNextR2 = j + 1;
			if (indexNextR2 > (int)r2->size() - 1)
				indexNextR2 = 0;
			double convexR2Prev = r2->at(j)->Convex(r2->at(indexPrevR2), r1->at(i));
			double convexR2Next = r2->at(j)->Convex(r2->at(indexNextR2), r1->at(i));
			if (convexR2Prev < 0 || (convexR2Prev == 0 && r2->at(indexPrevR2)->x < r2->at(j)->x))
			{
				j = indexPrevR2;
				modif = true;
			}
			else if (convexR2Next < 0 || (convexR2Next == 0 && r2->at(indexNextR2)->x < r2->at(j)->x))
			{
				j = indexNextR2;
				modif = true;
			}
		}
	} while(modif);

	listBottomVerticesConvexPolygon->push_back(i);
	listBottomVerticesConvexPolygon->push_back(j);
}

void ConvexHull::ConvexHullJoin(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<vec3*> * listVerticesConvexPolygon)
{
	std::vector<int> * topvec3 = new std::vector<int>();
	std::vector<int> * bottomvec3 = new std::vector<int>();

	ConvexHullTopSegment(r1, r2, topvec3);
	ConvexHullBottomSegment(r1, r2, bottomvec3);
	
	unsigned int iEndPos = topvec3->at(0);
	unsigned int iStartPos = bottomvec3->at(0);
	while (iStartPos != iEndPos)
	{
		listVerticesConvexPolygon->push_back(r1->at(iStartPos));
		++iStartPos;
		if (iStartPos > r1->size() - 1)
			iStartPos = 0;
	}
	listVerticesConvexPolygon->push_back(r1->at(iEndPos));
	
	iEndPos = bottomvec3->at(1);
	iStartPos = topvec3->at(1);
	while (iStartPos != iEndPos)
	{
		listVerticesConvexPolygon->push_back(r2->at(iStartPos));
		++iStartPos;
		if (iStartPos > r2->size() - 1)
			iStartPos = 0;
	}
	listVerticesConvexPolygon->push_back(r2->at(iEndPos));

	delete topvec3;
	delete bottomvec3;
}

void ConvexHull::ConvexHullRecursive(std::vector<vec3> * listVertices, std::vector<vec3*> * listVerticesConvexPolygon, bool (*FunctionCompvec3)(vec3, vec3))
{
	listVerticesConvexPolygon->clear();
	sort(listVertices->begin(), listVertices->end(), FunctionCompvec3);
	if (listVertices->size() < 3)
	{
		for (unsigned int i = 0; i < listVertices->size(); ++i)
		{
			listVerticesConvexPolygon->push_back(&listVertices->at(i));
		}
	}
	else
	{
		int size = listVertices->size() / 2;
		std::vector<vec3> * p1 = new std::vector<vec3>(listVertices->begin(), listVertices->begin() + size);
		std::vector<vec3*> * r1 = new std::vector<vec3*>();
		ConvexHullRecursive(p1, r1, FunctionCompvec3Left);

		std::vector<vec3> * p2 = new std::vector<vec3>(listVertices->begin() + size, listVertices->end());
		std::vector<vec3*> * r2 = new std::vector<vec3*>();
		ConvexHullRecursive(p2, r2, FunctionCompvec3Left);

		ConvexHullJoin(r1, r2, listVerticesConvexPolygon);

		delete r1;
		delete r2;
	}
}

void ConvexHull::DivideAndConquer(std::vector<vec3> * listVertices, std::vector<vec3*> * listVerticesConvexPolygon)
{
	std::vector<vec3> * listTempVertices = new std::vector<vec3>(listVertices->begin(), listVertices->end());
	ConvexHullRecursive(listTempVertices, listVerticesConvexPolygon, FunctionCompvec3Left);
	delete listTempVertices;
}