#ifndef COMMAND_BASE_H
#define COMMAND_BASE_H

#include "WPILib.h"
#include "Subsystems/SmartCANJaguar.h"
#include "Subsystems/SmartSpeedCANJaguar.h"
#include "Commands/Command.h"
#include "Subsystems/Robot1073.h"
#include "Subsystems/Arm.h"
#include "Subsystems/MecanumDrive.h"
#include "Subsystems/Collector.h"
#include "Subsystems/Turret.h"
#include "OI.h"
#include "Subsystems/Shooter.h"
#include "Subsystems/DriverstationMessages.h"
#include "Subsystems/Brakes.h"
#include "Subsystems/DashboardSender.h"
#include "Subsystems/DashboardReceiver.h"
#include "Subsystems/Navigation.h"
#include "Subsystems/SWPIDCANJaguar.h"
#include "Subsystems/TiltMeter.h"
#include "Subsystems/LED.h"
#include "Subsystems/BallSpeedSensor.h"
#include "Subsystems/AutonomousInput.h"
#include "Subsystems/SmartGyro.h"
/**
 * The base for all commands. All atomic commands should subclass CommandBase.
 * CommandBase stores creates and stores each control system. To access a
 * subsystem elsewhere in your code in your code use CommandBase.examplesubsystem
 */ 
class CommandBase: public Command {
public:
	CommandBase(const char *name);
	CommandBase();

	static bool isLibraKraken();

	static void init();
	static void DoSimulate();	// Hack to deal with a few connection points....
	static void PeriodicService();
	
	
	//used for killing stuff... set to true when brakes are on; other people mighth have use for this!!!
	static bool safe;
	//static bool canShoot;
	static bool readyToShoot;
	static int driveMode;
	static int startPosition;
	//
	 
	// Create a single static instance of all of your subsystems
	static Robot1073 *robot;
	static Arm *arm;
	static Collector *collector1;
	static DriverstationMessages *driverstationMessages;
	static DashboardSender *dashboardSender;
	static DashboardReceiver *dashboardReceiver;
	static Navigation *navigation;
	static TiltMeter *tiltMeter;
	static LED *led;  // for turning on the retro illuminator
	
	static AutonomousInput *autoIn;
	
	//chassis / drivetrain / other random stuff
	static MecanumDrive *mecanumDrive;
	static SWPIDCANJaguar *rightFrontJag;
	static SWPIDCANJaguar *rightBackJag;
	static SWPIDCANJaguar *leftFrontJag;
	static SWPIDCANJaguar *leftBackJag;
	static PIDController *rightFrontPID;
	static PIDController *rightBackPID;
	static PIDController *leftFrontPID;
	static PIDController *leftBackPID;
	
	static PIDController *turretPID;

	static SmartCANJaguar *turretJag;
	static SmartSpeedCANJaguar *upperWheelJag;
	static SmartSpeedCANJaguar *lowerWheelJag;
 
	static Servo *rightFrontBrake;
	static Servo *rightBackBrake;
	static Servo *leftFrontBrake;
	static Servo *leftBackBrake;
	static Brakes *brakes;
	static Relay *illuminator;
	static Gyro *gyro;
	
	//arm
	static AnalogChannel *armMagneticEncoder;
	static DigitalInput *armLimitUp;
	static DigitalInput *armLimitDown;
	static Victor *armMotor;
	
	//collector
	static DigitalInput *collectorSensorLower;
	static DigitalInput *collectorSensorMiddle;
	static DigitalInput *collectorSensorUpper;
	static Victor *collectorVictor0;
	static Victor *collectorVictor1;
	static Victor *collectorVictor2;
	static Victor *collectorVictor3;
	 
	//shooter
	static Shooter *shooter;
	static Turret *turret;
	static DigitalInput *sueLimit1;
	static DigitalInput *sueLimit2;
	static BallSpeedSensor *ballSpeedSensor;
	
	static OI *oi;
	
	static void ReverseFrontBack();
	
	static int DashBoardcallback(char * buffer, int length);
private:
	//will be called regardless of libra kraken. those finer details are handled in the methods #encapsultation
	static void ConstructArm();
	static void ConstructTurret();
	static void ConstructShooter();
	static void ConstructCollector();
};

#endif
