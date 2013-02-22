#include "WPILib.h"
#include "Commands/Command.h"

#include "Commands/AutonomousGroup.h"
#include "Commands/MobileBaseAutonomous.h"
#include "CommandBase.h"
#include "LibraKraken.h"
#include "CommandBasedRobot.h"

#include "Subsystems/ShooterOneWheel.h"

CommandBasedRobot::CommandBasedRobot() {
	
}
	
void CommandBasedRobot::RobotInit() {
	if (IsLibraKraken())
		printf("\n\nLibraKraken " __DATE__ " " __TIME__ "\n" __FILE__ "\n\n" );
	else
		printf("\n\nMobile base " __DATE__ " " __TIME__ "\n" __FILE__ "\n\n" );
	CommandBase::init();
	autonomousCommand = new AutonomousGroup();
	//	richardCommand = new MobileBaseAutonomous();
} 
	 void CommandBasedRobot::DisabledPeriodic(){
		// While disabled, we have some code like DriverStation and DashboardSender
		// that should run
		CommandBase::driverstationMessages->PeriodicService();
		CommandBase::dashboardSender->DisablePeriodic();
		CommandBase::PeriodicService();
	}
	
	 void CommandBasedRobot::AutonomousInit() { 
		if (CommandBase::isLibraKraken())
		 autonomousCommand->Start();
		else richardCommand->Start();
	}
	
	 void CommandBasedRobot::AutonomousPeriodic() {
		CommandBase::driverstationMessages->PeriodicService();
		Scheduler::GetInstance()->Run();
		CommandBase::PeriodicService();
#ifdef ENV_VISUALSTUDIO
		CommandBase::DoSimulate();
#endif

	}
	 void CommandBasedRobot::TeleopInit() {
		 printf("Teleop Init at time %f", DriverStation::GetInstance()->GetMatchTime());
		if (CommandBase::isLibraKraken()){
			CommandBase::shooter->MotorsOff();
			CommandBase::collector1->AllMotorsOff(); 
		}
		CommandBase::shooter->Initialize();
	 }
	
	 void CommandBasedRobot::TeleopPeriodic() {
		CommandBase::driverstationMessages->PeriodicService();
		Scheduler::GetInstance()->Run();
		CommandBase::PeriodicService();
#ifdef ENV_VISUALSTUDIO
		CommandBase::DoSimulate();
#endif

	}

START_ROBOT_CLASS(CommandBasedRobot);

