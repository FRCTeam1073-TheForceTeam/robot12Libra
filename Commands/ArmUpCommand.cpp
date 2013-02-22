#include "ArmUpCommand.h"

ArmUpCommand::ArmUpCommand() {
	Requires(arm);
}

// Called just before this Command runs the first time
void ArmUpCommand::Initialize() {
	arm->Move(UP_SPEED);
}

// Called repeatedly when this Command is scheduled to run
void ArmUpCommand::Execute()
{
	arm->Move(UP_SPEED);
}

// Make this return true when this Command no longer needs to run execute()
bool ArmUpCommand::IsFinished() {
	return arm->Stall() || arm->isUpLimitPressed();
}

// Called once after isFinished returns true
void ArmUpCommand::End() {
	arm->Move(ARM_OFF);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ArmUpCommand::Interrupted() {
	arm->Move(ARM_OFF);
}
