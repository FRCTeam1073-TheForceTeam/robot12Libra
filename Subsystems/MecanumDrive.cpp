#include "MecanumDrive.h"
#include "Math.h"
#include "../UserDefinitions.h"
#include "../CommandBase.h"

const float twistConst = .8;
static const float OFF = 0.0;
static const float DEADZONE = .02;
static const float TWIST_DEADZONE = .01;
static const int EPIC_FAIL = 100;
static const float MECANUM_CONSTANT = 1.4142;


float MecanumDrive::GetDesiredAngle(float angle)
{
	return (float) angle + Get();
}
//BAD BAD BAD BAD  Can NEVER LOOP!!!!  
bool MecanumDrive::HaveIReachedTheDesiredAngle(float desiredAngle, float ANGLE_TOLERANCE)
{
	return !(fabs(desiredAngle - Get()) > ANGLE_TOLERANCE);
}
MecanumDrive::MecanumDrive(SWPIDCANJaguar *rfmj, SWPIDCANJaguar *rbmj, SWPIDCANJaguar *lfmj,
						   SWPIDCANJaguar *lbmj, Gyro *g) : Subsystem("MecanumDrive") {
#ifdef DEBUG_VERBOSE
	printf("MecanumDrive() called\n");
#endif
	
	gyro = g;
	
	rightFrontMotorJaguar = rfmj;
	rightBackMotorJaguar = rbmj;
	leftFrontMotorJaguar = lfmj;
	leftBackMotorJaguar = lbmj;
	
#ifdef ENV_VISUALSTUDIO
	encoderFail = -1; // Make voltate mode in visual studio
	printf("Mecanum Voltage mode enabled\n");
#else
	encoderFail = 0; // otherwise speed PID
	printf("Mecanum Speed mode enabled\n");
#endif
	

	//servo1 = new Servo(1);
	//servo2 = new Servo(2);
	 
	isFieldOriented = false;
	isFinesseMode = false;
	isTankDriveMode = false;

	rightFrontMotorJaguar->SetPositionReference(SmartCANJaguar::kPosRef_QuadEncoder);
	rightBackMotorJaguar->SetPositionReference(SmartCANJaguar::kPosRef_QuadEncoder);
	leftFrontMotorJaguar->SetPositionReference(SmartCANJaguar::kPosRef_QuadEncoder);
	leftBackMotorJaguar->SetPositionReference(SmartCANJaguar::kPosRef_QuadEncoder);
	
	rightFrontMotorJaguar->ConfigEncoderCodesPerRev(ENC_PULSES_PER_REV);
	rightBackMotorJaguar->ConfigEncoderCodesPerRev(ENC_PULSES_PER_REV);
	leftFrontMotorJaguar->ConfigEncoderCodesPerRev(ENC_PULSES_PER_REV);
	leftBackMotorJaguar->ConfigEncoderCodesPerRev(ENC_PULSES_PER_REV);
	
	CommandBase::rightFrontPID->SetOutputRange(-MAX_RPM, MAX_RPM);
	CommandBase::rightBackPID->SetOutputRange(-MAX_RPM, MAX_RPM);
	CommandBase::leftFrontPID->SetOutputRange(-MAX_RPM, MAX_RPM);
	CommandBase::leftBackPID->SetOutputRange(-MAX_RPM, MAX_RPM);
	
	panrightfl = true;
	panrightfr = false;

	panrightbl = !panrightfl;
	panrightbr = !panrightfr;
	

	CommandBase::rightFrontPID->SetSetpoint(0);
	CommandBase::rightBackPID->SetSetpoint(0);
	CommandBase::leftFrontPID->SetSetpoint(0);
	CommandBase::leftBackPID->SetSetpoint(0);

	if(encoderFail >= 0){
		CommandBase::rightFrontPID->Enable();
		CommandBase::rightBackPID->Enable();
		CommandBase::leftFrontPID->Enable();
		CommandBase::leftBackPID->Enable();
	}

	

#ifdef DEBUG_VERBOSE
	printf("MecanumDrive() finished\n");
#endif
}

