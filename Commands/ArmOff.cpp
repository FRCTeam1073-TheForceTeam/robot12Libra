#include "ArmOff.h"

ArmOff::ArmOff() {
	// Use requires() here to declare subsystem dependencies
	Requires(arm);
}

// Called just before this Command runs the first time
void ArmOff::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void ArmOff::Execute() {
	printf("Arm Off\n");
	arm->Move(0.0);
}

// Make this return true when this Command no longer needs to run execute()
bool ArmOff::IsFinished() {
	return arm->GetSpeed() == 0.0;
}

// Called once after isFinished returns true
void ArmOff::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ArmOff::Interrupted() {
}
