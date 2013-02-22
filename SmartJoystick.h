#ifndef SMART_JOYSTICK_H
#define SMART_JOYSTICK_H
#include "WPIlib.h"
class SmartJoystick : public Joystick
{
public:
	SmartJoystick(int port);
	SmartJoystick(Joystick *dumb);	//alternative constructor
	virtual float GetX();
	virtual float GetY();
	virtual float GetRawY();
	virtual float GetZ();
	bool ButtonDownEvent(UINT32 button);
	bool ButtonUpEvent(UINT32 button);
	//void SetScaleFactor(float inScaleFactor);
	float ScaleJoystick(float axisVal, float scaleFactor);
private:

protected:
	typedef map<UINT32, bool> StateMap; StateMap buttonStates;
};
#endif