void MecanumDrive::Spin(bool positive)
{
	int i = positive ? 1 : -1;
	const float katesValue = 0.5f;
	rightFrontMotorJaguar->Set(i * katesValue);
	rightBackMotorJaguar->Set(i * katesValue);
	leftFrontMotorJaguar->Set(i * katesValue);
	leftBackMotorJaguar->Set(i * katesValue);
}

bool MecanumDrive::isTankDriveModePublic() {return isTankDriveMode;}    
bool MecanumDrive::isFinesseModePublic() {return isFinesseMode;}
bool MecanumDrive::isFieldOrientedPublic()  {return isFieldOriented;}
void MecanumDrive::InitDefaultCommand() {
	//Set the default command for a subsystem here.
	//SetDefaultCommand(new MySpecialCommand());
}

void MecanumDrive::MecanumDriveAction(float joyX, float joyY, float joyTwist, float robAng) {

	
//	joyTwist = 0;
	robAng = PI/180.0;
	//robAng = 0.0;

	// jfh Broke into separte variables for debugging...
	float rfs = rightFrontMotorJaguar->GetLastMotorSpeed();
	float rbs = rightBackMotorJaguar->GetLastMotorSpeed();
	float lfs = leftFrontMotorJaguar->GetLastMotorSpeed();
	float lbs = leftBackMotorJaguar->GetLastMotorSpeed();


	if(((fabs(joyX) > DEADZONE) || (fabs(joyY) > DEADZONE) || (fabs(joyTwist) > TWIST_DEADZONE)) && 
		((fabs(rfs) < MIN_RPM) || (fabs(rbs) < MIN_RPM) || 	 (fabs(lfs) < MIN_RPM) ||  (fabs(lbs) < MIN_RPM))) {
		if(encoderFail >= 0) {
			encoderFail++; 
			printf("Encoder fail count: %d\n", encoderFail);
		}
	}
	else if (encoderFail > 0){					// reset because not consecutive failures
		encoderFail = 0;
	}
	
	if(encoderFail > EPIC_FAIL) {
		encoderFail = -1;
		CommandBase::rightFrontPID->Disable();	// shutdown all PID so it doesn't conflict with voltage setting
		CommandBase::rightBackPID->Disable();
		CommandBase::leftFrontPID->Disable();
		CommandBase::leftBackPID->Disable();
		printf("Fallback to voltage drive - one or more drive wheel encoders has failed\n");
	}

	if (fabs(joyX) < DEADZONE)		joyX = 0.0;
	if (fabs(joyY) < DEADZONE)		joyY = 0.0;
	if (fabs(joyTwist) < TWIST_DEADZONE)	joyTwist = 0.0;
	
	float joyAng = atan2(-joyX, -joyY);
	float actAngle;
//				#yolo
	if(isFieldOriented){
		robAng = (gyro->GetAngle())*(PI/180.0f);
		actAngle = joyAng - robAng;
	}
	else if(isTankDriveMode){
		actAngle=0;
	}
	else{
		actAngle = joyAng;
	}
	
	
	//actAngle = atan2(sin(actAngle),cos(actAngle)/sqrt(2.0f));	//UNTESTED, but we hope this fixes the issue with scaling constants
	float joyMag = sqrt(joyY * joyY + joyX * joyX);
	
	if(isTankDriveMode)	//tank mode should work now
		joyMag = -joyY;
	
	rightFront = joyMag * (cos(actAngle) - sin(actAngle));
	rightBack = joyMag * (cos(actAngle) + sin(actAngle));
	leftFront = joyMag * (cos(actAngle) + sin(actAngle));
	leftBack = joyMag * (cos(actAngle) - sin(actAngle));
	
	rightFront += joyTwist * twistConst;
	rightBack += joyTwist * twistConst; 
	leftFront -= joyTwist * twistConst;
	leftBack -= joyTwist * twistConst;
	
#ifdef DEBUG_VERBOSE
	printf("gyro: %f joystick angle: %f\n", robAng, joyAng);
	printf("lf: %f rf: %f\n", leftFrontMotorJaguar->GetPosition(), rightFrontMotorJaguar->GetPosition());
	printf("lb: %f rb: %f\n", leftBackMotorJaguar->GetPosition(), rightBackMotorJaguar->GetPosition());
#endif	
	Scale();
	SetMotors();

#if 0
	printf("------------------------------------------------\n");
	printf("gyro: %f joystick angle: %f\n", robAng, joyAng);
	printf("lf %4.1f(S%4.1f %4.1fV %4.1fft) rf: %4.1f(S%4.1f %4.1fV %4.1fft)\n",	leftFront, leftFrontMotorJaguar->GetLastMotorSpeed(), leftFrontMotorJaguar->GetLastMotorOutput(), leftFrontMotorJaguar->GetFeetPosition(),    rightFront, rightFrontMotorJaguar->GetLastMotorSpeed(), rightFrontMotorJaguar->GetLastMotorOutput(), rightFrontMotorJaguar->GetFeetPosition());
	printf("lb %4.1f(S%4.1f %4.1fV %4.1fft) rb: %4.1f(S%4.1f %4.1fV %4.1fft)\n",	leftBack,  leftBackMotorJaguar->GetLastMotorSpeed(),  leftBackMotorJaguar->GetLastMotorOutput(),  leftBackMotorJaguar->GetFeetPosition(),  rightBack, rightBackMotorJaguar->GetLastMotorSpeed(),   rightBackMotorJaguar->GetLastMotorOutput(),  rightBackMotorJaguar->GetFeetPosition());
#endif
}

