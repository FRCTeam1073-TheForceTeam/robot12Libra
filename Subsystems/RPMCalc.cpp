
#include "RPMCalc.h"
#include <math.h>

#define pi 3.14159
#define gravity 9.81

RPMCalc::RPMCalc(float r, float I, float m, float e)
{
	radius = r;
	mOfInertia = I;
	mass = m;
	error = e;
}

float RPMCalc::GetRPM(float distance, float height, float angle)
{
	/*float velocity = sqrt(gravity*distance*distance/2/cos(angle)/cos(angle)/(distance*tan(angle)-height));
	float vOverSpin = sqrt(1.0/radius/radius+mass/mOfInertia/error);
	float rpm = 30/pi*velocity*vOverSpin;*/
	float rpm = 0;
	if(height>4 && distance<14)rpm = 2500;
	else if(height>4 && distance<15.5)rpm = 2600;
	else if(height>4 && distance<17.5)rpm = 2750;
	else if(height>4&&distance<22)rpm=3000;
	else if(height<4&&distance<5)rpm=1370;
	else if(height<4)rpm = 1500;//This is a guess. Untested.
	return rpm;
}

float RPMCalc::GetRPM1Wheel(float distance, float height, float angle)
{
	float velocity = sqrt(gravity*distance*distance/2/cos(angle)/cos(angle)/(distance*tan(angle)-height));
	float vOverSpin = sqrt(4.0/radius/radius+mass/mOfInertia/error);
	float rpm = 30/pi*velocity*vOverSpin;
		return rpm;
}

float RPMCalc::GetError(float distance, float heightHit, float angle, float rpm)
{
	float velocity = sqrt(gravity*distance*distance/2/cos(angle)/cos(angle)/(distance*tan(angle)-heightHit));
	float angV = pi*rpm/30;
	float error = mass/2/mOfInertia/(angV*angV/velocity/velocity-1/radius/radius);
	return error;
}

float RPMCalc::GetRPMAfterLaunch(float distance, float height, float angle)
{
	float velocity = sqrt(gravity*distance*distance/2/cos(angle)/cos(angle)/(distance*tan(angle)-height));
	return velocity/radius*30/pi;
}

float RPMCalc::GetEnergy(float rpm)
{
	float spinV = rpm*pi/30;
	return spinV*spinV*mOfInertia;
}

float RPMCalc::BallEnergy(float distance, float height, float angle)
{
	float velocity = sqrt(gravity*distance*distance/2/cos(angle)/cos(angle)/(distance*tan(angle)-height));
	return velocity*velocity*mass/2;
}
