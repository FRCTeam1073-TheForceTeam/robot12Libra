#ifndef SMARTCANJAGUAR_H
#define SMARTCANJAGUAR_H
#include "WPILib.h"

class SmartCANJaguar : public CANJaguar
{
public:
	SmartCANJaguar(UINT8 deviceNumber, bool isEncReversed=false, bool isMotReversed= false, CANJaguar::ControlMode controlMode = kPercentVbus);
	int GetErrorCount();
	void ResetEncoder()
	{
		puts("Zero'ing Encoder!");
	   	CANJaguar::ControlMode old = CANJaguar::GetControlMode();	//inherits from can jag
	   	SetPositionReference(kPosRef_QuadEncoder);
	   	ConfigEncoderCodesPerRev(1440);
	   	ChangeControlMode(kPosition);
	   	EnableControl(0.0);
	   	ChangeControlMode(old);
	   	EnableControl();
	}
	
	void Set(float value, UINT8 syncGroup=0);
	float GetBusVoltage();
	float GetOutputVoltage();
	float GetOutputCurrent();
	float GetTemperature();
	double GetPosition();
	double GetSpeed();
	bool GetForwardLimitOK();
	bool GetReverseLimitOK();

	void ReverseMotorEncoder();
	
	
private:
	bool isEncoderReversed;
	bool isMotorReversed;
	int errorCount;
	
};

#endif
