#ifndef DRIVEWITHJOYSTICKS_H
#define DRIVEWITHJOYSTICKS_H

#include "../CommandBase.h"

/*
 * @author cherian
 */
class DriveWithJoysticks: public CommandBase {
private:
	
public:
	DriveWithJoysticks();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	bool bKey;
};

#endif
