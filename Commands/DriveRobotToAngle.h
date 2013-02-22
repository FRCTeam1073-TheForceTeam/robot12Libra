#ifndef DRIVE_ROBOT_TO_ANGLE
#define DRIVE_ROBOT_TO_ANGLE
#define ANGLE_TOLERANCE 5.0
#include "../CommandBase.h"
class DriveRobotToAngle : public CommandBase
{
public:
	DriveRobotToAngle(float angle)
	{
		Requires(mecanumDrive);
		specifiedAngle = angle;
		desired = mecanumDrive->GetDesiredAngle(angle);
	}
	void Initialize() {}
	void Execute()
	{
		desired = mecanumDrive->GetDesiredAngle(angle);
		bool b = desired >= angle + Get();
		mecanumDrive->Spin(desire);
	}
	bool IsFinished() {
		return mecanumDrive->HaveIReachedTheDesiredAngle(desired, (float)ANGLE_TOLERANCE);
	}
	void End() 
	{
		puts("reached desired angle");
	}
	void IsInterrupted()
	{
		
	}
private:
	float specifiedAngle;
	float desired
};

#endif