#ifndef SMART_RAMP_CLIMBER_H
#define SMART_RAMP_CLIMBER_H
#define RIGHT_ACCEL_THRESHOLD 10
#define LEFT_ACCEL_THRESHOLD -RIGHT_ACCEL_THRESHOLD
#define DRIVE_SPEED 4453 //set that later... idk
#define RAMP_OFF 0	//we will probs need to interrupt the command
#include "Math.h"
#include "../CommandBase.h"
#include "Commands/Subsystem.h"
#include "WPIlib.h"
class SmartRampClimber : public Subsystem
{
public:
	SmartRampClimber();
	float HowFarFromBeingBalanced();
	void GetBalancedEnough();
	bool IsBalancedEnough();
	void Stop();
private:
	
	//not needed for local use perse, just my ease.
	SmartCANJaguar *leftF;
	SmartCANJaguar *rightF;
	SmartCANJaguar *leftR;
	SmartCANJaguar *rightR;
	void Drive(float speed, bool direction);
	float currentPos;	/*y accel for the sake of my
						 *pseudo code... for the time 
						 * being
						 */ 
};

#endif
