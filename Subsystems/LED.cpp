#include "LED.h"
#include "wpilib.h"
#include "relay.h"
#define ILLUMINATOR_ON 1
#define ILLUMINATOR_OFF 0
LED::LED(Relay *illuminator) : Subsystem("LED")	
{
	this->illuminator = illuminator;
	On();
	
}
void LED::On() 
{
	illuminator->Set(Relay::kForward);
	isOn=true;
	printf("LED::On(%4.1f)\n", DriverStation::GetInstance()->GetMatchTime());
}
void LED::Off() 
{
	illuminator->Set(Relay::kOff);
	isOn=false;
	printf("LED::Off(%4.1f)\n", DriverStation::GetInstance()->GetMatchTime());
}

