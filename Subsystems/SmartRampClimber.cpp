#include "SmartRampClimber.h"


SmartRampClimber::SmartRampClimber() : Subsystem("Smart Ramp Climber")
{
	leftF = CommandBase::leftFrontJag;
	rightF = CommandBase::rightFrontJag;
	leftR = CommandBase::leftBackJag;
	rightR = CommandBase::rightBackJag;
}
float SmartRampClimber::HowFarFromBeingBalanced()
{
	//insert trig here...
	return 0.0;
}
bool SmartRampClimber::IsBalancedEnough()
{
	return (currentPos > LEFT_ACCEL_THRESHOLD || currentPos < RIGHT_ACCEL_THRESHOLD);
}
void SmartRampClimber::GetBalancedEnough()
{
		bool direction = currentPos >= 0;
		Drive(DRIVE_SPEED, direction);
}

void SmartRampClimber::Stop()
{
	Drive(0.0, true);
}
void SmartRampClimber::Drive(float speed, bool direction)
{
	float f = direction ? speed : -speed;
	leftF->Set(f);
	rightF->Set(f);
	leftR->Set(f);
	rightR->Set(f);	
}
