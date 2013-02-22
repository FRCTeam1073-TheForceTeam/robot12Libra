#ifndef TURRET_TURN_TO_ROBOT_ANGLE_H
#define TURRET_TURN_TO_ROBOT_ANGLE_H
#include "../CommandBase.h"

class TurretTurnToRobotAngle : public CommandBase
{
public:
	TurretTurnToRobotAngle()
	{
		Requires(turret);
		SetTimeout(0.5);
	}
	void Initialize()
	{
		turret->TurnRelativeDegrees(-turret->GetDegrees());
	}
	void Execute() {}
	bool IsFinished() {return IsTimedOut();}
	void End() {}
	void Interrupted() {}
};

#endif
