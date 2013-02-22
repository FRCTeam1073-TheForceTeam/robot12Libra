#ifndef TOGGLE_BRAKE_H
#define TOGGLE_BRAKE_H

#include "../CommandBase.h"

/**
 *
 *
 * @author Greg (Cowan)
 */
class ToggleBrake: public CommandBase {
public:
	ToggleBrake(int button);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	static bool button1;
	static bool button2;
};

#endif
