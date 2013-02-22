#ifndef DRIVE_H
#define DRIVE_H

#include "../UserDefinitions.h"
#include "WPILib.h"
#include "SmartCANJaguar.h"
#include "Commands/Subsystem.h"
#include "SWPIDCANJaguar.h"

/*
 * @author cherian
 */
class MecanumDrive : public Subsystem {
public:
	MecanumDrive(SWPIDCANJaguar *rfmj, SWPIDCANJaguar *rbmj, SWPIDCANJaguar *lfmj, SWPIDCANJaguar *lbmj, Gyro *g);
	void InitDefaultCommand();
//	float Deadzone(float speed, float deadzone);  not used
	float CubeSpeed(float speed, float magnitude);
	bool IsEncoderAtDistance(float feet, SWPIDCANJaguar *jag);
	bool AreEncodersAtDistance(float feet);
	void MecanumDriveAction(float joyX, float joyY, float joyTwist, float robAng);
	void SetFieldMode(bool isFieldMode); // false for robot mode
	void ToggleFinesseMode();
	void SetArcadeMode(); 
	void KillWheelMotors();
	bool HaveIReachedTheDesiredAngle(float possibleDesierdAngle, float ANGLE_TOLERANCE);
	float GetDesiredAngle(float angle);
	bool MotorsAreOff();
	bool isTankDriveModePublic();
	bool isFinesseModePublic();
	bool isFieldOrientedPublic();
	float Get() {return gyro->GetAngle();}
	int dashBoardcallback(char * buffer, int length);
	int callback(char * buffer, int length);
	void Spin(bool positive);
private:
	void MecanumScale();
	void Scale();
	//sfloat GetMax();
	void SetMotors();
	
	Gyro *gyro;
	
	SWPIDCANJaguar *rightFrontMotorJaguar;
	SWPIDCANJaguar *rightBackMotorJaguar;
	SWPIDCANJaguar *leftFrontMotorJaguar;
	SWPIDCANJaguar *leftBackMotorJaguar;

	float rightFront;
	float rightBack;
	float leftFront;
	float leftBack;

	bool panrightfl;
	bool panrightfr;
	bool panrightbl;
	bool panrightbr;
	
	bool isFieldOriented;
	bool isFinesseMode;
	bool isTankDriveMode;

	int encoderFail;

	
	// It's desirable that everything possible under private except
	// for methods that implement subsystem capabilities0
};

#endif
