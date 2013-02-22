#include "CommandBase.h"
#include "Subsystems/Robot1073.h"
#include "Subsystems/MecanumDrive.h"
#include "Subsystems/Collector.h"
#include "Commands/Scheduler.h"
#include "Subsystems/Shooter.h"
#include "Subsystems/ShooterTwoWheel.h"
#include "Subsystems/DefensiveCollector.h"
#include "Subsystems/ShooterOneWheel.h"
#include "LibraKraken.h"
#include "Subsystems/DriverstationMessages.h"
#include "UserDefinitions.h"
#include "Subsystems/forceIPC.h"

CommandBase::CommandBase(const char *name) : Command(name) {}
CommandBase::CommandBase() : Command() {}

// Initialize a single static instance of all of your subsystems to NULL
Robot1073* CommandBase::robot = NULL;
DashboardReceiver *CommandBase::dashboardReceiver = NULL;
Arm* CommandBase::arm = NULL;
Shooter* CommandBase::shooter = NULL;
MecanumDrive* CommandBase::mecanumDrive = NULL;
DriverstationMessages *CommandBase::driverstationMessages = NULL;
DashboardSender *CommandBase::dashboardSender = NULL;
Navigation *CommandBase::navigation = NULL;
TiltMeter *CommandBase::tiltMeter = NULL;
LED *CommandBase::led = NULL;
SWPIDCANJaguar* CommandBase::rightFrontJag = NULL;
SWPIDCANJaguar* CommandBase::rightBackJag = NULL;
SWPIDCANJaguar* CommandBase::leftFrontJag = NULL;
SWPIDCANJaguar* CommandBase::leftBackJag = NULL;
PIDController* CommandBase::rightFrontPID = NULL;
PIDController* CommandBase::rightBackPID = NULL;
PIDController* CommandBase::leftFrontPID = NULL;
PIDController* CommandBase::leftBackPID = NULL;
PIDController* CommandBase::turretPID = NULL;
SmartCANJaguar* CommandBase::turretJag = NULL;
SmartSpeedCANJaguar* CommandBase::upperWheelJag = NULL;
SmartSpeedCANJaguar* CommandBase::lowerWheelJag = NULL;
DigitalInput *CommandBase::armLimitDown = NULL;
DigitalInput *CommandBase::armLimitUp = NULL;
Victor *CommandBase::armMotor = NULL;
AnalogChannel* CommandBase::armMagneticEncoder = NULL;
Victor* CommandBase::collectorVictor0 = NULL;
Victor* CommandBase::collectorVictor1 = NULL;
Victor* CommandBase::collectorVictor2 = NULL;
Victor* CommandBase::collectorVictor3 = NULL;
DigitalInput* CommandBase::collectorSensorLower = NULL;
DigitalInput* CommandBase::collectorSensorMiddle = NULL;
DigitalInput* CommandBase::collectorSensorUpper = NULL;
Collector* CommandBase::collector1 = NULL; 
OI* CommandBase::oi = NULL;
Turret* CommandBase::turret = NULL;
Servo* CommandBase::leftFrontBrake = NULL;
Servo* CommandBase::leftBackBrake = NULL;
Servo* CommandBase::rightFrontBrake = NULL;
Servo* CommandBase::rightBackBrake = NULL;
Brakes* CommandBase::brakes = NULL;
Relay* CommandBase::illuminator = NULL;
Gyro* CommandBase::gyro = NULL;
BallSpeedSensor* CommandBase::ballSpeedSensor = NULL;
AutonomousInput* CommandBase::autoIn = NULL;
bool CommandBase::safe = true;
bool CommandBase::readyToShoot = false;
int CommandBase::startPosition = 2;//starting robot position in feild...defined positions
int CommandBase::driveMode = 2;//starts as robot oriented mode

bool CommandBase::isLibraKraken() {
	return IsLibraKraken();}

