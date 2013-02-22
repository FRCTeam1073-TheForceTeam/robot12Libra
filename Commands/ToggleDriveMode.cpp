#include "ToggleDriveMode.h"

SetMecanumDriveMode::SetMecanumDriveMode(DriveMode mode) {
	//Requires(mecanumDrive);
	desiredMode = mode;
}

// Called just before this Command runs the first time
void SetMecanumDriveMode::Initialize() {
	printf("Changing mecanum mode\n");	
}

// Called repeatedly when this Command is scheduled to run
void SetMecanumDriveMode::Execute() {
	if (desiredMode == kFieldMode)
		mecanumDrive->SetFieldMode(true);
	else
		mecanumDrive->SetFieldMode(false);
}

// Make this return true when this Command no longer needs to run execute()
bool SetMecanumDriveMode::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void SetMecanumDriveMode::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SetMecanumDriveMode::Interrupted() {
}
