#ifndef _COMMANDBASEDROBOT_H_
#define  _COMMANDBASEDROBOT_H_

#include "WPILib.h"
#include "Subsystems/Navigation.h"

class CommandBasedRobot : public IterativeRobot {
public:
	CommandBasedRobot();
	virtual void RobotInit();
	
	
	virtual void AutonomousInit();
	virtual void DisabledPeriodic();
	virtual void AutonomousPeriodic(); 
	virtual void TeleopInit();
	virtual void TeleopPeriodic(); 

	Navigation *navigation;
private:
	Command *autonomousCommand;
	Command *richardCommand;
};
#endif
