#ifndef __GRAVITY__
#define __GRAVITY__

#include "../EsgiGL/Common/EsgiUtils.h"
#include "../EsgiGL/EsgiGL.h"

class Gravity
{
public:
	Gravity(void);
	Gravity(float gravity);
	void setGravity(float gravity);
	void setUseGravity(bool use);
	bool getUseGravity();
	void resetTime();
	void setWeight(float weight);
	void addForce(vec3 force, vec3* velocity, float elapsedTime);
	void applyForce(vec3* velocity, float elapsedTime);
	vec3 getForce();
	void setResistance(float mass, float resistance);
	void setBounciness(float mass, float bounciness);
	float getResistance();
	float getBounciness();
	~Gravity(void);

private:
	float time;
	float gravity;
	bool useGravity;
	float weight;
	vec3 force;

	float resistance;
	float bounciness;
};

#endif