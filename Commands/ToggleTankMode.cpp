#include "ToggleTankMode.h"

ToggleTankMode::ToggleTankMode() {
	//Requires(mecanumDrive);

}

// Called just before this Command runs the first time
void ToggleTankMode::Initialize() {
	printf("Changing to tank\n");	
}

// Called repeatedly when this Command is scheduled to run
void ToggleTankMode::Execute() {
	mecanumDrive->SetArcadeMode();
}

// Make this return true when this Command no longer needs to run execute()
bool ToggleTankMode::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void ToggleTankMode::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ToggleTankMode::Interrupted() {
}
