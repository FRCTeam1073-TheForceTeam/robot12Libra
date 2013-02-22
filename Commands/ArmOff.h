#ifndef ARM_OFF_H
#define ARM_OFF_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Ken Cowan
 */
class ArmOff : public CommandBase {
public:
	ArmOff();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
