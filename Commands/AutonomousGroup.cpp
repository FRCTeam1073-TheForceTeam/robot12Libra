#include "AutonomousGroup.h"
#include "../CommandBase.h"
AutonomousGroup::AutonomousGroup()
{

	//can rearrange order
	if(CommandBase::isLibraKraken()){
		AddSequential(new AutonomousShooterCommand());
		//AddSequential(new AutoBridgeBackup()); // Uncomment when tested
	}
	else{
		//AddSequential(new MobileBaseAutonomous());
	}
}


