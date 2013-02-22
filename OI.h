#ifndef OI_H
#define OI_H
#include "UserDefinitions.h"
#include "WPILib.h"
#include "SmartJoystick.h"
#include "JoystickButtonSheet.h"
class OI {
private:
	void ArmConstruction();
	void TurretConstruction();
	void DriveConstruction();
	void ShooterConstruction();
	void CollectorConstruction();
	void MiscConstruction();
	int degrees;
	SmartJoystick *driveStick;
	SmartJoystick *operatorStick;

	
	//flush shooter
	JoystickButton *flushButtonOne;
	JoystickButton *flushButtonTwo;	//safety... press both.
	
	// I put thse in order to help me make sure I have them all
	JoystickButton *linearDriveButton;
	JoystickButton *semiCubicButton;
	JoystickButton *driverArmUpButton;
	JoystickButton *driverArmDownButton;
	JoystickButton *toggleBrakeFirstButton;
	JoystickButton *toggleBrakeSecondButton;
	// Drivestation messages done the old way, button 10
	JoystickButton *fieldModeButton;
	JoystickButton *robotModeButton;
	JoystickButton *setTankModeButton;
	
	//JoystickButton *turretRelativeUp;
	//JoystickButton *turretRelativeDown;
	
	// Operator stick
	JoystickButton *moveShooter;
	JoystickButton *collectorOnButton;
	JoystickButton *shooterSpeedDownButton;
	JoystickButton *operatorArmDownButton;
	JoystickButton *shooterSpeedUpButton;
	JoystickButton *operatorArmUpButton;
	JoystickButton *collectorPurge;
	JoystickButton *collectorAlwaysOnButton;
	JoystickButton *shooterOffButton;
	JoystickButton *shooterOnButton;
	JoystickButton *shooterFineAdjButton;
	JoystickButton *turretManualButton;

	JoystickButton *degreesUp;
	JoystickButton *degreesDown;
	
public:
	OI();
	SmartJoystick * GetOperatorStick();
	SmartJoystick * GetDriveStick();
	
};

#endif
