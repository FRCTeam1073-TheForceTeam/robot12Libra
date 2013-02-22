#ifndef TOGGLE_DRIVE_MODE_H
#define TOGGLE_DRIVE_MODE_H

#include "../CommandBase.h"

typedef enum {kRobotMode, kFieldMode } DriveMode;

/**
 *
 *
 * @author GREG (Cowan)
 */
class SetMecanumDriveMode : public CommandBase
{
public:
	SetMecanumDriveMode(DriveMode mode);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	DriveMode desiredMode;
};

#endif

