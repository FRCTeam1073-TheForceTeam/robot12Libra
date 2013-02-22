#ifndef BUTTON_LED_H
#define BUTTON_LED_H
#include "../CommandBase.h"
class ButtonLED : public CommandBase
{	
public:
	ButtonLED() {Requires(led);}
	virtual void Initialize() {}
	virtual void Execute()
	{
		if(led->IsOn()) led->Off();
		else led->On();
	}
	virtual bool IsFinished() {return true;}
	virtual void End() {}
	virtual void Interrupted(){}
};
#endif

