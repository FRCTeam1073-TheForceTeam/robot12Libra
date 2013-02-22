#ifndef SMARTGYRO_H
#define SMARTGRRO_H
#include "WPILib.h"

class SmartGyro : public Gyro
{
public:
	SmartGyro(UINT32 channel, float offsetAngle);
	float GetAngle();
	void QuickZero();

private:
	float zeroOffset;
	float offsetAngle;

};

#endif
