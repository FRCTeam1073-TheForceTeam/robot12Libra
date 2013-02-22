#include "CollectorReverse.h"

CollectorReverse::CollectorReverse() {
	// Use requires() here to declare subsystem dependencies
	// eg. requires(chassis);
	Requires(collector1);
	isTimeOut = false;
}

CollectorReverse::CollectorReverse(bool b) {
	
	Requires(collector1);
	off = b;
	isTimeOut = false;
}
CollectorReverse::CollectorReverse(double time_out)
{
	Requires(collector1);
	isTimeOut = true;
	SetTimeout(time_out);
}
// Called just before this Command runs the first time
void CollectorReverse::Initialize() {
	printf("collector REVERSED\n");
	collector1->AllMotorsReverse();
}

// Called repeatedly when this Command is scheduled to run
void CollectorReverse::Execute() {}

// Make this return true when this Command no longer needs to run execute()
bool CollectorReverse::IsFinished() {
	if (isTimeOut)
		return IsTimedOut();
	else
		return (false || off);
}

// Called once after isFinished returns true
void CollectorReverse::End() {
	collector1->AllMotorsOff();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CollectorReverse::Interrupted() {
	collector1->AllMotorsOff();
}
