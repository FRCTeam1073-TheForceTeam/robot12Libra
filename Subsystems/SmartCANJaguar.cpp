#include "SmartCANJaguar.h"

#define CAN_TIMEOUT -44087

SmartCANJaguar::SmartCANJaguar(UINT8 deviceNumber, bool isEncReversed, bool isMotReversed, ControlMode controlMode)
:CANJaguar(deviceNumber, controlMode),
 isEncoderReversed(isEncReversed),
 isMotorReversed(isMotReversed)
{
	errorCount = 0;
	ResetEncoder();
}
int SmartCANJaguar::GetErrorCount()
{
	return errorCount;
}

void SmartCANJaguar::Set(float value, UINT8 syncGroup)
{
	float localValue = value;	/*i don't know how C++ lets you assin values to 
								  paramter variables... it probably should be fine
								  just checking by making a local variable...
								  */
	if(isMotorReversed) localValue = -localValue;
	CANJaguar::Set(localValue, syncGroup);
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
}

float SmartCANJaguar::GetBusVoltage()
{
	float result = CANJaguar::GetBusVoltage();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}

float SmartCANJaguar::GetOutputVoltage()
{
	float result = CANJaguar::GetOutputVoltage();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;	
}

float SmartCANJaguar::GetOutputCurrent()
{
	float result = CANJaguar::GetOutputCurrent();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}

float SmartCANJaguar::GetTemperature()
{
	float result = CANJaguar::GetTemperature();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}

double SmartCANJaguar::GetPosition()
{
	double result = CANJaguar::GetPosition();

	if(isEncoderReversed) result = -result;

	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}

double SmartCANJaguar::GetSpeed()
{
	double result = CANJaguar::GetSpeed();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}
bool SmartCANJaguar::GetForwardLimitOK()
{
	bool result = CANJaguar::GetForwardLimitOK();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}

bool SmartCANJaguar::GetReverseLimitOK()
{
	bool result = CANJaguar::GetReverseLimitOK();
	if (GetError().GetCode() == CAN_TIMEOUT)	// TBD: Make this "magic number" a constant
	{
		errorCount++;
		ClearError();
	}
	return result;
}


void 
SmartCANJaguar::ReverseMotorEncoder()
{
	isEncoderReversed = !isEncoderReversed;
	isMotorReversed = !isMotorReversed; 
}


