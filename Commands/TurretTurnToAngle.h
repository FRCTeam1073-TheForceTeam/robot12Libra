#ifndef TURRET_TURN_TO_ANGLE_H	//NOT relative angle; actual angle.
#define TURRET_TURN_TO_ANGLE_H
#include "TurretCommands.h"
#include "../CommandBase.h"
class TurretTurnToAngle : public CommandGroup
{
public:
	TurretTurnToAngle()
	{
		AddSequential(new TurretTurnLeftCommand());
		AddSequential(new TurretTurnRelativeAngleCommand());
	}
};
#endif








