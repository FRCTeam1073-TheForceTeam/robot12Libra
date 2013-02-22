#ifndef UPDATE_DRIVER_STATION_H
#define UPDATE_DRIVER_STATION_H
#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "../Subsystems/DriverstationMessages.h"	//i'd imagine we'd need this...
#include "../JoystickButtonSheet.h"
#include "../SmartJoystick.h"

class UpdateDriverstation : public CommandBase
{
public:
	UpdateDriverstation();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
};
#endif
