#ifndef BACKUP_UNTIL_YOU_SEE_BACKBOARD_H
#define BACKUP_UNTIL_YOU_SEE_BACKBOARD_H
#include "../CommandBase.h"
#include "../Subsystems/MecanumDrive.h"
class BackupUntilYouSeeBackboard : public CommandBase
{
public:
	BackupUntilYouSeeBackboard()
	{
		SetTimeout(4.0);
		Requires(robot);
		Requires(shooter);
		drive = robot->Get();
	}
	void Initialize() {
		drive->MecanumDriveAction(0,1,0,0);}
	void Execute()
	{}
	bool IsFinished()	
	{
		return ((shooter->GetDistance() != 0) || IsTimedOut());
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
