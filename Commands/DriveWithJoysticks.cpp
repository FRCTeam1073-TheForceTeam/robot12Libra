#include "DriveWithJoysticks.h"

DriveWithJoysticks::DriveWithJoysticks() {
	Requires(robot);
	Requires(mecanumDrive);
	bKey = false;
}

// Called just before this Command runs the first time
void DriveWithJoysticks::Initialize() {
	
}

// Called repeatedly when this Command is scheduled to run
void DriveWithJoysticks::Execute() {
	//if(CommandBase::safe) 
#ifdef ENV_VISUALSTUDIO
		if(GetAsyncKeyState('K') < 0/* && !hasStarted*/){
			bKey = !bKey;
		}
		if(bKey){
			robot->KeyboardDrive();
		}
		else{
			robot->JoystickDrive(oi->OI::GetDriveStick());
		}
#else 
		robot->JoystickDrive(oi->OI::GetDriveStick());
		
		
#endif

}

// Make this return true when this Command no longer needs to run execute()
bool DriveWithJoysticks::IsFinished() {
	return false;
}

// Called once after isFinished returns true
void DriveWithJoysticks::End() {
	
}

// Called when another command which requires one or more of the same
// subsystems is scheduled to run
void DriveWithJoysticks::Interrupted() {
}
