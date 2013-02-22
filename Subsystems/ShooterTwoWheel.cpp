#include "LED.h"
#include "wpilib.h"
#include "ShooterTwoWheel.h"

static const float backSpin = .30;

ShooterTwoWheel::ShooterTwoWheel(SmartSpeedCANJaguar *upMotor, SmartSpeedCANJaguar *downMotor)
	: Shooter()
{
	this->upShooterMotorJag = upMotor;
	this->downShooterMotorJag = downMotor;
	
	rpmCalculation = new RPMCalc(.103,.00901,.294,.95);
	shootSpeed = DefaultRPM();
	
	upShooterMotorJag->SetPID(.7,.01,.55);
	downShooterMotorJag->SetPID(.7,.01,.55);}

void ShooterTwoWheel::SetCANJaguars(float value)
{
	upShooterMotorJag->SetWheelSpeedRevolutionsPerMinute(- (value - value * backSpin)  );
	downShooterMotorJag->SetWheelSpeedRevolutionsPerMinute(- (value + value * backSpin) );
}

float ShooterTwoWheel::DefaultRPM()
{
	return 896;
}
