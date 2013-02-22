#ifndef ShooterOneWheel_H
#define ShooterOneWheel_H

#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "Shooter.h"

class ShooterOneWheel : public Shooter
{
public:
	ShooterOneWheel(SmartSpeedCANJaguar *firstMotor);
	float GetCurrentShooterSpeed();
	bool SafeToShoot();
	void Initialize();
private:
	virtual void SetCANJaguars(float value);
	virtual float DefaultRPM();
	SmartSpeedCANJaguar *wheelJag;
};
#endif
