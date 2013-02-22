#ifndef SHOOTER_H
#define SHOOTER_H
#include "Commands/Subsystem.h"
#include "SmartSpeedCANJaguar.h"
#include "WPILib.h"
#include "RPMCalc.h"

/*
 * @author cherian
 */
class Shooter: public Subsystem {
protected:
	virtual void SetCANJaguars(float value) = 0;
	virtual float DefaultRPM() = 0;
	
	static int callback(char * buffer, int length );
	static int speedCallback(char *buffer, int length); // for shooter speed presets
	RPMCalc *rpmCalculation;
	float shootSpeed;
	float increasedSpeed;
	bool motorsOn;
	
	
public:
	Shooter();

	virtual void Initialize() = 0;
	
	int dashBoardcallback(char * buffer, int length );
	void OperatorAim();
	void VisionTrack();
	virtual void MotorsOn();
	virtual void MotorsOff();
	void IncrementSpeed(float increment);
	virtual void SetShooterSpeedFPS(double distance, double height, double angle);
	void SetSpeed(float setSpeed);
	float GetShootSpeed();
	virtual float GetCurrentShooterSpeed() = 0;
	float GetIncreasedSpeed();
	void setManualOverride(bool manualOverride);
	float GetAngle() { return angle; }
	float GetDistance() { return distance; }
	float GetHeight() { return height; }
	virtual bool SafeToShoot() = 0;
private:
	float angle;
	float distance;
	float height;
	bool manualOverrideOn;
};

#endif
