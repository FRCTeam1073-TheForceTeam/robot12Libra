#include "CollectorMoveShooter.h"
#include "../CommandBase.h"
#include "CollectorStart.h"

CollectorMoveShooter::CollectorMoveShooter() {
	Requires(shooter);
	Requires(collector1);
}


bool klugeShot2 = false;

void CollectorMoveShooter::Initialize() {
	// stuff
	klugeShot2 = true;


	double timeout = shooter->SafeToShoot() ? 2.5 : 0;
	if (shooter->SafeToShoot())
	{
		printf("CollectorMoveShooter(%4.1f): Initialize\n",  DriverStation::GetInstance()->GetMatchTime());
		collector1->startShooterSequence();
	}
	else{
		printf("CollectorMoveShooter(%4.1f): Attempt to shoot but not safe\n",  DriverStation::GetInstance()->GetMatchTime());
	}
	SetTimeout(timeout);  // 2.5 second timeout if shooter active, else 0	
}

void CollectorMoveShooter::Execute() {}

bool CollectorMoveShooter::IsFinished() {
	return collector1->isShootingFinished() || IsTimedOut();
}

void CollectorMoveShooter::End() {
	if (shooter->SafeToShoot())	
	{
		CommandBase::readyToShoot = false;
		collector1->AllMotorsOff();
		if(!DriverStation::GetInstance()->IsAutonomous()){
			collectorStart = new CollectorStart();
			collectorStart->Start();
		}
		printf("CollectorMoveShooter(%4.1f): End\n",  DriverStation::GetInstance()->GetMatchTime());
		// jfh to fix linker problems CommandBase::canShoot = true;
	}
	else puts("CollectorMoveShooter did nothing because shooter not running");
}
void CollectorMoveShooter::Interrupted() {
	collector1->AllMotorsOff();
	printf("CollectorMoveShooter(%4.1f): Interrupted\n",  DriverStation::GetInstance()->GetMatchTime());
}
