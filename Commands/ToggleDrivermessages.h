#ifndef TOGGLE_DRIVRMESSAGES_H
#define TOGGLE_DRIVRMESSAGES_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Greg Cowan
 */
class ToggleDrivermessages: public CommandBase {
public:
	ToggleDrivermessages();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