void MecanumDrive::Scale() {
	if(rightFront > 1)
	{
		rightBack /= rightFront;
		leftFront /= rightFront;
		leftBack /= rightFront;
		rightFront = 1;
	}
	if(rightBack > 1)
	{
		rightFront /= rightBack;
		leftFront /= rightBack;
		leftBack /= rightBack;
		rightBack = 1;
	}
	if(leftFront > 1)
	{
		rightBack /= leftFront;
		rightFront /= leftFront;
		leftBack /= leftFront;
		leftFront = 1;
	}
	if(leftBack > 1)
	{
		rightBack /= leftBack;
		rightFront /= leftBack;
		leftFront /= leftBack;
		leftBack = 1;
	}
	if(rightFront < -1)
	{
		rightBack /= -rightFront;
		leftFront /= -rightFront;
		leftBack /= -rightFront;
		rightFront = -1;
	}
	if(rightBack < -1)
	{
		rightFront /= -rightBack;
		leftFront /= -rightBack;
		leftBack /= -rightBack;
		rightBack = -1;
	}
	if(leftFront < -1)
	{
		rightBack /= -leftFront;
		rightFront /= -leftFront;
		leftBack /= -leftFront;
		leftFront = -1;
	}
	if(leftBack < -1)
	{
		rightBack /= -leftBack;
		rightFront /= -leftBack;
		leftFront /= -leftBack;
		leftBack = -1;
	}
	
	if(isFinesseMode)
	{
		rightBack/=3;
		rightFront/=3;
		leftBack/=3;
		leftFront/=3;
	}
	rightFront *= MAX_RPM;
	rightBack *= MAX_RPM;
	leftFront *= MAX_RPM;
	leftBack *= MAX_RPM;

        // Hack to match Libra at GSR
	
	float RF1 = rightFront;
	float RB1 = rightBack;
	float LF1 = leftFront;
	float LB1 = leftBack;
	
	rightFront = LF1;
	rightBack = LB1;
	leftFront = RF1;
	leftBack = RB1;

	// printf("RF: %f RB: %f LF %f LB %f \n",rightFront, rightBack, leftFront, leftBack);
}
//float GetMax()

void MecanumDrive::KillWheelMotors()
{

	rightFrontMotorJaguar->Set(OFF);
	rightBackMotorJaguar->Set(OFF);
	leftFrontMotorJaguar->Set(OFF);
	leftBackMotorJaguar->Set(OFF);
	
}
bool MecanumDrive::MotorsAreOff()
{
	return rightFrontMotorJaguar->Get() == OFF && rightBackMotorJaguar->Get() == OFF && leftFrontMotorJaguar->Get() == OFF && leftBackMotorJaguar->Get() == OFF;
}

