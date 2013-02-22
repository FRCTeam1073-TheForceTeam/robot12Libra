#ifndef AUTONOMOUS_INPUT_H
#define AUTONOMOUS_INPUT_H
#include "wpilib.h"
#include "Commands/Subsystem.h"
class AutonomousInput : public Subsystem
{
public:
	AutonomousInput() : Subsystem("AutonomousInput")
	{
			delay = false;
			backUpFirst = true;
	}
	
	void DelayGesture(int i)	{delay = i == 1;}	//when gesture is assumed; we send a 1 down to the robot.
	void DontBackup() {backUpFirst = false;}
	bool IsDelay() {return delay;}
	bool DoIBackUpFirst() {return backUpFirst;}
private:
	bool delay;
	bool backUpFirst;
};
#endif
