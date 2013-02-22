#include "../UserDefinitions.h"
#include "SWPIDCANJaguar.h"

static const float encoderRatio = 4.67;

static int printCount = 0;

int printNow(int divisor)
{
	if ((++printCount % divisor) == 0)
		return 1;
	else
		return 0;
}


SWPIDCANJaguar::SWPIDCANJaguar(UINT8 deviceNum, int encPulsesPerRev, float gearRatio, bool isEncReversed, bool isMotReversed)
:SmartCANJaguar(deviceNum, isEncReversed, isMotReversed), 
 PIDSource(),
 encoderPulsesPerRevolution(encPulsesPerRev),
 pid_P(0), pid_I(0), pid_D(0),
 lastSetSpeed(-1)
{
	this->gearRatio = gearRatio;

	id = deviceNum;
	count = 0;
	
	lastTime = Timer::GetFPGATimestamp();
	lastPos = this->GetPosition();

}

double SWPIDCANJaguar::GetMotorSpeed() {
	curTime = Timer::GetFPGATimestamp();
	curPos = this->GetPosition();
	
	double timeChange = (double) (curTime - lastTime);	// code for wrap?
	double posChange = (curPos - lastPos) /* * directionFactor */;
	
	lastRpm = (posChange * 60.0 * encoderRatio) / timeChange;
	
	lastTime = curTime;
	lastPos = curPos;
/*	if (printNow()) {
		printf("motor RPM: %f\n", rpm);
	} */

	if(id == 4)
		id = 4;
	
	return lastRpm;
}

double SWPIDCANJaguar::GetLastMotorSpeed() {
	return lastRpm;
}
double SWPIDCANJaguar::PIDGet() {
	return (this->GetMotorSpeed());
}

void SWPIDCANJaguar::PIDWrite(float output) {
/*	if (printNow()) {
		printf("PIDWrite called with %f\n", output);
	} */
	
	float tempOut = output / MAX_RPM; 
	
	// Check for NaN 
	if(tempOut != tempOut)
	{
		printf("SWPIDCANJaguar::PIDWrite output NAN\n");
		tempOut = 1;
	}

	if(tempOut > 1) { 
		printf("SWPIDCANJaguar::PIDWrite output %f too high\n", tempOut);
		tempOut = 1;
	}
	if(tempOut < -1) {
		printf("SWPIDCANJaguar::PIDWrite output %f too low\n", tempOut);
		tempOut = -1;
	}
	
/*	if(printNow()) {
		printf("PIDWrite calling Set with %f\n\n", tempOut);
	} */

	lastMotorOutput = tempOut;
	Set(tempOut);
}

double SWPIDCANJaguar::GetWheelPosition() {
	return this->GetPosition() * gearRatio / encoderRatio; 
}

double SWPIDCANJaguar::GetFeetPosition()
{
	double pos = GetPosition() * feetPerRev; 

		return pos;
}







