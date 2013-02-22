#ifndef ACTUALLY_DRIVE_BACK_TO_BRIDGE_H
#define ACTUALLY_DRIVE_BACK_TO_BRIDGE_H
#include "../CommandBase.h"
#include "../Subsystems/MecanumDrive.h"
#define BACK_DISTANCE 4.58
#define ROBOT_SIZE 2.83
#define FINAL_DISTANCE BACK_DISTANCE - ROBOT_SIZE
class ActuallyDriveBackToBridge : public CommandBase
{
public:
	ActuallyDriveBackToBridge()
	{
		SetTimeout(5.0);	/*
							if encoders fail we can come back and set
							this to something reasonable
							*/
		Requires(robot);
		drive = robot->Get();
	}
	void Initialize()
	{
		drive->MecanumDriveAction(0,1,0,0);
	}
	void Execute() {}
	bool IsFinished()
	{
		return drive->AreEncodersAtDistance((float)FINAL_DISTANCE) || IsTimedOut();
	}
	void End()
	{
		drive->KillWheelMotors();
	}
	void Interrupted()
	{
		drive->KillWheelMotors();
	}
private:
	MecanumDrive *drive;
};

#endif
