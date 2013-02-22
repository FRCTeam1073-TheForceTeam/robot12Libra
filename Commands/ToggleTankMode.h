#ifndef TOGGLE_TANK_MODE_H
#define TOGGLE_TANK_MODE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Greg (Cowan)
 */
class ToggleTankMode: public CommandBase {
public:
	ToggleTankMode();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
};

#endif
