#include "Quaternion.h"
#include <xmmintrin.h> 

#include "sse_mathfun.h"
#include <emmintrin.h>
#include <mmintrin.h>

Quaternion::Quaternion(void)
{
}

vec3 Quaternion::getQuaternion3f(float angle, vec3 vect)
{	
	/*float w;
	float x;
	float y;
	float z;

	w = (float)cos(((angle/2)*M_PI) / 180);
	x = (float)sin(((angle/2)*M_PI) / 180) * vect.x;
	y = (float)sin(((angle/2)*M_PI) / 180) * vect.y;
	z = (float)sin(((angle/2)*M_PI) / 180) * vect.z;

	return vec3(x, y, z);*/

	__m128 reg1;
	__m128 reg2;

	reg1 = _mm_set_ps1(angle);
	reg2 = _mm_set_ps1(2.0f);
	reg1 = _mm_div_ps(reg1, reg2);

	reg2 = _mm_set_ps1(PI_DIV_180);
	reg1 = _mm_mul_ps(reg1, reg2);

	reg2 = reg1;
	reg1 = sin_ps(reg1);
	reg2 = cos_ps(reg2);
	reg1 = _mm_set_ps(reg1.m128_f32[3], reg1.m128_f32[2], reg1.m128_f32[1], reg2.m128_f32[3]);
	
	reg2 = _mm_set_ps(vect.z, vect.y, vect.x, 1.0f);
	reg1 = _mm_mul_ps(reg1, reg2);

	
	return vec3(reg1.m128_f32[1], reg1.m128_f32[2], reg1.m128_f32[3]);
}

vec4 Quaternion::getQuaternion4f(float angle, vec3 vect)
{
	/*
	float w;
	float x;
	float y;
	float z;

	w = (float)cos(((angle/2)*M_PI) / 180);
	x = (float)sin(((angle/2)*M_PI) / 180) * vect.x;
	y = (float)sin(((angle/2)*M_PI) / 180) * vect.y;
	z = (float)sin(((angle/2)*M_PI) / 180) * vect.z;
	
	return vec4 (w, x, y, z);*/

	__m128 reg1;
	__m128 reg2;

	reg1 = _mm_set_ps1(angle);
	reg2 = _mm_set_ps1(2.0f);
	reg1 = _mm_div_ps(reg1, reg2);

	reg2 = _mm_set_ps1(PI_DIV_180);
	reg1 = _mm_mul_ps(reg1, reg2);

	reg2 = reg1;
	reg1 = sin_ps(reg1);
	reg2 = cos_ps(reg2);
	reg1 = _mm_set_ps(reg1.m128_f32[3], reg1.m128_f32[2], reg1.m128_f32[1], reg2.m128_f32[3]);
	
	reg2 = _mm_set_ps(vect.z, vect.y, vect.x, 1.0f);
	reg1 = _mm_mul_ps(reg1, reg2);

	return vec4(reg1.m128_f32[0], reg1.m128_f32[1], reg1.m128_f32[2], reg1.m128_f32[3]);
	
}

vec4 Quaternion::multiplyQuaternion4f(vec4 quat1, vec4 quat2)
{
	/*float w;

	vec3 part1;
	vec3 part2;
	vec3 part3;

	vec3 sumParts;

	w = quat1.x * quat2.x - (float)MF.dotProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));
	part1 = vec3(quat1.x * quat2.y, quat1.x * quat2.z, quat1.x * quat2.w);
	part2 = vec3(quat2.x * quat1.y, quat2.x * quat1.z, quat2.x * quat1.w);
	part3 = MF.crossProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));

	sumParts = part1 + part2 + part3;
	vec3 tempSum = part1 + part2;
	
	return vec4(w, sumParts.x, sumParts.y, sumParts.z);*/


	__m128 reg1;
	__m128 reg2;
	__m128 reg3;
	__m128 reg_part1;
	__m128 reg_part2;
	__m128 reg_part3;

	vec3 part3;

	reg1 = _mm_set_ss(quat1.x);
	reg2 = _mm_set_ss(quat2.x);

	reg1 = _mm_mul_ss(reg1, reg2);
	reg2 = _mm_set_ss(MF.dotProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w)));
	reg1 = _mm_sub_ss(reg1, reg2);


	reg2 = _mm_set_ps(0.0f, quat2.y,quat2.z, quat2.w);
	reg3 = _mm_set_ps(0.0f, quat1.x,quat1.x, quat1.x);

	reg_part1 = _mm_mul_ps(reg2, reg3);

	reg2 = _mm_set_ps(0.0f, quat1.y,quat1.z, quat1.w);
	reg3 = _mm_set_ps(0.0f, quat2.x,quat2.x, quat2.x);

	reg_part2 = _mm_mul_ps(reg2, reg3);

	part3 = MF.crossProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));

	reg_part3 = _mm_set_ps(0.0f, part3.x, part3.y, part3.z);

	reg2 = _mm_add_ps(reg_part1, reg_part2);
	reg2 = _mm_add_ps(reg2, reg_part3);

	return vec4(reg1.m128_f32[0], reg2.m128_f32[2], reg2.m128_f32[1], reg2.m128_f32[0]); 

}

vec3 Quaternion::multiplyQuaternion3f(vec4 quat1, vec4 quat2)
{
	/*float w;

	vec3 part1;
	vec3 part2;
	vec3 part3;

	vec3 sumParts;

	w = quat1.x * quat2.x - (float)MF.dotProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));
	part1 = vec3(quat1.x * quat2.y, quat1.x * quat2.z, quat1.x * quat2.w);
	part2 = vec3(quat2.x * quat1.y, quat2.x * quat1.z, quat2.x * quat1.w);
	part3 = MF.crossProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));

	sumParts = part1 + part2 + part3;

	return vec3(sumParts.x, sumParts.y, sumParts.z);*/

	__m128 reg1;
	__m128 reg2;
	__m128 reg_part1;
	__m128 reg_part2;
	__m128 reg_part3;

	vec3 part3;

	reg1 = _mm_set_ps(0.0f, quat2.y,quat2.z, quat2.w);
	reg2 = _mm_set_ps(0.0f, quat1.x,quat1.x, quat1.x);

	reg_part1 = _mm_mul_ps(reg1, reg2);

	reg1 = _mm_set_ps(0.0f, quat1.y,quat1.z, quat1.w);
	reg2 = _mm_set_ps(0.0f, quat2.x,quat2.x, quat2.x);

	reg_part2 = _mm_mul_ps(reg1, reg2);

	part3 = MF.crossProduct(vec3(quat1.y,quat1.z,quat1.w) , vec3(quat2.y,quat2.z,quat2.w));

	reg_part3 = _mm_set_ps(0.0f, part3.x, part3.y, part3.z);

	reg1 = _mm_add_ps(reg_part1, reg_part2);
	reg1 = _mm_add_ps(reg1, reg_part3);

	return vec3(reg1.m128_f32[2], reg1.m128_f32[1], reg1.m128_f32[0]);
}

Quaternion::~Quaternion(void)
{
}