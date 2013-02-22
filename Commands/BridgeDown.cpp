#include "BridgeDown.h"
#include "../Subsystems/Arm.h"
BridgeDown::BridgeDown() {Requires(arm);}

// Called just before this Command runs the first time
void BridgeDown::Initialize()
{
	printf("BRIDGE DOWN CMD INIT\n");
	arm->Move(DOWN_SPEED);
}

// Called repeatedly when this Command is scheduled to run
void BridgeDown::Execute()
{
	arm->Move(DOWN_SPEED);
}

// Make this return true when this Command no longer needs to run execute()
bool BridgeDown::IsFinished()
{
	return arm->Stall() || arm->isDownLimitPressed();
}

// Called once after isFinished returns true
void BridgeDown::End()
{
	arm->Move(ARM_OFF);
}

void BridgeDown::Interrupted()
{
	arm->Move(ARM_OFF);
}
