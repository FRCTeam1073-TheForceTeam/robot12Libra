#include "Robot1073.h"
#include "../Commands/DriveWithJoysticks.h"
#include "../UserDefinitions.h"
#include "Math.h"
#include "../CommandBase.h"

Robot1073 *robot1073;

Robot1073::Robot1073() : Subsystem("Robot") {

	
robot1073 = this;
navigation = CommandBase::navigation;

#ifdef DEBUG_VERBOSE
	printf("Robot1073::Robot1073() called\n");
#endif
	drive = CommandBase::mecanumDrive;
#ifdef DEBUG_VERBOSE
	printf("Robot::Robot() finished\n");
#endif
	
	drive->MecanumDriveAction(0.0, 0.0, 0.0, 0.0);
}
    
void Robot1073::InitDefaultCommand() {
#ifdef DEBUG_VERBOSE
	//Set default command like this:
	printf("Robot::InitDefaultCommand to Drive with joysticks\n");
#endif
	SetDefaultCommand(new DriveWithJoysticks());
}

void Robot1073::JoystickDrive(SmartJoystick *joy){
	
	// Check that we have a gyro before trying to get the angle
	float angle = 0.0;

	if (CommandBase::gyro != NULL)
	{
		// Subtract angle from 180 as robot points 180 degrees opposit from gyro..
		angle = 180.0 - CommandBase::gyro->GetAngle();
	}
	//printf("Robot::JoystickDrive was called with %f %f %f %f\n", joy->GetX(), joy->GetY(), joy->GetZ(), CommandBase::gyro->GetAngle());
	drive->MecanumDriveAction(joy->ScaleJoystick(joy->GetX(), 1.0), joy->ScaleJoystick(joy->GetY(), 1.0), joy->ScaleJoystick(joy->GetZ(), 1.0), angle);
}

#ifdef ENV_VISUALSTUDIO
void Robot1073::KeyboardDrive(){

	if(GetAsyncKeyState('A') < 0)
		robot1073->drive->MecanumDriveAction(1,0,0,0);
	else if(GetAsyncKeyState('S') < 0)
		drive->MecanumDriveAction(0,1,0,0);
	else if(GetAsyncKeyState('D') < 0)
		drive->MecanumDriveAction(-1,0,0,0);
	else if(GetAsyncKeyState('W') < 0)
		drive->MecanumDriveAction(0,-1,0,0);
	else if(GetAsyncKeyState('Q') < 0)
		drive->MecanumDriveAction(0,0,1,0);
	else if(GetAsyncKeyState('E') < 0)
		drive->MecanumDriveAction(0,0,-1,0);
	else
		drive->MecanumDriveAction(0,0,0,0);


	if(GetAsyncKeyState('9') < 0){
		CommandBase::turret->TurnRelativeDegrees(90);
	}
	
	if(GetAsyncKeyState('0') < 0){
		CommandBase::turret->TurnRelativeDegrees(-90);
	}


}
#endif

MecanumDrive* Robot1073::Get() 
{
	return drive;
}
