//	This user definations file has been ported by Willie Wider for the use of electrical
//	to test their new stuff.
#ifndef USER_DEFINITIONS_H
#define USER_DEFINITIONS_H
#include "WPIlib.h"
#include "Math.h"
#define LENGTH(ARRAY) (sizeof(ARRAY)/sizeof(ARRAY[0]))
#define VELOCITY(H_DISTANCE, V_DISTANCE, ANGLE) ((H_DISTANCE/ cos(ANGLE) * 4/sqrt(H_DISTANCE * tan(ANGLE) - V_DISTANCE)))
#define PI 3.145926
// jfh Needed a high level location...
#define MAX_RPM 10000.0		// 2655.0 is max load value from data sheet
#define MIN_RPM 0.05
#define ENC_PULSES_PER_REV 360
#define DRIVETRAIN_GEAR_RATIO 14.01

const bool IsLeftFrontEncoderReversed = false;
const bool IsRightFrontEncoderReversed = true;
const bool IsLeftBackEncoderReversed = false;
const bool IsRightBackEncoderReversed = true;

const bool IsLeftFrontMotorReversed = true;
const bool IsRightFrontMotorReversed = false;
const bool IsLeftBackMotorReversed = true;
const bool IsRightBackMotorReversed = false;



#ifdef ENV_VISUALSTUDIO
#pragma region JAGUARS
#endif
	#define CIM_JAG_LEFTBACK 1		// Includes 360 line quad encoder connector
	#define CIM_JAG_RIGHTBACK 2
	#define CIM_JAG_LEFTFRONT 3
	#define CIM_JAG_RIGHTFRONT 4
	#define TURRET_JAG 5			//includes 2 limit switches & quad encoder
	#define UPPER_SHOOTER_JAG 6
	#define LOWER_SHOOTER_JAG 7
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif

#ifdef ENV_VISUALSTUDIO
#pragma region PWMs
#endif
	#define BRAKE_SERVO_FRONT_LEFT 3
	#define BRAKE_SERVO_FRONT_RIGHT 2

	#define BRAKE_SERVO_BACK_LEFT 4
	#define BRAKE_SERVO_BACK_RIGHT 5
	
	#define COLLECTOR_VICTOR_ZERO 6	
	#define COLLECTOR_VICTOR_ONE 7	//FOR CLARITY: The mobile base uses this victor
	#define COLLECTOR_VICTOR_TWO 8
	#define COLLECTOR_VICTOR_THREE 9
	#define ARM_VICTOR 1
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif

#ifdef ENV_VISUALSTUDIO
#pragma region RELAYS
#endif
	#define LED_ILLUMINATOR_SPIKE 1
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif 

#ifdef ENV_VISUALSTUDIO
#pragma region ANALOG_INPUTS
#endif
	#define YAW_GYRO ((UINT32) 1)
	#define MAGNETIC_ARM_ENCODER ((UINT32) 3)
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif

#ifdef ENV_VISUALSTUDIO
#pragma region JOYSTICKS
#endif
	#define DRIVE_STICK 1
	#define OPERATOR_STICK 2
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif

#ifdef ENV_VISUALSTUDIO
#pragma region DIGITAL_IO
#endif
	#define SHOOTER_SPEED_DETECTOR 1
 //	#define I2C_ACCEL 6	//mr kabel said it's probably not wired off of a digital io
	#define COLLECTOR_SENSOR_LOWER 2
	#define COLLECTOR_SENSOR_MIDDLE 3
	#define COLLECTOR_SENSOR_UPPER 4
	#define ARM_LIMIT_UP 10
	#define ARM_LIMIT_DOWN 11
	#define IS_LIBRA_IO 14
#ifdef ENV_VISUALSTUDIO
#pragma endregion
#endif
#endif
