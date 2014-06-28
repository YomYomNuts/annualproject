#ifndef __MATHFUNCTIONS__
#define __MATHFUNCTIONS__

#include "../EsgiGL/Common/EsgiUtils.h"
#include "../EsgiGL/EsgiGL.h"
#include <vector>
#include <cmath>
#include <iostream>


#define Signe(val)	(val >= 0?1:-1)

#define M_PI	3.14159265358979323846f

class MathFunctions
{
public:
	MathFunctions(void);
	float dotProduct(vec3 vecA, vec3 vecB);
	vec3 crossProduct(vec3 vect1, vec3 vect2);
	vec3 normalizeVector(vec3 vect);
	vec3 getVector(vec3 pointA, vec3 pointB);
	vec3 inverseNormal(vec3 normal);
	float getVolumeOfASphere(float radius);
	float getVolumeOfACube(float sizeOfEdge);
	float getAngleBetweenVectors(vec3 vect1, vec3 vect2);
	vec3 roundf(vec3 value, int decimal);
	float roundf(float value, int decimal);
	vec3 getNormalFrom2Vecs(vec3 pointA1, vec3 pointA2, vec3 pointB1, vec3 pointB2);
	bool isTriangleFlat(vec3 pointA1, vec3 pointA2, vec3 pointB1, vec3 pointB2);
	bool sameSide(vec3 pointToTest, vec3 vertexA, vec3 vertexB, vec3 vertexC);
	bool pointInTriangle(vec3 pointToTest, vec3 vertexA, vec3 vertexB, vec3 vertexC);
	bool pointOnSegment(vec3 pointToTest, vec3 pointA, vec3 pointB);
	float distanceBetween2points(vec3 pointA, vec3 pointB);
	~MathFunctions(void);
};


#endif