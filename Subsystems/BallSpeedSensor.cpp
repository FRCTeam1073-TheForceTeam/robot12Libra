#include "ballspeedsensor.h"
const float BallDiameterFeet = ((25.0 / 3.1415)/12.0);  // Should probably be somewhere else...


int BallSenseInterrupt(uint32_t mask, BallSpeedSensor *ballSpeedSensor)
{
	return 1; // temporarily disable ball speed sensor
	ballSpeedSensor->ProcessInterrupt();
	return 1;
}
void BallSpeedSensor::ProcessInterrupt()
{
	switch(state){
	case(WaitingForBallDetect):				// Must have detected leading edge of ball
		if(input.Get() == 0)
		{
			state = NotWaiting;
			time1 = Timer::GetFPGATimestamp();	// Get & store the exact time the Ball was first detected
			state = WaitingForBallExit;			// We are now waiting for no ball
		}
		break;
	case(WaitingForBallExit):				// Must have detected trailing edge of ball
		if(input.Get() == 1 )
		{
			state = NotWaiting;
			time2 = Timer::GetFPGATimestamp();	// Get & store the exact time the Ball was last detected
			ballCount++;
			elapsedTime = time2 - time1;	// Elapsed time in seconds with uSecond resolution
			lastSpeed    = BallDiameterFeet / elapsedTime;
			hist_counter++;
			speeds[hist_counter++ % WINDOW] = lastSpeed;
			state = WaitingForBallDetect;  		// reset state
			time1 = time2 = -1;					// reinit times
		}
		break;
	default: break;
	}
}
BallSpeedSensor::BallSpeedSensor(int port)
: input(port),
  state(NotWaiting),
  time1(-1), time2(-1)
{
	hist_counter = 0;
	printf("Creating BallSpeedSensor port %d\n", port);
#ifndef ENV_VISUALSTUDIO
	input.RequestInterrupts((tInterruptHandler)BallSenseInterrupt, this);
#endif
	input.SetUpSourceEdge(true, true);
	ballCount = 0;
	elapsedTime = 0.0;
	//Start();
}  
bool BallSpeedSensor::IsBallDetectEnabled()	{return (state == WaitingForBallDetect || state == WaitingForBallExit);}
void BallSpeedSensor::EnableBallDetect()
{
	state = WaitingForBallDetect;  // Set state
	time1 = time2 = -1; 							// Times not initialized
}
void BallSpeedSensor::DisableBallDetect()
{
	state = NotWaiting;  							// Set state
	time1 = time2 = -1; 							// Times not initialized
	input.SetUpSourceEdge(false, false);			
	input.DisableInterrupts();
}
float BallSpeedSensor::GetBallSpeedFeetPerSec()	{return lastSpeed;}
void BallSpeedSensor::EnableInterupt()	{input.EnableInterrupts();}
int BallSpeedSensor::GetBallCount()	{return ballCount;}
void BallSpeedSensor::Start()
{
	EnableBallDetect();
	EnableInterupt();	
}
void BallSpeedSensor::PrintInfo()	{printf("BallCount %d elapsedTime %f speed %f\n", ballCount, elapsedTime, GetBallSpeedFeetPerSec());}
float BallSpeedSensor::GetMinSpeed()
{
	float f = speeds[0];
	for (int i = 0; i < (int) LENGTH(speeds); i++)	f = speeds[i] && f!= 0.0f < f ? speeds[i] : f;
	return f;
}
float BallSpeedSensor::GetMaxSpeed()
{
	float f = speeds[0];
	for (int i = 0; i < (int) LENGTH(speeds); i++) f = speeds[i] && f!= 0.0f > f ? speeds[i] : f;
	return f;
}
float BallSpeedSensor::GetRange() {return GetMaxSpeed() - GetMinSpeed();}
float BallSpeedSensor::GetAverage()
{
	float f = 0.0f;
	for (int i = 0; i < (int) LENGTH(speeds); i++) f = speeds[i]!= 0 ? f + speeds[i] : f;
	return f/(float) LENGTH(speeds);
}





