#include "SetJoystickScalingCommand.h"

SetJoystickScalingCommand::SetJoystickScalingCommand(float factor) {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
}

// Called just before this Command runs the first time
void SetJoystickScalingCommand::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void SetJoystickScalingCommand::Execute() {
}

// Make this return true when this Command no longer needs to run execute()
bool SetJoystickScalingCommand::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void SetJoystickScalingCommand::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void SetJoystickScalingCommand::Interrupted() {
}
