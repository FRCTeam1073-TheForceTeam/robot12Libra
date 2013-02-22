#include "LED.h"
#include "wpilib.h"
#include "ShooterOneWheel.h"
#include "forceipc.h"
#include "../RPC1073.h"
ShooterOneWheel::ShooterOneWheel(SmartSpeedCANJaguar *firstMotor) : Shooter(){
	this->wheelJag = firstMotor;
	Initialize();
}
void ShooterOneWheel::Initialize(){
	printf("\n\nSHOOTER ONE WHEEL INIT\n\n");
	rpmCalculation = new RPMCalc(.103f,.00901f,.294f,.95f);
	shootSpeed = DefaultRPM();
	//wheelJag->SetPID(.7,.01,.55);
	//wheelJag->SetPID(.7,.01,0);
	//wheelJag->SetPID(.5,.01,0);  // This wasn't bad
	wheelJag->SetPID(.4,.003,0);
}
void ShooterOneWheel::SetCANJaguars(float value){
	wheelJag->SetWheelSpeedRevolutionsPerMinute(-1 * value );
}
float ShooterOneWheel::DefaultRPM(){
	return 2500;   /*2500 is the RPM for 8 to 12 feet
					 some evidence shows that 2450 might
					 actually be ideal instead */
}
float ShooterOneWheel::GetCurrentShooterSpeed(){
	return wheelJag->GetWheelSpeed();
}
bool ShooterOneWheel::SafeToShoot(){
			const float too_slow  = 200.0f;
			return !(fabs(GetCurrentShooterSpeed()) <= too_slow);			
}

#define derp 0
#if derp
int ShooterOneWheel::DashboardShot(char *buf, int len) {
	//wheelJag->SetWheelSpeedRevolutionsPerMinute(2500);
	RPC1073 msg(buf, len);
	wheelJag->Set((float)msg.GetParameterInt(0));
	return 0;
}
#endif
