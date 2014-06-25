#ifndef DEF_CONVEXHULL_H
#define DEF_CONVEXHULL_H

#include "../../EsgiGL/Common/vector.h"
#include <vector>

class ConvexHull
{
public:
	static void DivideAndConquer(std::vector<vec3> * listVertices, std::vector<vec3*> * listVerticesConvexPolygon);
private:
	static void ConvexHullTopSegment(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<int> * listTopVerticesConvexPolygon);
	static void ConvexHullBottomSegment(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<int> * listBottomVerticesConvexPolygon);
	static void ConvexHullJoin(std::vector<vec3*> * r1, std::vector<vec3*> * r2, std::vector<vec3*> * listVerticesConvexPolygon);
	static void ConvexHullRecursive(std::vector<vec3> * listVertices, std::vector<vec3*> * listVerticesConvexPolygon, bool (*FunctionCompvec3)(vec3, vec3));
};

#endif