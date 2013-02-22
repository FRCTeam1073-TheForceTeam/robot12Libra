#include "CollectorOnAutonomous.h"


CollectorOnAutonomous::CollectorOnAutonomous() {
	Requires(collector1);
	Requires(shooter);
}

bool CollectorOnAutonomous::isAutonomousFinished() {
	const float fast = 2300;
	return shooter->GetCurrentShooterSpeed() == fast;
}
	

void CollectorOnAutonomous::Execute() {
	if (isAutonomousFinished())
		collector1->AllMotorsOn();
}

bool CollectorOnAutonomous::IsFinished() {
	return isAutonomousFinished();
}

void CollectorOnAutonomous::Interrupted(){
	printf("CollectorOn interrupted turning all motors off\n");
	collector1->AllMotorsOff();
	//We'd like to turn the motors off because when autonomous ends, we don't want them running
}
