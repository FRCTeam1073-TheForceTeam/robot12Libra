#ifndef DRIVERSTATION_MESSAGES_H
#define DRIVERSTATION_MESSAGES_H

#include "../UserDefinitions.h"
#include "Commands/Subsystem.h"
#include "WPIlib.h"
#include "../SmartJoystick.h"
#include "SmartCANJaguar.h"
#include "BallSpeedSensor.h"


class DriverstationMessages : public Subsystem
{
public:
	DriverstationMessages();
	void Print(int line, char *data, ...);
	void DisplayMessage();
	void Toggle();
	void PeriodicService();
private:
	void PrintDefaultDiags();
	void PrintJoy(SmartJoystick *stick, char *label);
	void PrintJag(SmartCANJaguar *jag, char *label);
	void PrintDigitalIO(DigitalInput *dig1, DigitalInput *dig2, DigitalInput *dig3, char *label);
	void PrintVictor(Victor *vicky, char *label);
	void PrintLimits(DigitalInput *upLimit, DigitalInput *downLimit, char *label);
	void PrintGyro(Gyro* gyro, char * label);
	void PrintMagEncoder(AnalogChannel *encoder, char *label);
	void PrintFeetPosition();
	void SendTextLines();
	void PrintBallSpeedSensor(BallSpeedSensor *sensor, char *label);
	int index;
	char text[6][80];
};

#endif
