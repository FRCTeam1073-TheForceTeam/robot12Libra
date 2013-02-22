#ifndef BRAKES_H
#define BRAKES_H


#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "SmartCANJaguar.h"

class Brakes : public Subsystem
{
public:
	Brakes(Servo *leftFrontBrake, Servo *leftBackBrake, Servo *rightFrontBrake, Servo *rightBackBrake);
	void ToggleBrake();
	bool AreBrakesOn() { return isBrakeOn; }
private:
	Servo *leftFrontBrake;
	Servo *leftBackBrake;
	Servo *rightFrontBrake;
	Servo *rightBackBrake;
	bool isBrakeOn;
	void LowerMotorSpeed(SmartCANJaguar *motor);
};
#endif
