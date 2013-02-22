#include "turret.h"
#include "Math.h"
#include "Robot1073.h"
#include "../CommandBase.h"
#include "../Commands/TurretCommands.h"
#include "forceIPC.h"

	const float DegreesPerEncoderGet =  33.33f;
	const float MaxErrorDegrees = 1.0;
	const float MaxTurretSpeed= 0.3;
	static const float TURRET_DEADZONE = .01;
	static const float P = 0.05;
	//const int encoderCodes = 1440;
	

Turret::Turret(SmartCANJaguar *motor) : Subsystem("Turret")
{
	canMotor = motor;
	canMotor->SetPositionReference(CANJaguar::kPosRef_QuadEncoder);
	canMotor->ConfigEncoderCodesPerRev(360);	//changed to 1
	targetDegrees = 0;
	isDefaultMode = true;
	turretMode = TurretManual;
	CommandBase::dashboardReceiver->RegisterDashboardCallback(TURRET_SHOOTER_CMD, callback);
	
}
void Turret::InitDefaultCommand()
{
	SetDefaultCommand(new TurretTurnRelativeAngleCommand());
}
void Turret::TurnLeft()
{
	SetMotor(-MaxTurretSpeed);
}

void Turret::TurnRight()
{
	SetMotor(MaxTurretSpeed);
}


void Turret::Stop()
{
	SetMotor(0);
}
void Turret::TurnRelativeRads(float rads)
{
//	float positionNow = GetEncoderDegrees();
//	float targetPosition = positionNow + (rads * DegreesPerEncoderRotation);
	//	float radians = (deg * PI)/180;
	//	TurnRelativeRads(radians);
}

void Turret::TurnRelativeDegrees(float degrees)
{
	printf("DriveMotorToAngle\n");	
	targetDegrees = GetEncoderDegrees() + degrees;
	DriveMotorToAngle();
	CommandBase::turretPID->SetSetpoint(targetDegrees);
}
void Turret :: DriveMotorToAngle()
{
	if (IsAuto())
	{
		CommandBase::turretPID->Enable();
#if 0
		//printf("DriveMotorToAngle\n");
		float error = targetDegrees - GetEncoderDegrees();
		float outputVoltage = error * P;
		//printf("Encoder Degrees:\t%fOutput Voltage\t%f Error\t%fTarget Degrees\t%f\t Enc=%f\n", GetEncoderDegrees(), outputVoltage, error, targetDegrees, GetEncoderDegrees());		
		SetMotor(outputVoltage);
#endif
	}
	else
	{
		CommandBase::turretPID->Disable();
	}
}

bool Turret::IsLeftLimit()
{	
	return !canMotor->GetForwardLimitOK();
}
bool Turret::IsRightLimit()
{
	return !canMotor->GetReverseLimitOK();
}

bool
Turret::IsAnyLimitPressed()
{
	return IsLeftLimit() || IsRightLimit();
}


float Turret::GetEncoderDegrees()
{
	float getValue = canMotor->GetPosition();
	
	float degrees = getValue * DegreesPerEncoderGet;
	return degrees;
	
}

void Turret::SetMotor(float val)	
{
	if(val > MaxTurretSpeed)
			val = MaxTurretSpeed;
	
	if(val < -MaxTurretSpeed)
			val = -MaxTurretSpeed;
	
//	printf("setting turret motor to %4.1f\n", val);
	canMotor->Set(val);
}
bool Turret::IsAtAngle()
{
	if((fabs(GetEncoderDegrees() - targetDegrees)) < MaxErrorDegrees )
	{
		return true;
	}
	return false;
}

void Turret::DriveManualControl(SmartJoystick *joy)
{


#if 1  // jfh to debug manual control.  manual degrees and clipping logic seem not quite ideal...
	CommandBase::turretPID->Disable();	// Need to disable PID control while in Manual control mode

	float joyValue = joy->ScaleJoystick(joy->GetZ(), 0.5);
	float outValue;
//	printf("Turret Z %2.1f\n", manualDegrees);
	if (fabs(joyValue) > TURRET_DEADZONE)
	{
//		printf("DriveManualControl %f\n", manualDegrees);
		outValue = joyValue * MaxTurretSpeed;
	}
	else{
		outValue = 0.0;
	}

	SetMotor(outValue);
#endif
}

int Turret::dashBoardcallback(char * buffer, int length)
{
	RPC1073 rpcMsg(buffer, length);
	int funcId = rpcMsg.GetFunction();
	int paramCount = rpcMsg.GetParamCount();
	int paramType;
	bool validInput = true;

	printf("Entered Turret:dashboardCallback()\n");
	if ((funcId == 31) && (paramCount == 2)) 
	{
		if ((paramType = rpcMsg.GetParamType(0)) == RPC1073::PARAM_INT && (paramType = rpcMsg.GetParamType(1)) == RPC1073::PARAM_INT)
		{
			if (rpcMsg.GetParameterInt(0) == 1)
				turretMode = TurretManual;
			else if (rpcMsg.GetParameterInt(0) == 0)
				turretMode = TurretAuto;

/*	Removed ability to change shooter override		
			if (rpcMsg.GetParameterInt(1) == 1)
				CommandBase::shooter->setManualOverride(true);
			else if(rpcMsg.GetParameterInt(1) == 0)
				CommandBase::shooter->setManualOverride(false);
*/		
			
			// Once the manual override button has been pressed, we will ignore
			// whatever the default was
			isDefaultMode = false;
			
		}
		else
		{
			validInput = false;
		}	
	}
	else
	{
		printf("Turret received invalid command. FuncId %d, paramCount %d\n", funcId, paramCount);
		validInput = false;
	}
	
	if (validInput == true)
	{
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
int Turret::callback(char * buffer, int length)
{
	return CommandBase::turret->dashBoardcallback(buffer, length);
}

bool Turret::IsAuto()
{
	if (isDefaultMode)
		return DefaultMode() == TurretAuto;
	else
		return turretMode == TurretAuto;
}

Turret::TurretMode Turret::DefaultMode()
{
	if (DriverStation::GetInstance()->IsAutonomous())
		return TurretManual;
	else
		return TurretAuto;
}

void 
Turret::PIDWrite(float f)
{
	SetMotor(f);
}

