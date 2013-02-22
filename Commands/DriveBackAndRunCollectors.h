#ifndef DRIVE_BACK_AND_RUN_COLLECTORS_H
#define DRIVE_BACK_AND_RUN_COLLECTORS_H
#include "../CommandBase.h"
#include "../Subsystems/MecanumDrive.h"
class DriveBackAndRunCollectors : public CommandBase
{
public:
	DriveBackAndRunCollectors()
	{
		Requires(robot);
		Requires(collector1);
		Requires(shooter);
		drive = robot->Get();
		 
	}
	void Initialize()
	{
		shooter->MotorsOn();
		collector1->AllMotorsOn();
	//	shooter->SetSpeed(shooter->DistanceToRPM());
	}
	void Execute()	{}
	bool IsFinished() {return DriverStation::GetInstance()->IsAutonomous() || collector1->BallCount() == 3;}
	void End() {
		drive->KillWheelMotors();
		collector1->AllMotorsOff();
		shooter->MotorsOff();
	}
	void Interrupted() {
		drive->KillWheelMotors();
		collector1->AllMotorsOff();
		shooter->MotorsOff();
	}
private:
	MecanumDrive *drive;
	
};
#endif
