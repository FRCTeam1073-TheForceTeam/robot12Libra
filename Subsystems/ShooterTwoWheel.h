#ifndef ShooterTwoWheel_H
#define ShooterTwoWheel_H

#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "Shooter.h"

class ShooterTwoWheel : public Shooter
{
public:
	ShooterTwoWheel(SmartSpeedCANJaguar *upMotor, SmartSpeedCANJaguar *downMotor);
private:
	virtual void SetCANJaguars(float value);
	virtual float DefaultRPM();
	SmartSpeedCANJaguar *upShooterMotorJag;
	SmartSpeedCANJaguar *downShooterMotorJag;

};
#endif