void MecanumDrive::SetMotors()
{
	if ((leftFront > MAX_RPM) || (leftFront < (-MAX_RPM)) || 
		(leftBack > MAX_RPM) || (leftBack < (-MAX_RPM))   || 
		(rightFront > MAX_RPM) || (rightFront < (-MAX_RPM)) ||
		(rightBack > MAX_RPM) || (rightBack < (-MAX_RPM)))
		{
			printf("Robot::JoystickDrive was called with %f %f %f %f\n", leftFront, leftBack, rightFront, rightBack);
		}
	
#ifdef DEBUG_VERBOSE
	printf("LF: %f, RF: %f, LB: %f, RB: %f \n", leftFront, rightFront, leftBack, rightBack);
#endif
	
	if(encoderFail != -1) { // Software PID speed mode
		CommandBase::rightFrontPID->SetSetpoint(rightFront);
		CommandBase::rightBackPID->SetSetpoint(rightBack);
		CommandBase::leftFrontPID->SetSetpoint(leftFront);
		CommandBase::leftBackPID->SetSetpoint(leftBack);
		/*if(printNow(10)) {
			printf("RFpos %4.3f, spd %4.3f, setpoint %4.3f\n", rightFrontMotorJaguar->GetWheelPosition(), rightFrontMotorJaguar->GetLastMotorSpeed(),
					-rightFront);
			printf("RBpos %4.3f, spd %4.3f, setpoint %4.3f\n", rightBackMotorJaguar->GetWheelPosition(), rightBackMotorJaguar->GetLastMotorSpeed(),
					-rightBack);
			printf("LFpos %4.3f, spd %4.3f, setpoint %4.3f\n", leftFrontMotorJaguar->GetWheelPosition(), leftFrontMotorJaguar->GetLastMotorSpeed(),
					leftFront);
			printf("LBpos %4.3f, spd %4.3f, setpoint %4.3f\n", leftBackMotorJaguar->GetWheelPosition(), leftBackMotorJaguar->GetLastMotorSpeed(),
					leftBack);
			printf("FPGAtime %f\n", (double) Timer::GetFPGATimestamp());
			printf("\n");
		}*/
	}
	else {	// drop to voltage mode, renormalize to (-1,1)
		// Check for NaN  
		if(rightFront != rightFront)
		{
			printf("SWPIDCANJaguar::PIDWrite rightFront NAN\n");
			rightFront = 1;
		}

		if(leftFront != leftFront)
		{
			printf("SWPIDCANJaguar::PIDWrite leftFront NAN\n");
			leftFront = 1;
		}
		if(rightBack != rightBack)
		{
			printf("SWPIDCANJaguar::PIDWrite rightBack NAN\n");
			rightBack = 1;
		}
		if(leftBack != leftBack)
		{
			printf("SWPIDCANJaguar::PIDWrite leftBack NAN\n");
			leftBack = 1;
		}
		
		// Bounds checking
		if(fabs(rightFront/MAX_RPM) > 1) { 
			printf("SWPIDCANJaguar::PIDWrite rightFront %f out of bounds\n", rightFront);
			rightFront = 1;
		}

		if(fabs(rightBack/MAX_RPM) > 1) { 
			printf("SWPIDCANJaguar::PIDWrite rightBack %f out of bounds\n", rightBack);
			rightBack = 1;
		}
		if(fabs(leftFront/MAX_RPM) > 1) { 
			printf("SWPIDCANJaguar::PIDWrite leftFront %f out of bounds\n", leftFront);
			leftFront = 1;
		}
		if(fabs(leftBack/MAX_RPM) > 1) { 
			printf("SWPIDCANJaguar::PIDWrite leftBack %f out of bounds\n", leftBack);
			leftBack = 1;
		}
		rightFrontMotorJaguar->Set(rightFront/MAX_RPM);
		rightBackMotorJaguar->Set(rightBack/MAX_RPM);
		leftFrontMotorJaguar->Set(leftFront/MAX_RPM);
		leftBackMotorJaguar->Set(leftBack/MAX_RPM);
		/*if(printNow(10)) {
			printf("RFpos %4.3f, spd %4.3f, direct set %4.3f\n", rightFrontMotorJaguar->GetWheelPosition(), rightFrontMotorJaguar->GetLastMotorSpeed(),
					-rightFront/MAX_RPM);
			printf("RBpos %4.3f, spd %4.3f, direct set %4.3f\n", rightBackMotorJaguar->GetWheelPosition(), rightBackMotorJaguar->GetLastMotorSpeed(),
					-rightBack/MAX_RPM);
			printf("LFpos %4.3f, spd %4.3f, direct set %4.3f\n", leftFrontMotorJaguar->GetWheelPosition(), leftFrontMotorJaguar->GetLastMotorSpeed(),
					leftFront/MAX_RPM);
			printf("LBpos %4.3f, spd %4.3f, direct set %4.3f\n", leftBackMotorJaguar->GetWheelPosition(), leftBackMotorJaguar->GetLastMotorSpeed(),
					leftBack/MAX_RPM);
			printf("FPGAtime %f\n", (double) Timer::GetFPGATimestamp());
			printf("\n");
		}*/
	}
}

