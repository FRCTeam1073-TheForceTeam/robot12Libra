#ifndef FLUSH_SHOOTER_GROUP_H
#define FLUSH_SHOOTER_GROUP_H
#include "../CommandBase.h"
#include "ShootAtSpeed.h"
#include "CollectorReverse.h"
#include "SHOOTERONOFF.h"
#include "WaitCommand.h"
class WaitShoot : public CommandGroup{
public:
	WaitShoot(){
		AddSequential(new EvinWaitCommand(0.5));
		AddSequential(new ShootAtSpeed(-750));
		AddSequential(new EvinWaitCommand(4.5));
		AddSequential(new ShooterOff());
	}
};
class FlushShooter : public CommandGroup{
public:
	static bool buttonOne;
	static bool buttonTwo;
	static double originalSpeed;
	FlushShooter(bool one, int i, double originalSpeedBeforeFlush){
		puts("flush button one pressed");
		FlushShooter::buttonOne = one;
		originalSpeed = originalSpeedBeforeFlush;
		init();
	}
	FlushShooter(bool two, float f, double originalSpeedBeforeFlush){
		puts("flush button two pressed...");
		FlushShooter::buttonTwo = two;
		originalSpeed = originalSpeedBeforeFlush;
		init();
	}
	void init(){
		if (FlushShooter::buttonOne && FlushShooter::buttonTwo){
			puts("Flush Shooter Init");
			FlushShooter::buttonOne = false;
			FlushShooter::buttonTwo = false;
			AddParallel(new CollectorReverse(5.0));
			AddParallel(new WaitShoot());
		}
	}
	void End(){
		reset();
	}
	void Interrupted(){
		reset();
	}
	ShootAtSpeed *backToNormal;
	void reset(){
		puts("Flush Sequence interrupted or ended");
		puts("Defaulting back to speed prior to flush");
		backToNormal = new ShootAtSpeed(originalSpeed);
		backToNormal->Start();
	}
};
#endif
