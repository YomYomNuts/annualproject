#include "MathFunctions.h"


MathFunctions::MathFunctions(void)
{
}

float MathFunctions::dotProduct(vec3 vecA, vec3 vecB){

	//float dot;

	//dot = (vecA.x * vecB.x) + (vecA.y * vecB.y) + (vecA.z * vecB.z);

	//return dot;

	__m128 vec1;
	__m128 vec2;

	vec1 = _mm_set_ps(vecA.x, vecA.y, vecA.z, 0.0f);
	vec2 = _mm_set_ps(vecB.x, vecB.y, vecB.z, 0.0f);

	vec1 = _mm_mul_ps(vec1, vec2);
	vec1 = _mm_hadd_ps(vec1, vec1);
	vec1 = _mm_hadd_ps(vec1, vec1);

	return vec1.m128_f32[0];
}

vec3 MathFunctions::crossProduct(vec3 vect1, vec3 vect2)
{
	//float x;
	//float y;
	//float z;

	//x = (vect1.y * vect2.z) - (vect1.z * vect2.y);
	//y = (vect1.z * vect2.x) - (vect1.x * vect2.z);
	//z = (vect1.x * vect2.y) - (vect1.y * vect2.x);

	//return vec3(x, y, z);

	__m128 reg1;
	__m128 reg2;
	__m128 regXY;

	__m128 reg3;
	__m128 reg4;
	__m128 regZ;

	reg1 = _mm_set_ps(vect1.x, vect1.z, vect1.z, vect1.y);
	reg2 = _mm_set_ps(vect2.z, vect2.x, vect2.y, vect2.z);

	regXY = _mm_mul_ps(reg1, reg2);
	regXY = _mm_hsub_ps(regXY, regXY);

	reg3 = _mm_set_ps(0.0f, 0.0f, vect1.y, vect1.x);
	reg4 = _mm_set_ps(0.0f, 0.0f, vect2.x, vect2.y);

	regZ = _mm_mul_ps(reg3, reg4);
	regZ = _mm_hsub_ps(regZ, regZ);

	return vec3(regXY.m128_f32[0], regXY.m128_f32[1], regZ.m128_f32[0]);

}

vec3 MathFunctions::normalizeVector(vec3 vect)
{
	//float x;
	//float y;
	//float z;
	//float squareRoot;

	//if(dotProduct(vect, vect) != 1)
	//{
	//	x = pow(vect.x,2);
	//	y = pow(vect.y,2);
	//	z = pow(vect.z,2);

	//	squareRoot = sqrt(x + y + z);
	//}
	//else
	//	return vect;
	//
	//return vec3(vect.x / squareRoot, vect.y / squareRoot, vect.z / squareRoot);

	if(dotProduct(vect, vect) != 1)
	{
		__m128 reg;
		__m128 regSQRT;

		reg = _mm_set_ps(0.0f, vect.z, vect.y, vect.x);
		regSQRT = _mm_mul_ps(reg, reg);
		regSQRT = _mm_hadd_ps(regSQRT, regSQRT);
		regSQRT = _mm_hadd_ps(regSQRT, regSQRT);
		regSQRT = _mm_sqrt_ps(regSQRT);

		reg = _mm_div_ps(reg, regSQRT);

		return vec3(reg.m128_f32[0], reg.m128_f32[1], reg.m128_f32[2]);
	}
	else
		return vect;

}

vec3 MathFunctions::getVector(vec3 pointA, vec3 pointB)
{
	return vec3(pointB.x - pointA.x, pointB.y - pointA.y, pointB.z - pointA.z);
}

vec3 MathFunctions::inverseNormal(vec3 normal)
{
	return vec3(-1 * normal.x, -1 * normal.y, -1 * normal.z);
}

float MathFunctions::getVolumeOfASphere(float radius)
{
	return (4 * M_PI * pow(radius,3))/3;
}

