#include "ToggleDrivermessages.h"

ToggleDrivermessages::ToggleDrivermessages() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(driverstationMessages);
}

// Called just before this Command runs the first time
void ToggleDrivermessages::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void ToggleDrivermessages::Execute() {
	driverstationMessages->Toggle();
}

// Make this return true when this Command no longer needs to run execute()
bool ToggleDrivermessages::IsFinished() {
	return true;
}

// Called once after isFinished returns true
void ToggleDrivermessages::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void ToggleDrivermessages::Interrupted() {
}
