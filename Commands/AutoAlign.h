#ifndef AUTO_ALIGN_H
#define AUTO_ALIGH_H
#include "../CommandBase.h"
#include "../Subsystems/MecanumDrive.h"
#define SMALL_THRESHOLD 1.0
class AutoAlign : public CommandBase
{
public:
	AutoAlign()
	{
		Requires(robot);
		Requires(shooter);
		drive = robot->Get();
	}
	void Initialize()
	{
		int i = shooter->GetAngle() > 0 ? 1 : -1;
		drive->MecanumDriveAction(i, 0, 0, 0);
	}
	void Execute() {}
	bool IsFinished()
	{
		return shooter->GetAngle() > -SMALL_THRESHOLD && shooter->GetAngle() < SMALL_THRESHOLD;
	}
	void End() {drive->KillWheelMotors();}
	void Interrupted() {drive->KillWheelMotors();}
private:
	MecanumDrive *drive;
};
#endif
