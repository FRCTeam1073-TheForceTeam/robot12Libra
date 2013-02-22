#ifndef AUTONOMOUS_SHOOTER_COMMAND_H
#define AUTONOMOUS_SHOOTER_COMMAND_H
#include "../CommandBase.h"
#include "TurretCommands.h"
#include "CollectorMoveShooter.h"
#include "CollectorOn.h"
#include "CollectorOff.h"
#include "ShootCommandButton.h"	//actually has nothing to do with a button...
#include "ShootAtSpeed.h"
#include "WaitCommand.h"
#include "TurretTurnToAngle.h"
static const float SHOOTSPEED = 2400.0f; /* mr. r has data that suggests a 2450
											rpm would provide better results
										  */
static const float STARTSPEED = 100.0f;
class AutonomousShooterCommand : public CommandGroup{
public: 
	AutonomousShooterCommand(){
		bool stayOn = true;
		float shootingspeed=STARTSPEED;
		while(shootingspeed<SHOOTSPEED)
		{
			AddSequential(new ShootAtSpeed(shootingspeed));
			shootingspeed*=2;
		}
		AddSequential(new ShootAtSpeed(SHOOTSPEED));
		AddSequential(new WaitCommand(2.0f));	//battlecry ROAR!
												//^ kate's "battlecry"
		AddSequential(new CollectorOn());
		if (!stayOn){
			AddSequential(new WaitCommand(5.0f));	// allow time for balls to be fired
			AddSequential(new CollectorOff());
			AddSequential(new ShootAtSpeed(0.0f));
		}
	}
	void Initialize(){
//		Navigation has set the shooter speed based on initial position but not turned the motors on
//		CommandBase::shooter->SetSpeed(2500.0f);//hard code from heden
//		CommandBase::shooter->MotorsOn();
	}
	void Execute(){ 
//		CommandBase::shooter->SetSpeed(shootSpeed);
//		CommandBase::shooter->MotorsOn();
	}
};
#endif
