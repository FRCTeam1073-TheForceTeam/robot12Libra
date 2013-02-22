#ifndef ArmUp_COMMAND_H
#define ArmUp_COMMAND_H
#include "../Subsystems/Arm.h"
#include "../CommandBase.h"
class ArmUpCommand: public CommandBase {
public:
	ArmUpCommand();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
