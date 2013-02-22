#include "CollectorStart.h"

CollectorStart::CollectorStart() {
	Requires(collector1);
}

// Called just before this Command runs the first time
void CollectorStart::Initialize() {
	printf("Collector(%4.1f): Started\n", DriverStation::GetInstance()->GetMatchTime());
	collector1->StartManagingBalls();
}

// Called repeatedly when this Command is scheduled to run
void CollectorStart::Execute() {
	collector1->ManageBalls();
}

// Make this return true when this Command no longer needs to run execute()
bool CollectorStart::IsFinished() {
	return collector1->IsDoneManaging();
}

// Called once after isFinished returns true
void CollectorStart::End() {
printf("Collector(%4.1f): Ended\n", DriverStation::GetInstance()->GetMatchTime());
	collector1->AllMotorsOff();
	collector1->ChangeGlobalShooterVarible();
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void CollectorStart::Interrupted() {
printf("Collector(%4.1f): Interrupted\n",DriverStation::GetInstance()->GetMatchTime());
	collector1->AllMotorsOff();
}
