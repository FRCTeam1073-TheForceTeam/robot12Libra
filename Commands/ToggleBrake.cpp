#include "ToggleBrake.h"
#include "../CommandBase.h"

bool ToggleBrake::button1 = false;
bool ToggleBrake::button2 = false;

ToggleBrake::ToggleBrake(int button) {
	Requires(brakes);
	Requires(mecanumDrive);
	
	if (button == 1)
		button1 = true;
	
	if (button == 2)
		button2 = true;
	
}
// Called just before this Command runs the first time
void ToggleBrake::Initialize() {
	if (button1 && button2)
		mecanumDrive->KillWheelMotors();
}

// Called repeatedly when this Command is scheduled to run
void ToggleBrake::Execute() {
	if (button1 && button2)
	{
		brakes->ToggleBrake();
		button1 = false;
		button2 = false;
		printf("BRAKE TOGGLED!!!");
	}
}

// Make this return true when this Command no longer needs to run execute()
bool ToggleBrake::IsFinished() {
	
	// This complicated.  By the IsFinished is called, both buttons will be false
	// only if Execute really toggled the brake
	if (!button1 && !button2)
	{
		//make sure brakes are off
		return mecanumDrive->MotorsAreOff();
	}
	
	return true;
}

// Called once after isFinished returns true
void ToggleBrake::End() {}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ToggleBrake::Interrupted() {

}
