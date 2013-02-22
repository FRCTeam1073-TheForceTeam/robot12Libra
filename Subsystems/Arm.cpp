#include "Arm.h"
#include "../UserDefinitions.h"
Arm::Arm() : Subsystem("Arm") {
	downLimitSwitch = NULL;
	upLimitSwitch = NULL;
	armMotor = NULL;
	encoder = NULL;
	hasArrayInit = false;
}
Arm::Arm(DigitalInput *upLimit, DigitalInput *downLimit, Victor *motor, AnalogChannel *magEncoder) : Subsystem("Arm")
{
	downLimitSwitch = downLimit;
	upLimitSwitch = upLimit;
	armMotor = motor;
	encoder = magEncoder;
	data = new float[BUFFER];
	delay = 0;
}
void Arm::PrintStallData()
{
	for (int i = 0; i < (int) LENGTH(data); i++)
	{
		printf("%f  ", data[i]);
		if(i%4 == 0) printf("\n");
	}
}
void Arm::InitDefaultCommand() {}
void Arm::Move(float speed)
{
	if (speed != ARM_OFF) PrintStallData();
	armMotor->Set(speed);
	if (speed == ARM_OFF)
		ResetArray();
	else
		FillArray();
}

void Arm::FillArray()		
{
	float f = encoder->GetVoltage();
	data[delay] = f;
	printf("Adding:\t%f\n", f);
}
bool Arm::Stall()
{
	bool noEncoder = true;
	if (!hasArrayInit) return false;
	float one = data[delay];
	float two = data[BUFFER - delay];
	printf("V1:\t%f\nV2:\t%f\n", one, two);
	printf("\nDealy Value: %i\n", delay);
	int i = hasArrayInit ? 1 : 0;
	printf("Has the array been initialized?\t%i\n", i);
	
	if (delay == BUFFER)
	{
		delay = 0;
		hasArrayInit = true;
	}
	else {delay++; return false;}
	bool bIsStalled = fabs(one - two) < STALL_DETECTION_LIMIT;
	if (bIsStalled) printf("Arm stalled\n");
	if(!noEncoder)
	return bIsStalled;
	else return false;
}

// The arm limits return 0 when pressed
bool Arm::isUpLimitPressed()	{return upLimitSwitch->Get() == 0;}
bool Arm::isDownLimitPressed()	{return downLimitSwitch->Get() == 0;}

void Arm::ResetArray()
{
	for (int i = 0; i < (int) LENGTH(data); i++)
		data[i] = 0;
	delay = 0;
}
float Arm::GetSpeed()
{
	return armMotor->Get();
}