void CommandBase::init() {
	
	//canShoot = false;
	
	// Create a single static instance of all of your subsystems. The following
	// line should be repeated for each subsystem in the project.
	// false false true true for mobile base, true true false false for Libra
	dashboardReceiver = new DashboardReceiver(); // need to be early so other constructors can register themselves
	autoIn = new AutonomousInput();
	rightFrontJag = new SWPIDCANJaguar(CIM_JAG_RIGHTFRONT, ENC_PULSES_PER_REV, DRIVETRAIN_GEAR_RATIO, IsRightFrontEncoderReversed, IsRightFrontMotorReversed);
	rightBackJag = new SWPIDCANJaguar(CIM_JAG_RIGHTBACK, ENC_PULSES_PER_REV, DRIVETRAIN_GEAR_RATIO, IsRightBackEncoderReversed, IsRightBackMotorReversed);
	leftFrontJag = new SWPIDCANJaguar(CIM_JAG_LEFTFRONT, ENC_PULSES_PER_REV, DRIVETRAIN_GEAR_RATIO, IsLeftFrontEncoderReversed, IsLeftFrontMotorReversed);
	leftBackJag = new SWPIDCANJaguar(CIM_JAG_LEFTBACK, ENC_PULSES_PER_REV, DRIVETRAIN_GEAR_RATIO, IsLeftBackEncoderReversed, IsLeftBackMotorReversed);

#if 1   
	// D constant changed from 0.01 to 0.03 by Greg P. Best constants from GSR: 2.0, 0.003, 0.01
	rightFrontPID = new PIDController(2.0f, 0.003f, 0.03f, rightFrontJag, rightFrontJag);
	rightBackPID = new PIDController(2.0f, 0.003f, 0.03f, rightBackJag, rightBackJag);
	leftFrontPID = new PIDController(2.0f, 0.003f, 0.03f, leftFrontJag, leftFrontJag);
	leftBackPID = new PIDController(2.0f, 0.003f, 0.03f, leftBackJag, leftBackJag);
#else  // jfh to play with different constants
	rightFrontPID = new PIDController(0.95f, 0.01f, -0.0f, rightFrontJag, rightFrontJag);
	rightBackPID = new PIDController(0.95f, 0.01f, -0.0f, rightBackJag, rightBackJag);
	leftFrontPID = new PIDController(0.95f, 0.01f, -0.0f, leftFrontJag, leftFrontJag);
	leftBackPID = new PIDController(0.95f, 0.01f, -0.00f, leftBackJag, leftBackJag);

#endif

	// Been having trouble with initializing the gyro.  If it failed, 
	// set the pointer to null
	
	gyro = new SmartGyro(YAW_GYRO, 180.0f);

#ifndef ENV_VISUALSTUDIO  // This Practice of setting pointers to NULL is dangerous UNLESS ALL pointer accesses everywhere implements this check.
	if (gyro->GetError().GetCode() != 0)
		gyro = NULL;
#endif  // Need to turn this off for use in the simulator as this low level error handling not managed...		

	mecanumDrive = new MecanumDrive(rightFrontJag, rightBackJag, leftFrontJag, leftBackJag, gyro);

	navigation = new Navigation(leftFrontJag, rightFrontJag, leftBackJag, rightBackJag, gyro);

	robot = new Robot1073();
	
	
	illuminator = new Relay(LED_ILLUMINATOR_SPIKE);
	led = new LED(illuminator);

	if(IsLibraKraken())
	{
		tiltMeter = new TiltMeter();
		leftFrontBrake = new Servo(BRAKE_SERVO_FRONT_LEFT);
		leftBackBrake = new Servo(BRAKE_SERVO_BACK_LEFT);
		rightFrontBrake = new Servo(BRAKE_SERVO_FRONT_RIGHT);
		rightBackBrake = new Servo(BRAKE_SERVO_BACK_RIGHT);
		brakes = new Brakes(leftFrontBrake, leftBackBrake, rightFrontBrake, rightBackBrake);
	} 
		
	// Send Back to Front
	//ReverseFrontBack();
	
	driverstationMessages = new DriverstationMessages();
	dashboardSender = new DashboardSender();

	ConstructArm();
	ConstructTurret();
	ConstructShooter();
	ConstructCollector();
	
	oi = new OI();
	
	//test for command debugging without subsystem on mobile base
	dashboardReceiver->RegisterDashboardCallback(STARTUP_CMD, DashBoardcallback);
	
	puts("\n\nYou're good to go. Success\n\n");
}

void CommandBase::ConstructArm(){
	puts("arm constructed init");
	armLimitUp = new DigitalInput(ARM_LIMIT_UP);
	armLimitDown = new DigitalInput(ARM_LIMIT_DOWN);
	armMotor = new Victor(ARM_VICTOR);
	armMagneticEncoder = new AnalogChannel(MAGNETIC_ARM_ENCODER);
	arm = new Arm(armLimitUp, armLimitDown, armMotor, armMagneticEncoder);
	puts("arm constructed");
}

