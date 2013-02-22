#ifndef ROBOT_H
#define ROBOT_H
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "MecanumDrive.h"
#include "../SmartJoystick.h"
#include "Navigation.h"

class Robot1073;

extern Robot1073 *robot1073;
/**
 * @author cherian
 */
class Robot1073: public Subsystem {
public:
	Robot1073();
	void JoystickDrive(SmartJoystick *joy);
	void InitDefaultCommand();
	MecanumDrive *Get();
	void DriveToAngle(float angle);
	Navigation *navigation;

#ifdef ENV_VISUALSTUDIO
	void KeyboardDrive();
#endif

private:
	MecanumDrive *drive;
};

#endif
