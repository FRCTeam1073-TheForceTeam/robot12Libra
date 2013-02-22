#ifndef CAMERA_H
#define CAMERA_H


#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"

class LED : public Subsystem
{
public:
	LED(Relay *illuminator);
	void On();
	void Off();
	bool IsOn() {return isOn;}
private:
	bool isOn;
	Relay *illuminator;
};
#endif
