#ifndef SMARTSPEEDCANJAGUAR_H_
#define SMARTSPEEDCANJAGUAR_H_
#include "wpilib.h"
#include "SmartCANJaguar.h"
#define HISTORY_TIME 10
#define HISTORY_SIZE 200
#ifndef DIM
#define DIM(_x) ( sizeof(_x) / sizeof(_x[0]) )
#endif
class SmartSpeedCANJaguar : public SmartCANJaguar
{
public:
	SmartSpeedCANJaguar(UINT8 deviceNumber, int encPulsesPerRev, float gearRatio, bool isEncReversed=false);
	void SetWheelSpeedRevolutionsPerMinute(float speed);
	double GetWheelSpeed();
	
	void PeriodicService(double speed, bool isPressed, bool dumpButton, bool empt, bool avg);
	void SetPID(double p, double i, double d);
	double superMin, superMax;
	double superDelta;
private:
	int test;
	float gearRatio;
	float speedHistory[HISTORY_SIZE];
	void Initialize();
	bool isEncoderReversed;
	int encoderPulsesPerRevolution;
	double pid_P, pid_I, pid_D;		
	double lastSetSpeed;
	bool isInitialized;
	int id;
	void PrintExtremes();
	void PrintArray();
	void EmptyArray();
	void PrintAverage();
};
#endif








