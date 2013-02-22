#include "ArmDownCommand.h"

ArmDownCommand::ArmDownCommand() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(arm);
}

// Called just before this Command runs the first time
void ArmDownCommand::Initialize() {
	printf("ARM DOWN CMD INIT\n");
	arm->Move(DOWN_SPEED);
}

// Called repeatedly when this Command is scheduled to run
void ArmDownCommand::Execute() {arm->Move(DOWN_SPEED);}

// Make this return true when this Command no longer needs to run execute()
bool ArmDownCommand::IsFinished() {
	return (arm->Stall() || arm->isDownLimitPressed());
}

// Called once after isFinished returns true
void ArmDownCommand::End() {
	arm->Move(ARM_OFF);
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ArmDownCommand::Interrupted() {
	arm->Move(ARM_OFF);
}
