#ifndef DASHBOARD_SENDER_H
#define DASHBOARD_SENDER_H

#include "../UserDefinitions.h"
#include "Commands/Subsystem.h"
#include "SmartCANJaguar.h"
#include "Robot1073.h"



class DashboardSender;
class SmartJoystick;
class SmartGyro;
class SmartCANJaguar;
class SmartJaguarMotorEncoder;
class DashboardSender  : public Subsystem {
public:
	void InitDefaultCommand();
	DashboardSender();
	void SendData();
	//void SendConstants(void);
	void SetDashboardIndex(int val){index = val;}
	void DisablePeriodic();
private:
	void SendNav();
	void SendJag(SmartCANJaguar *toSend, char* label);
	void SendJoystick(SmartJoystick *toSend, int maxBtn, char* label);
	void SendDigitalIO(DigitalInput *toSend, char* label);
	void SendAnalogChannel(AnalogChannel *toSend, char* label);
	void SendI2C(ADXL345_I2C *toSend);
	void SendServo(Servo *toSend, char* label);
	void SendRelay(Relay *toSend);
	void SendVictor(Victor *toSend, char* label);
	void SendGyro(Gyro *toSend, char *label);
	void SendHeader();
	void SendTiltData();
	void MyAddString(char* value);
	void SendSettings();
	static int packetCount;
	int index;
};
#endif
