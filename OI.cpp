#include "OI.h"
#include "SmartJoystick.h"
#include "LibraKraken.h"
#include "Commands/ToggleDriveMode.h"
#include "Commands/ToggleFinesseMode.h"
#include "Commands/ToggleTankMode.h"
#include "Commands/BridgeDown.h"
#include "Commands/ArmDownCommand.h"
#include "Commands/ArmUpCommand.h"
#include "Commands/ToggleBrake.h"
#include "Commands/CollectorStart.h"
#include "Commands/CollectorMoveShooter.h"
#include "Commands/ToggleDrivermessages.h"
#include "Commands/ArmOff.h"
#include "Commands/CollectorOff.h"
#include "Commands/CollectorOn.h"
#include "Commands/TurretCommands.h"
#include "Commands/ShootCommandButton.h"
#include "Commands/SetJoystickScalingCommand.h"
#include "Commands/ShooterIncrementSpeed.h"
#include "Commands/CollectorReverse.h"
#include "Commands/ButtonLED.h"
#include "Commands/DummyTurret.h"
#include "Commands/FlushShooterGroup.h"

bool FlushShooter::buttonOne = false;
bool FlushShooter::buttonTwo = false;
double FlushShooter::originalSpeed = 0.0;
OI::OI() {
	// Process operator interface input here.
	driveStick = new SmartJoystick(DRIVE_STICK);
	operatorStick = new SmartJoystick(OPERATOR_STICK);
	
	ArmConstruction();
	TurretConstruction();
	DriveConstruction();
	CollectorConstruction();
	ShooterConstruction();
	MiscConstruction();
		
	printf("OI::init done\n");
	return;
}
void OI::ArmConstruction(){
	//arm up.
	operatorArmUpButton = new JoystickButton(operatorStick, OPERATOR_ARM_UP_BUTTON);
	operatorArmUpButton->WhileHeld(new ArmUpCommand());
	operatorArmUpButton->WhenReleased( new ArmOff() );
	
	//arm down.
	operatorArmDownButton = new JoystickButton(operatorStick, OPERATOR_ARM_DOWN_BUTTON);
	operatorArmDownButton->WhileHeld(new ArmDownCommand());
	operatorArmDownButton->WhenReleased( new ArmOff() );
	
	driverArmDownButton = new JoystickButton(driveStick, DRIVER_ARM_DOWN_BUTTON);
	driverArmDownButton->WhileHeld(new ArmDownCommand());
	driverArmDownButton->WhenReleased( new ArmOff() );
	
	driverArmUpButton = new JoystickButton(driveStick, DRIVER_ARM_UP_BUTTON);
	driverArmUpButton->WhileHeld(new ArmUpCommand());
	driverArmUpButton->WhenReleased( new ArmOff() );		
}
void OI::TurretConstruction(){
	//mobile base has no turret!
	if(IsLibraKraken()){
		//	degreesDown = new JoystickButton(driveStick, TURRET_LEFT_DEGREES);
		//	degreesUp = new JoystickButton(driveStick, TURRET_RIGHT_DEGREES);

		turretManualButton = new JoystickButton(operatorStick, TURRET_MANUAL_BUTTON);
		turretManualButton->WhileHeld(new TurretManualTurn(false));
		turretManualButton->WhenReleased(new TurretManualTurn(true));
	}
}

