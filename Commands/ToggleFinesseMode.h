#ifndef TOGGLE_FINESSE_MODE_H
#define TOGGLE_FINESSE_MODE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Greg (Cowan)
 */
class ToggleFinesseMode: public CommandBase {
public:
	ToggleFinesseMode();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