void CommandBase::ConstructTurret(){
	puts("turret constructed init");
	if(IsLibraKraken()){
		turretJag = new SmartCANJaguar(TURRET_JAG);
				turret = new Turret(turretJag);
		#ifndef ENV_VISUALSTUDIO
				turretPID = new PIDController(0.05/2, 0.001, 0, turret, turret);
		#else
				turretPID = new PIDController(0.05, 0.0f, 0, turret, turret);
		#endif
				turretPID->SetOutputRange(-135, 135);
	}
	puts("turret constructed");
}
void CommandBase::ConstructShooter(){
	puts("shooter construction in command base init");
	//for depreceated 2 wheel shooter
	//	lowerWheelJag = new SmartSpeedCANJaguar(LOWER_SHOOTER_JAG, 360, 1.0f);
	//	upperWheelJag = new SmartSpeedCANJaguar(UPPER_SHOOTER_JAG, 360, 1.0f);
	//	shooter = new ShooterTwoWheel(upperWheelJag, lowerWheelJag);
	lowerWheelJag = new SmartSpeedCANJaguar(LOWER_SHOOTER_JAG, 360, 1.0f);
	shooter = new ShooterOneWheel(lowerWheelJag);
	if(IsLibraKraken()){
		//check to see if this exists on the mobile base!!!!!!!!!
		ballSpeedSensor = new BallSpeedSensor(SHOOTER_SPEED_DETECTOR);
	}
	puts("shooter stuff constructed in command base");
}

void CommandBase::ConstructCollector(){
	puts("collector hardware contruction init");
	collectorSensorLower = new DigitalInput(COLLECTOR_SENSOR_LOWER);
	if(true /*SIMULATOR HACK*/ || IsLibraKraken()){
		collectorVictor0 = new Victor(COLLECTOR_VICTOR_ZERO);
		collectorVictor1 = new Victor(COLLECTOR_VICTOR_ONE);
		collectorVictor2 = new Victor(COLLECTOR_VICTOR_TWO);
		collectorVictor3 = new Victor(COLLECTOR_VICTOR_THREE);
		collectorSensorMiddle = new DigitalInput(COLLECTOR_SENSOR_MIDDLE);
		collectorSensorUpper = new DigitalInput(COLLECTOR_SENSOR_UPPER);
		collector1 = new Collector(collectorSensorLower, collectorSensorMiddle, collectorSensorUpper,collectorVictor0,
		collectorVictor1, collectorVictor2, collectorVictor3);
		
	}
#if 0
	else {
		collectorVictor1 = new Victor(COLLECTOR_VICTOR_ONE);
		collector1 = new DefensiveCollector(collectorSensorLower, collectorVictor1);
	}
#endif
	puts("collector hardware construction finished");
}

void CommandBase::DoSimulate(){
#ifdef ENV_VISUALSTUDIO
	rightFrontPID->Calculate();
	rightBackPID->Calculate();
	leftFrontPID->Calculate();
	leftBackPID->Calculate();
	turretPID->Calculate();
	
#endif
}

void CommandBase::PeriodicService(){
	navigation->PeriodicService();
}

void  CommandBase::ReverseFrontBack()
{
	rightFrontJag->ReverseMotorEncoder();
	leftFrontJag->ReverseMotorEncoder();

	rightBackJag->ReverseMotorEncoder();
	leftBackJag->ReverseMotorEncoder();
}

int CommandBase::DashBoardcallback(char * buffer, int length)
 {
	RPC1073 rpcMsg(buffer, length);
	int funcId = rpcMsg.GetFunction();
	int paramCount = rpcMsg.GetParamCount();
	int paramType;
	bool validInput = true;
	printf("Entered Startup:dashboardCallback()\n");
	if ((funcId == 31) && (paramCount == 2)) 
	{
		if ((paramType = rpcMsg.GetParamType(0)) == RPC1073::PARAM_INT && (paramType = rpcMsg.GetParamType(1)) == RPC1073::PARAM_INT)
		{
			if (rpcMsg.GetParameterInt(0) <= 6 && rpcMsg.GetParameterInt(0) >= 1)
			{
				startPosition = rpcMsg.GetParameterInt(0);
				printf("Start Pos: %d", startPosition);
			}
			
			if (rpcMsg.GetParameterInt(1) <= 3 && rpcMsg.GetParameterInt(1) >= 1)
			{
				driveMode = rpcMsg.GetParameterInt(1);
				printf("Drive Mode: %d", driveMode);
				
				if (rpcMsg.GetParameterInt(1) == 1)
					mecanumDrive->SetArcadeMode();
				else if (rpcMsg.GetParameterInt(1) == 2)
					mecanumDrive->SetFieldMode(false);
				else if (rpcMsg.GetParameterInt(1) == 3)
					mecanumDrive->SetFieldMode(true);
			}
		}
		else	validInput = false;	
	}
	else
	{
		printf("Start received invalid command. FuncId %d, paramCount %d\n", funcId, paramCount);
		validInput = false;
	}
	
	if (validInput)
	{
		navigation->SetStartPosition(startPosition);
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
