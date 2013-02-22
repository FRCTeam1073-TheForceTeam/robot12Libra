#ifndef RAMP_COMMAND_H
#define RAMP_COMMAND_H
#include "../CommandBase.h"
#include "../Subsystems/SmartRampClimber.h"
class RampCommand : public CommandBase
{
public:
	RampCommand();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	
};
#endif

