#ifndef MOBILE_BASE_AUTONOMOUS_H2
#define MOBILE_BASE_AUTONOMOUS_H2
#include "../CommandBase.h"
#include "../Subsystems/MecanumDrive.h"
class MobileBaseAutonomous : public CommandBase{
public:
	MobileBaseAutonomous(){
		Requires(robot);
		drive = robot->Get();
		SetTimeout(3.0);
	}
	void Intialize(){
		drive->MecanumDriveAction(-1.0f, -1.0f, 0.0f, 0.0f);
	}
	void Execute(){
		
	}
	bool IsFinished(){
		return IsTimedOut();
	}
	void End(){
		Stop();
	}
	void Interrupted(){
		Stop();
	}
private:
	void Stop(){
		drive->MecanumDriveAction(0.0f, 0.0f, 0.0f, 0.0f);
	}
	MecanumDrive *drive;
};
#endif
