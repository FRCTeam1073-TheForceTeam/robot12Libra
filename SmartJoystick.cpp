#include "SmartJoystick.h"
#include <math.h>

//for your sanity; some override methods have been provided
SmartJoystick::SmartJoystick(int port): Joystick(port){
	//scaleFactor = 1;		// linear response
}

bool yInversion = false;	// set to true to invert y axis

float SmartJoystick::GetX()	{return Joystick::GetX(); }
float SmartJoystick::GetY()	{return (yInversion ? -1 : 1) * Joystick::GetY(); }
float SmartJoystick::GetRawY(){return Joystick::GetY();}
float SmartJoystick::GetZ()	{return Joystick::GetZ(); }

//actual smart aspects of the SMART joystick class
bool SmartJoystick::ButtonDownEvent(UINT32 button)
{
	StateMap::iterator it = buttonStates.find(button);
	bool current = GetRawButton(button);	
	if (it == buttonStates.end())
	{
		buttonStates[button] = current;
		return false;
	}
	bool old = (*it).second;
	it->second = current;
	return !old && current;
}

bool SmartJoystick::ButtonUpEvent(UINT32 button)
{
	StateMap::iterator it = buttonStates.find(button);
	bool current = GetRawButton(button);	
	if (it == buttonStates.end())
	{
		buttonStates[button] = current;
		return false;
	}
	bool old = (*it).second;
	it->second = current;
	return old && !current;
}

/*void SmartJoystick::SetScaleFactor(float inScaleFactor)
{
	scaleFactor = inScaleFactor;
}*/

float SmartJoystick::ScaleJoystick(float axisVal, float scaleFactor)
{
	return (scaleFactor * pow(axisVal, 3) + (1-scaleFactor)*axisVal);
}


