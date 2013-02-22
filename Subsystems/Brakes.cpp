#include "Brakes.h"
#include "../CommandBase.h"
#define BRAKE_ON 0.56 //completely random
#define BRAKE_OFF 0.0 //this too (Calibrate ME!!!!)

Brakes::Brakes(Servo *leftFrontBrake, Servo *leftBackBrake, Servo *rightFrontBrake, Servo *rightBackBrake) : Subsystem("Brakes")
{
	this->leftFrontBrake = leftFrontBrake;
	this->leftBackBrake = leftBackBrake;
	this->rightFrontBrake = rightFrontBrake;
	this->rightBackBrake = rightBackBrake;
	isBrakeOn = false;
}

void Brakes::ToggleBrake()
{
	if (isBrakeOn) //then set all to off
	{
		leftFrontBrake->Set(BRAKE_OFF);
		leftBackBrake->Set(BRAKE_OFF);
		rightFrontBrake->Set(BRAKE_OFF);
		rightBackBrake->Set(BRAKE_OFF);
	}
	else
	{
		leftFrontBrake->Set(BRAKE_ON);
		leftBackBrake->Set(BRAKE_ON);
		rightFrontBrake->Set(BRAKE_ON);
		rightBackBrake->Set(BRAKE_ON);
	}
	isBrakeOn = !isBrakeOn;
	// jfh to fix linker problem CommandBase::safe = !CommandBase::safe;
}