#if 0	// not used
float Deadzone(float speed, float deadzone){
	if(speed <= deadzone){
		return 0.0;
	} else {
		return speed;
	}
}
#endif

float CubeSpeed(float speed, float magnitude){
	return speed * speed * speed * magnitude;
}

void MecanumDrive::SetFieldMode(bool isFieldOriented)
{
	this->isFieldOriented = isFieldOriented;
	this->isTankDriveMode = false;
	printf("Toggle Drive Mode: %s\n", this->isFieldOriented ? "Field" : "Robot");
}

void MecanumDrive::ToggleFinesseMode()
{
	isFinesseMode = !isFinesseMode;
	printf("Toggle Finesse Mode: %s\n", isFinesseMode ? "Finesse" : "Regular Power");
}

void MecanumDrive::SetArcadeMode()
{
	isTankDriveMode = true;
	isFieldOriented = false;
	printf("Toggle Tank Mode: %s\n", isTankDriveMode ? "Tank" : "Not Tank");
}
bool MecanumDrive::IsEncoderAtDistance(float feet, SWPIDCANJaguar *jag)
{
	const float small_threshold = 0.1f; //1/10 feet
	float actualValue = fabs((float) jag->GetFeetPosition());
	return fabs(feet - actualValue) <= small_threshold;
	
}
bool MecanumDrive::AreEncodersAtDistance(float feet)
{
	bool lr = IsEncoderAtDistance(feet, leftBackMotorJaguar);
	bool rr = IsEncoderAtDistance(feet, rightBackMotorJaguar);
	bool lf = IsEncoderAtDistance(feet, leftFrontMotorJaguar);
	bool rf = IsEncoderAtDistance(feet, rightFrontMotorJaguar);
	return lr && rr && lf && rf;
}

int MecanumDrive::dashBoardcallback(char * buffer, int length)
{
	RPC1073 rpcMsg(buffer, length);
	int funcId = rpcMsg.GetFunction();
	int paramCount = rpcMsg.GetParamCount();
	int paramType;
	bool validInput = true;

	if ((funcId == 31) && (paramCount == 1)) 
	{
		if ((paramType = rpcMsg.GetParamType(0)) == RPC1073::PARAM_INT)
		{
			if (rpcMsg.GetParameterInt(0) == 3)
				CommandBase::gyro->Reset();
		}
		else
		{
			validInput = false;
		}	
	}
	else
	{
		printf("MecanumDrive received invalid command. FuncId %d, paramCount %d\n", funcId, paramCount);
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
 
int MecanumDrive::callback(char * buffer, int length)
{
	return CommandBase::mecanumDrive->dashBoardcallback(buffer, length);
}

