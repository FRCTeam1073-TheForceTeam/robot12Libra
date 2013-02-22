#include "SmartGyro.h"


SmartGyro::SmartGyro(UINT32 channel, float offsetDegrees )
: Gyro(channel),
  zeroOffset(0.0f),
  offsetAngle(offsetDegrees)
{

}
	
float 
SmartGyro::GetAngle()
{
	float raw = Gyro::GetAngle();
	return (raw - zeroOffset) + offsetAngle;
}

void 
SmartGyro::QuickZero()
{
	zeroOffset = Gyro::GetAngle();
}
