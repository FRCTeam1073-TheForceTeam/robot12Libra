#ifndef TURRET_H
#define TURRET_H


#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "SmartCANJaguar.h"
#include "../SmartJoystick.h"
#include "../RPC1073.h"

class Turret : public Subsystem, public PIDSource, public PIDOutput
{
public:
	Turret(SmartCANJaguar *canMotor);
	
	void Stop();  // Stop the turret motor();

	// Support for turning left & right
	void TurnLeft();
	void TurnRight();
	
	float GetDegrees() {return targetDegrees;} //was an int method

	// Limit Support
	bool IsAnyLimitPressed();
	bool IsLeftLimit();
	bool IsRightLimit();

	void InitDefaultCommand();
	bool IsStallDetected() { return false;	}
		
	void TurnRelativeRads(float rads);  //
	void TurnRelativeDegrees(float deg); //
	void DriveManualControl(SmartJoystick *joystick);
	bool IsAuto();
	
	float GetEncoderDegrees();
	void DriveMotorToAngle();
	bool IsAtAngle();
	
	int dashBoardcallback(char * buffer, int length);
	static int callback(char * buffer, int length);
	
	typedef enum  { TurretAuto, TurretManual } TurretMode;
private:

	TurretMode turretMode;
	TurretMode DefaultMode();
	float targetDegrees;
	SmartCANJaguar *canMotor;
	SmartJoystick *joystick;
	void ZeroEncoder();
	float manualDegrees;
	bool isDefaultMode;
	double PIDGet()	{return this->GetEncoderDegrees()/* - targetDegrees*/ ;}
	//void PIDWrite(float output);
	void SetMotor(float val);

	void PIDWrite(float f);
	
};
#endif


