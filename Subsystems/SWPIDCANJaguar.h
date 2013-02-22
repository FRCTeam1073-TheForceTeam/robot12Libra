#ifndef SWPIDCANJAGUAR_H_
#define SWPIDCANJAGUAR_H_
#define feetPerRev 0.524f
#include "wpilib.h"
#include "SmartCANJaguar.h"
int printNow(int divisor);

class SWPIDCANJaguar : public SmartCANJaguar, public PIDSource
{
public:
	SWPIDCANJaguar(UINT8 deviceNumber, int encPulsesPerRev, float gearRatio, bool isEncReversed=false, bool isMotReversed= false);
	void SetWheelSpeedRevolutionsPerMinute(float speed);
	double GetFeetPosition();
	double GetWheelPosition();
	double GetLastMotorSpeed();
	double GetLastMotorOutput() { return lastMotorOutput; }
	double GetMotorSpeed();
	void SetPID(double p, double i, double d);
	void PIDWrite(float output);
	double PIDGet();

	
	
	float gearRatio;
	void Initialize();
	int encoderPulsesPerRevolution;
	double pid_P, pid_I, pid_D;		
	double lastSetSpeed;
	int id;
	int count;
	
	double lastTime;
	double curTime;
	float lastPos;
	float curPos;
	double lastRpm;
	double lastMotorOutput;
private:
	double fpr;

};


#endif
