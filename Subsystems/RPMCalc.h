#ifndef __RPMCalc_h__
#define __RPMCalc_h__

class RPMCalc
{
public:
	RPMCalc(float r, float I, float m, float e);
	float GetRPM(float distance, float height, float angle);
	float GetRPM1Wheel(float distance, float height, float angle);
	float GetError(float distance, float heightHit, float angle, float rpm);
	float GetRPMAfterLaunch(float distance, float height, float angle);
	float GetEnergy(float rpm);
	float BallEnergy(float distance, float height, float angle);
private:
	float radius;
	float mOfInertia;
	float mass;
	float error;
};

#endif
