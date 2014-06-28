#ifndef __QUATERNION__
#define __QUATERNION__

#include "../EsgiGL/Common/EsgiUtils.h"
#include "../EsgiGL/EsgiGL.h"
#include "MathFunctions.h"
//#include "functions.h"
#include <xmmintrin.h>

#define PI_DIV_180 0.01745329251f

class Quaternion
{
public:
	Quaternion(void);
	vec3 getQuaternion3f(float angle, vec3 vect);
	vec4 getQuaternion4f(float angle, vec3 vect);
	vec4 multiplyQuaternion4f(vec4 quat1, vec4 quat2);
	vec3 multiplyQuaternion3f(vec4 quat1, vec4 quat2);
	~Quaternion(void);

private:
	MathFunctions MF;
};

#endif