float MathFunctions::getVolumeOfACube(float sizeOfEdge)
{
	return pow(sizeOfEdge,3);
}

MathFunctions::~MathFunctions(void)
{
}

float MathFunctions::getAngleBetweenVectors(vec3 vect1, vec3 vect2)
{
	return acos((dotProduct(vect1, vect2))/(sqrtf(dotProduct(vect1, vect1))*sqrtf(dotProduct(vect2, vect2)))) * 180 / M_PI;
}

vec3 MathFunctions::roundf(vec3 value, int decimal)
{
	float x = value.x;
	float y = value.y;
	float z = value.z;

	int precision = 1;
	int i;

	for(i = 0; i < decimal; ++i)
		precision *= 10;

	x = floor(x * precision) / precision;
	y = floor(y * precision) / precision;
	z = floor(z * precision) / precision;

	return vec3(x, y, z);
}

float MathFunctions::roundf(float value, int decimal)
{
	int precision = 1;
	int i;

	for(i = 0; i < decimal; ++i)
		precision *= 10;

	return floor(value * precision) / precision;
}

vec3 MathFunctions::getNormalFrom2Vecs(vec3 pointA1, vec3 pointA2, vec3 pointB1, vec3 pointB2)
{
	vec3 vect1;
	vec3 vect2;
	vec3 cp;

	vect1 = getVector(pointA1, pointA2);
	vect2 = getVector(pointB1, pointB2);

	cp = crossProduct(vect1, vect2);

	return normalizeVector(inverseNormal(cp));
}

bool MathFunctions::isTriangleFlat(vec3 pointA1, vec3 pointA2, vec3 pointB1, vec3 pointB2)
{
	vec3 vect1;
	vec3 vect2;
	vec3 cp;

	vect1 = getVector(pointA1, pointA2);
	vect2 = getVector(pointB1, pointB2);

	cp = crossProduct(vect1, vect2);

	return (cp==0.0f?true:false);
}

bool MathFunctions::sameSide(vec3 pointToTest, vec3 vertexA, vec3 vertexB, vec3 vertexC)
{
	vec3 cp1;
	vec3 cp2;
	float dp;

	cp1 = crossProduct(vertexC - vertexB, pointToTest - vertexB);
	cp2 = crossProduct(vertexC - vertexB, vertexA - vertexB);
	
	dp = dotProduct(cp1, cp2);

	return (dp >= 0 ? true : false);
}

bool MathFunctions::pointInTriangle(vec3 pointToTest, vec3 vertexA, vec3 vertexB, vec3 vertexC)
{

	if( sameSide(pointToTest, vertexA, vertexB, vertexC) &&
		sameSide(pointToTest, vertexB, vertexA, vertexC) &&
		sameSide(pointToTest, vertexC, vertexA, vertexB))
		return true;
	else
		return false;
}

bool MathFunctions::pointOnSegment(vec3 pointToTest, vec3 pointA, vec3 pointB)
{
	vec3 cp;
	vec3 vAB;
	vec3 vAP;

	vAB = getVector(pointA, pointB);
	vAP = getVector(pointA, pointToTest);

	// Check if the point is on the LINE
	cp = crossProduct(vAB, vAP);
	float avg = (cp.x + cp.y + cp.z)/3;
	// Check if the point is on the SEGMENT ( inbetween the points)
	if(avg >= -0.1f && avg <= 0.1f)
	//if(cp == 0.0f)
	{
		float dp1;
		float dp2;

		dp1 = dotProduct(vAB, vAB);
		dp2 = dotProduct(vAB, vAP);

		if(dp2 < 0.0f)
			return false;

		if(dp2 > dp1)
			return false;

		return true;
	}

	return false;
}

float MathFunctions::distanceBetween2points(vec3 pointA, vec3 pointB)
{
	vec3 vector = pointA - pointB;

	return sqrtf(this->dotProduct(vector, vector));
}