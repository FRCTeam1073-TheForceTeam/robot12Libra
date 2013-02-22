#include "Shooter.h"
#include "../UserDefinitions.h"
#include "../CommandBase.h"
#include "forceIPC.h"
#include "../RPC1073.h"

Shooter::Shooter() : 
	Subsystem("Shooter")
{
	rpmCalculation = new RPMCalc(.1016,.015,.318,1.0);

	shootSpeed = 0;
	increasedSpeed = 0.0;
	motorsOn = false;
	manualOverrideOn = true;
	distance = 0;
	height = 0;
	angle = 0;
	
	CommandBase::dashboardReceiver->RegisterDashboardCallback(DISTANCE_ANGLE_HEIGHT_CMD, callback);
	CommandBase::dashboardReceiver->RegisterDashboardCallback(SET_SHOOTER_CMD, speedCallback);
}

#if 0
#endif

void Shooter::OperatorAim(){}
void Shooter::VisionTrack() {}
void Shooter::SetSpeed(float setSpeed)
{
	shootSpeed = setSpeed;
	//printf("Shooter(%4.1f): SetSpeed(%4.1f)\n", DriverStation::GetInstance()->GetMatchTime(), setSpeed);
}
void Shooter::MotorsOn() {
	motorsOn = true;
	float nominalSpeed = shootSpeed * (1 + increasedSpeed);
	if (fabs(nominalSpeed) > 5000)
	{
		printf("Shooter nominal speed outside the range 0 to 5000\n");
		return;
	}
	
	////printf("Shooter: Speed %f, increment %f, nominal speed %2.1f\n", shootSpeed, increasedSpeed, nominalSpeed );
	SetCANJaguars(nominalSpeed);
//printf("Shooter(%4.1f): MotorOn(%5.2f)\n",  DriverStation::GetInstance()->GetMatchTime(), nominalSpeed);
}

void Shooter::MotorsOff()
{
	printf("Shooter::MotorsOff()\n");
	
	if(!DriverStation::GetInstance()->IsAutonomous())
	{
		motorsOn = false;
		SetCANJaguars(0.0);
	}
}

void Shooter::IncrementSpeed(float increment)
{
	increasedSpeed += increment;
	
	// If the motors are spinning, recalculate how fast they should spin
	if (motorsOn)
		MotorsOn();
}

int Shooter::dashBoardcallback(char * buffer, int length)
 {
	RPC1073 rpcMsg(buffer, length);
	int funcId = rpcMsg.GetFunction();
	int paramCount = rpcMsg.GetParamCount();
	int paramType;
	bool validInput = true;
	float distance = 0;
	float angle = 0;
	float height = 0;

	// Note to Greg:  angle, distance and height can be local variable inside this function
	// And then call SetShooterSpeedFPS if image processing found an image.  SetShooterSpeedFPS will
	// calculate the RPM and set that as the desired speed, if the operators have turned on the motors

	if ((funcId == 31) && (paramCount == 3)) 
	{
		if ((paramType = rpcMsg.GetParamType(0)) == RPC1073::PARAM_FLOAT)
		{
			angle = rpcMsg.GetParameterFloat(0);
			printf("angle %4.1f\n", angle);
		}
		else
		{
			printf("Shooter: param 1 not a float but a %d\n", paramType);	
			validInput = false;
		}
		if ((paramType = rpcMsg.GetParamType(1)) == RPC1073::PARAM_FLOAT)
		{
			distance = rpcMsg.GetParameterFloat(1);
			printf("distance %4.1f\n", distance);
		}
		else
		{
			printf("Shooter: param 2 not a float but a %d\n", paramType);	
			validInput = false;
		}
		if (rpcMsg.GetParamType(2) == RPC1073::PARAM_FLOAT)
		{
			height = rpcMsg.GetParameterFloat(2);
			printf("height %4.1f\n", height);
		}
		else
		{
			printf("Shooter: param 3 not a float but a %d\n", paramType);
			validInput = false;
		}
	}
	else
	{
		printf("Shooter received invalid command. FuncId %d, paramCount %d\n", funcId, paramCount);
		validInput = false;
	}
	
	if (validInput == true)
	{
		printf("ShootCmd: dist %4.1f, height %4.1f, angle %4.1f\n",
				distance, height, angle);
		
		CommandBase::turret->TurnRelativeDegrees(angle);
		SetShooterSpeedFPS(distance, height, angle);
		return 0;
	}
	else
	{
		return -1;
	}
 }
 
int Shooter::callback(char * buffer, int length)
{
	return CommandBase::shooter->dashBoardcallback(buffer, length);
}

// Callback for speed presets
int Shooter::speedCallback(char * buffer, int length)
 {
	RPC1073 rpcMsg(buffer, length);
	int funcId = rpcMsg.GetFunction();
	int paramCount = rpcMsg.GetParamCount();
	int paramType;
	bool validInput = true;
	int rpm = 0;

	if ((funcId == 31) && (paramCount == 1)) 
	{
		if ((paramType = rpcMsg.GetParamType(0)) == RPC1073::PARAM_INT)
		{
			rpm = rpcMsg.GetParameterInt(0);
			printf("shooterCallback %d\n", rpm);
		}
		else
		{
			printf("Shooter speed callback param 1 not a int but a %d\n", paramType);	
			validInput = false;
		}
	}
	else
	{
		printf("Shooter speed callback received invalid command. FuncId %d, paramCount %d\n", funcId, paramCount);
		validInput = false;
	}
	
	if (validInput)
	{		
		CommandBase::shooter->SetSpeed(rpm);
		CommandBase::shooter->MotorsOn();
		return 0;
	}
	else
	{
		return -1;
	}
 }

// This method sets the desired speed.  It is suitable for use by 
// the dashboard at 5-15 frames per scond.  The motors will only spin
// at the desired speed if the operator turns them on.
void Shooter::SetShooterSpeedFPS(double distance, double height, double angle)
{
	if(!manualOverrideOn)
	{
		this->angle=angle;
		this->distance=distance;
		this->height=height;
		
		// GetRPM sometimes returns NaN
		double RPM = rpmCalculation->GetRPM(distance, height, angle);
		if (! (RPM >= 0 && RPM < 5000) )
			return;
			
		SetSpeed(RPM);
	 
		// If the motors are spinning, recalculate how fast they should spin
		if (motorsOn)
			MotorsOn();
	}
	else return;

}

#if 0  // Made this a pure virtual in Shooter.h allowing this to go away...
bool Shooter::SafeToShoot()
{
	const float too_slow  = 200.0f;

	return !(GetCurrentShooterSpeed() <= too_slow);	
}
#endif

float Shooter::GetShootSpeed()
{
	return shootSpeed;
}

float Shooter::GetIncreasedSpeed()
{
	return increasedSpeed;
}
void Shooter::setManualOverride(bool manualOverride)
{
	manualOverrideOn = manualOverride;
}

