#ifndef Arm_H
#define Arm_H
#include "Commands/Subsystem.h"
#include "../UserDefinitions.h"
#include "WPILib.h"
#define DOWN_SPEED -1.0
#define UP_SPEED 0.5
#define ARM_OFF 0.0
#define BUFFER 20
#define STALL_DETECTION_LIMIT 0.3
class Arm: public Subsystem {
private:
	DigitalInput *upLimitSwitch;
	DigitalInput *downLimitSwitch;
	Victor *armMotor;
	AnalogChannel *encoder;
	float *data;
	int delay;
	bool hasArrayInit;
public:
	Arm();
	Arm(DigitalInput *upLimit, DigitalInput *downLimit, Victor *motor, AnalogChannel *magEncoder);
	void ResetArray();
	void InitDefaultCommand();
	void Move(float speed);
	void FillArray();
	bool Stall();
	void PrintStallData();
	bool isUpLimitPressed();
	bool isDownLimitPressed();
	float lastSetVoltage;
	float GetSpeed();
	
};

#endif
