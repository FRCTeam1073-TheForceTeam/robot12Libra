#ifndef BRIDGE_DOWN_H
#define BRIDGE_DOWN_H

#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "../Subsystems/Arm.h"
class BridgeDown: public CommandBase {
public:
	BridgeDown();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
