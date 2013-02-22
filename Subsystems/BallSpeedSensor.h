#ifndef BALLSPEEDSENSOR_H_
#define BALLSPEEDSENSOR_H_
#define WINDOW 200
#include "wpilib.h"
#include "../userdefinitions.h"
class BallSpeedSensor
{
public:
	BallSpeedSensor(int port);	// Simple Constructor
	bool IsBallDetectEnabled();	// Is the Ball Sensor Enabled/Minitoring ?
	void EnableBallDetect();	// Enable the system for Ball Speed Detect
	void DisableBallDetect();	// Disable the Ball detect logic
	void BallSpeedSensor::EnableInterupt();//enables interupts
	float GetBallSpeedFeetPerSec();  // Get the actual speed
	void ProcessInterrupt();	// PUBLIC ONLY so it can be called by "C" style interrupt handler
	int GetBallCount();
	void Start();
	void PrintInfo();
	float GetMinSpeed();
	float GetMaxSpeed(); 
	float GetAverage();
	float GetRange();
private:
	enum BallDetectState { NotWaiting, WaitingForBallDetect, WaitingForBallExit};
	DigitalInput input;
	BallDetectState state;
	double time1, time2;
	double elapsedTime;
	int ballCount;
	float lastSpeed;
	float speeds[WINDOW];
	int hist_counter;
	
};
#endif
