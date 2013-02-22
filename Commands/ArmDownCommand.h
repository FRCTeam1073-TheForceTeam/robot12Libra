#ifndef ArmDown_COMMAND_H
#define ArmDown_COMMAND_H
#include "../CommandBase.h"
#include "../Subsystems/Arm.h"

class ArmDownCommand: public CommandBase {
public:
	ArmDownCommand();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};
#endif
