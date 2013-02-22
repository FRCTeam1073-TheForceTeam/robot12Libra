#include "CollectorOn.h"

CollectorOn::CollectorOn() {
	Requires(collector1);
	Requires(shooter);
}

void CollectorOn::Initialize() { 
	printf("CollectorOn::Initialize()\n");	
}

void CollectorOn::Execute() {
	if (shooter->SafeToShoot())
		collector1->AllMotorsOn();
	else {
		puts("Not safe to shoot, bro.");
		this->Cancel();
	}
}
bool CollectorOn::IsFinished() {
	return shooter->SafeToShoot();
}

void CollectorOn::End() {}

void CollectorOn::Interrupted(){}
