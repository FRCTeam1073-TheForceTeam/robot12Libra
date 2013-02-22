#include "SmartSpeedCanJaguar.h"
SmartSpeedCANJaguar::SmartSpeedCANJaguar(UINT8 deviceNum, int encPulsesPerRev, float gearRatio, bool isEncReversed)
: SmartCANJaguar(deviceNum),
 isEncoderReversed(isEncReversed),
 encoderPulsesPerRevolution(encPulsesPerRev),
 pid_P(0), pid_I(0), pid_D(0),
 lastSetSpeed(-1)
{
	this->gearRatio = gearRatio;
	printf("Jag Constructor\n");
	test = 1;
	id = deviceNum;
	isInitialized = false;
	Initialize();  // KC added this.  Had someone removed it?
	for(int i = 0; i < HISTORY_SIZE; i++)
		speedHistory[i] = 0.1;
}
void SmartSpeedCANJaguar::SetWheelSpeedRevolutionsPerMinute(float motorSpeed)
{
	double speed = motorSpeed / gearRatio;	
	if(isInitialized == false){		//Have we ever set a specific speed ?
		Initialize();			// Initialize if not
		lastSetSpeed = 1;
		isInitialized = true;
	} /* necessary for Jag to work but don't know why - SC. */
	Set(speed);	printf("CANJag::Set(%4.1f) from SmartSpeedCANJaguar::SetWheelSpeedRPM\n", speed);
	lastSetSpeed = speed;
}
void SmartSpeedCANJaguar::SetPID(double p, double i, double d)
{
	printf("Jag Setpid\n");
	if(isEncoderReversed){
		p *= -1;
		i *= -1;	
	}
	// Store the PID paramaters incase we need to reset the jaguar...
	pid_P = p;
	pid_I = i;
	pid_D = d;
}
void SmartSpeedCANJaguar::Initialize()
{
	printf("SmartSpeedCanJaguar::Initialize()\n");
	ChangeControlMode(kSpeed);								// Set for speed mode
	SetSpeedReference(CANJaguar::kSpeedRef_Encoder);		// Define the encoder type
	ConfigEncoderCodesPerRev(encoderPulsesPerRevolution);	// Define the lines per revolution
	CANJaguar::SetPID(pid_P, pid_I, pid_D);					// Set our passed in PID constants
	EnableControl();										// Must enable the control paramaters									
	CANJaguar::Set(lastSetSpeed);							// Restore the last target speed...
//	printf("CANJag::Set(%4.1f) from SmartSpeedCANJaguar::Initialize\n", lastSetSpeed);
}
double SmartSpeedCANJaguar::GetWheelSpeed()
{
	double encoderRPM = GetSpeed();
	return encoderRPM * gearRatio;
}
void SmartSpeedCANJaguar::PrintArray()
{
	int counter = 0;
		for (size_t i = 0; i < DIM(speedHistory); i++)
		{
			if (counter++ % 10 == 0)
				printf("\n");
			printf("%4.2f, ", speedHistory[i]);	
		}
}
void SmartSpeedCANJaguar::PrintExtremes()
{
	double min, max;
	min = 10000;
	max = -3;
	for (size_t i = 0; i < DIM(speedHistory); i++)
	{
		min = (speedHistory[i] < min && speedHistory[i] != 0) ? speedHistory[i] : min;
		max = (speedHistory[i] > max) ? speedHistory[i] : max;
	}
	superMin = (min < superMin) ? min : superMin;
	superMax = (max > superMax) ? max : superMax;
	superDelta = superMax - superMin;
	double osc = max - min;
	/*if (id == 6)
		printf("Upper Wheel ");
	else
		printf("Lower Wheel "); */
	printf("Test Number %i\nMin Speed in MPH:\t%f\nMax Speed in MPH:\t%f\nOscilation MPH %f\n-------------------------------------------\n", test, min, max, osc);
	test++;
}
void SmartSpeedCANJaguar::PrintAverage()
{
	double d = 0;
	double count = sizeof(speedHistory)/sizeof(speedHistory[4]);
	for (int i = 0; DIM(speedHistory); i++)
		d += speedHistory[i];
	d /= count;
	float f = (float) d;
	printf("Average Wheel Speed is %f\n\n", f);
}
void SmartSpeedCANJaguar::EmptyArray()
{
	for (size_t i = 0; i < DIM(speedHistory); i++)
		speedHistory[i] = 0;
}
void SmartSpeedCANJaguar::PeriodicService(double speed, bool isPressed, bool dumpButton, bool empt, bool avg)
{
	static int counter = 0;
	int hist_counter = 0;
	
	speedHistory[hist_counter++ % HISTORY_SIZE] = (float)speed;
	//if(lastSetSpeed < 0)		// Do nothing until speed has been set
	//return;
	
	if(++counter %20 == 0){		// Let's just check every second or so rather thaan at 20 Hz, same some traffic...
		if(GetControlMode() != CANJaguar::kSpeed){
			printf("Speed Jaguar Reset, Reinitializing\n");
			Initialize();
		}	
	}
	if (isPressed) 
		PrintExtremes();
	if (dumpButton)
		PrintArray();
	if (empt)
		EmptyArray();
	if (avg)
		PrintAverage();
}