void OI::DriveConstruction(){
	
	semiCubicButton = new JoystickButton(driveStick, SEMI_CUBIC_MOTION_BUTTON);
	semiCubicButton->WhenPressed(new SetJoystickScalingCommand(SEMI_CUBIC_MOTION_BUTTON));
	semiCubicButton->WhenReleased(new SetJoystickScalingCommand(JOYSTICK_CUBIC_MODE));

	linearDriveButton = new JoystickButton(driveStick, LINEAR_MOTION_BUTTON);
	linearDriveButton->WhenPressed(new SetJoystickScalingCommand(JOYSTICK_LINEAR_MODE));
	linearDriveButton->WhenReleased(new SetJoystickScalingCommand(JOYSTICK_CUBIC_MODE)); 
	

	setTankModeButton = new JoystickButton(driveStick, TANK_MODE_BUTTON);
	setTankModeButton->WhenPressed(new ToggleTankMode());

	fieldModeButton = new JoystickButton(driveStick, FIELD_MODE_BUTTON);
	fieldModeButton->WhenPressed(new SetMecanumDriveMode(kFieldMode));
	
	robotModeButton = new JoystickButton(driveStick, ROBOT_MODE_BUTTON);
	robotModeButton->WhenPressed(new SetMecanumDriveMode(kRobotMode));	
	
	if (IsLibraKraken()){
		toggleBrakeFirstButton = new JoystickButton(driveStick, TOGGLE_BRAKE_FIRST_BUTTON);
		toggleBrakeFirstButton->WhenPressed(new ToggleBrake(1));
		
		toggleBrakeSecondButton = new JoystickButton(driveStick, TOGGLE_BRAKE_SECOND_BUTTON);
		toggleBrakeSecondButton->WhenPressed(new ToggleBrake(2));	
	}
}

void OI::CollectorConstruction(){

	collectorOnButton = new JoystickButton(operatorStick, COLLECTOR_ON_BUTTON);
	collectorOnButton->WhenPressed(new CollectorStart());

	
	collectorAlwaysOnButton = new JoystickButton(operatorStick, COLLECTOR_ALWAYS_ON_BUTTON);
	collectorAlwaysOnButton->WhenPressed(new CollectorOn());	

	moveShooter = new JoystickButton(operatorStick, MOVE_TO_SHOOTER_BUTTON);
	moveShooter->WhenPressed(new CollectorMoveShooter());
	
	flushButtonOne = new JoystickButton(driveStick, SHOOTER_FLUSH_ONE_BUTTON);
	flushButtonTwo = new JoystickButton(driveStick, SHOOTER_FLUSH_TWO_BUTTON);
		
	//disabled for battlecry... causes fatal crash!
	//flushButtonOne->WhenReleased(new FlushShooter(true, 2, CommandBase::shooter->GetShootSpeed()));
	//flushButtonTwo->WhenReleased(new FlushShooter(true, 3.0f, CommandBase::shooter->GetShootSpeed()));
	
	//poorly named. use to turn off the collector...
	collectorPurge = new JoystickButton(operatorStick, PURGE_COLLECTOR_BUTTON);
	collectorPurge->WhileHeld( new CollectorReverse());
	collectorPurge->WhenReleased( new CollectorOff());

}

void OI::ShooterConstruction(){
	shooterSpeedDownButton = new JoystickButton(operatorStick, SHOOTER_SPEED_DOWN_BUTTON);
	shooterSpeedDownButton->WhenPressed(new ShooterIncrementSpeed(false));

	shooterSpeedUpButton = new JoystickButton(operatorStick, SHOOTER_SPEED_UP_BUTTON);
	shooterSpeedUpButton->WhenPressed(new ShooterIncrementSpeed(true));

	shooterOffButton = new JoystickButton(operatorStick, SHOOTER_OFF_BUTTON);
	shooterOffButton->WhenPressed(new ShootCommandButton(true));

	shooterOnButton = new JoystickButton(operatorStick, SHOOTER_ON_BUTTON);
	shooterOnButton->WhenPressed(new ShootCommandButton(false));
	
	shooterFineAdjButton = new JoystickButton(operatorStick, SHOOTER_FINE_ADJ_BUTTON);
	shooterFineAdjButton->WhenPressed(new ShooterFineSpeed(true));
	shooterFineAdjButton->WhenReleased(new ShooterFineSpeed(false));	
}

void OI::MiscConstruction(){
	//empty intentionally... anything else can be thrown in here if need be.
}

SmartJoystick * OI::GetDriveStick() {
	return driveStick;
}

SmartJoystick *OI::GetOperatorStick() {
	return operatorStick;
}
