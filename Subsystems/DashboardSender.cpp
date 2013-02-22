#include "..\UserDefinitions.h"
#include "DashboardSender.h"
#include "..\CommandBase.h"
#include "..\SmartJoystick.h"
#include "..\LibraKraken.h"
#include "SmartCANJaguar.h"
#include "..\Commands\SendDashboard.h"
#include "DriverStation.h"

#define LIBRA_CODE 0x4C494252
#define MOBILE_CODE 0x4D4F424C
#define PROTOCOL_VERSION 7

#define HEADER_TYPE 15
#define JAG_TYPE 1
#define DIO_TYPE 2
#define ANALOG_TYPE 3
#define VICTOR_TYPE 4
#define JOY_TYPE 5
#define GYRO_TYPE 6
#define SETTINGS_TYPE 7
#define NAVIGATION_TYPE 8
#define END_TYPE 127 //shows end of data

DriverStation *ds = DriverStation::GetInstance();
Dashboard &dash = ds->GetHighPriorityDashboardPacker();
int DashboardSender::packetCount = 0;

static int numberOfCallsToSendData = 0;
static int ourPeriod = 20;

DashboardSender::DashboardSender() : Subsystem("DashboardSender")
{
	
}

void DashboardSender::DisablePeriodic()
{
	SendData();
}

void DashboardSender::SendData()
{
	// Only send telemetry every 'n' calls to the periodic function
	if (numberOfCallsToSendData++ % ourPeriod != 0) return;
	
	dash.AddCluster();

	SendHeader();
	
	SendJag(CommandBase::leftBackJag, "LeftBackJag");
	SendJag(CommandBase::leftFrontJag, "LeftFrontJag");
	SendJag(CommandBase::rightBackJag, "RightBackJag");
	SendJag(CommandBase::rightFrontJag, "RightFrontJag");
	
	if (IsLibraKraken() == true)
	{
		SendJag(CommandBase::turretJag, "TurretJag");
		SendJag(CommandBase::lowerWheelJag, "LowWheelJag");
		SendJag(CommandBase::upperWheelJag, "TopWheelJag");
		
		// Now, send ball sensors
		SendDigitalIO(CommandBase::collectorSensorLower, "CollectorSensorLower");
		SendDigitalIO(CommandBase::collectorSensorMiddle, "CollectorSensorMiddle");
		SendDigitalIO(CommandBase::collectorSensorUpper, "CollectorSensorUpper");
	}
	
	SendSettings();
	SendNav();
	
#if 0
	//sends x,y,z
	SendJoystick(CommandBase::oi->OI::GetLeftJoystick(LEFT_OPERATOR_STICK), 4, "LeftJoy");	//sends first four buttons off left stick
	SendJoystick(CommandBase::oi->OI::GetRightJoystick(RIGHT_OPERATOR_STICK), 4, "RightJoy");
	SendJoystick(CommandBase::oi->OI::GetControlJoystick(), 5, "OpJoy");
#endif	
	
	dash.AddI8(END_TYPE);
	dash.FinalizeCluster();
	dash.Finalize();
}

void DashboardSender::InitDefaultCommand() {
	//Set the default command for a subsystem here.
	SetDefaultCommand(new SendDashboard());
}


void DashboardSender::SendJag(SmartCANJaguar *toSend, char* label)
{
	if (toSend != NULL)
	{
		dash.AddI8(JAG_TYPE);
		MyAddString(label);
		float value;
		CANJaguar::ControlMode mode = toSend->GetControlMode();
		
		if (mode == CANJaguar::kSpeed)
				value = toSend->GetSpeed();
			else 
				value = toSend->GetPosition();
		
		//printf("sdhjsdjfsdsfjdk value:%f\n", value);
		
		dash.AddFloat(0); // dash.AddFloat(toSend->GetOutputCurrent());
		dash.AddFloat(toSend->GetOutputVoltage());
		dash.AddFloat(0); // dash.AddFloat(toSend->GetBusVoltage());
		dash.AddFloat(0); // dash.AddFloat(toSend->GetTemperature());
		dash.AddFloat(value);//speed or position
		dash.AddU16(toSend->GetFaults());
		dash.AddBoolean(false); //dash.AddBoolean(toSend->IsSafetyEnabled());
		dash.AddBoolean(false); ///dash.AddBoolean(toSend->GetForwardLimitOK());
		dash.AddBoolean(false); //dash.AddBoolean(toSend->GetReverseLimitOK());
		dash.AddU16(toSend->GetErrorCount());//error count
	}
	
}
void DashboardSender::SendJoystick(SmartJoystick *toSend, int maxBtn, char* label)
{	
	dash.AddI8(JOY_TYPE);
	MyAddString(label);
	
	dash.AddFloat(toSend->GetX());
	dash.AddFloat(toSend->GetY());
	dash.AddFloat(toSend->GetZ());
	dash.AddFloat(toSend->GetDirectionDegrees());
	dash.AddFloat(toSend->GetMagnitude());
	dash.AddFloat(toSend->GetTwist());
	if (maxBtn >= 1)
	{
		for (int i = 0; i < maxBtn; i++)	dash.AddBoolean(toSend->GetRawButton(i));
	}
}


void DashboardSender::SendDigitalIO(DigitalInput *toSend, char* label)
{
	dash.AddI8(DIO_TYPE);
	MyAddString(label);
	
	if (toSend != NULL)
	{
		dash.AddI32(toSend->Get());
		dash.AddI32(toSend->GetChannel());
	}
	else
	{
		dash.AddI32(1); // For debugging on mobile base
		dash.AddI32(0x1234);
	}
}
void DashboardSender::SendAnalogChannel(AnalogChannel *toSend, char* label)
{
	dash.AddI8(ANALOG_TYPE);
	MyAddString(label);
		
	dash.AddI16(toSend->GetValue());
	dash.AddFloat(toSend->GetVoltage());
}
void DashboardSender::SendI2C(ADXL345_I2C *toSend)
{	
	dash.AddFloat((float) toSend->GetAcceleration(ADXL345_I2C :: kAxis_X));
	dash.AddFloat((float) toSend->GetAcceleration(ADXL345_I2C :: kAxis_Y));
	dash.AddFloat((float) toSend->GetAcceleration(ADXL345_I2C :: kAxis_Z));	
}
void DashboardSender::SendRelay(Relay *toSend)
{
	//meh
}
void DashboardSender::SendVictor(Victor *toSend, char* label)
{
	dash.AddI8(VICTOR_TYPE);
	MyAddString(label);
	
	dash.AddFloat(toSend->Get());
	dash.AddFloat(toSend->GetChannel());
}

void DashboardSender::SendHeader()
{
	dash.AddI8(HEADER_TYPE);
	
	MyAddString("Header");
	
	dash.AddI32(packetCount++);
	
	if(IsLibraKraken() == true)
		dash.AddU32(LIBRA_CODE);
	else
		dash.AddU32(MOBILE_CODE);
	
	dash.AddI8(PROTOCOL_VERSION);
	
}
void DashboardSender::SendGyro(Gyro *toSend, char *label)
{
	float angle = 0.0;
	if (toSend)
		angle = toSend->GetAngle();
	dash.AddI8(GYRO_TYPE);
	MyAddString(label);
	dash.AddFloat(angle);
}
void DashboardSender::MyAddString(char* value)
{
	UINT32 strLength;
	strLength = strlen(value);
	dash.AddU32(strLength);
	
	UINT32 magic = 0x99999999;
	dash.AddU32(magic);
	
	for (size_t i = 0; i < strLength; i++)	dash.AddI8(value[i]);
}
void DashboardSender::SendTiltData()
{
	float x,y,z;
	x = CommandBase::tiltMeter->GetXAccel();
	y = CommandBase::tiltMeter->GetYAccel();
	z = CommandBase::tiltMeter->GetZAccel();
	dash.AddFloat(x); 
	dash.AddFloat(y); 
	dash.AddFloat(z);
}

void DashboardSender::SendSettings()
{
	dash.AddI8(SETTINGS_TYPE);
	
	MyAddString("Settings");
	
	dash.AddBoolean(CommandBase::safe); //brakes are on
	dash.AddBoolean(CommandBase::readyToShoot); //collector is ready / not
	dash.AddBoolean(CommandBase::mecanumDrive->isFieldOrientedPublic());
	dash.AddBoolean(CommandBase::mecanumDrive->isFinesseModePublic());
	dash.AddBoolean(CommandBase::mecanumDrive->isTankDriveModePublic());
	dash.AddBoolean(CommandBase::led->IsOn());
	dash.AddFloat(DriverStation::GetInstance()->GetMatchTime());
	if (IsLibraKraken())
	{
		dash.AddFloat(CommandBase::shooter->GetShootSpeed());
		dash.AddFloat(CommandBase::shooter->GetIncreasedSpeed());
		dash.AddFloat(CommandBase::shooter->GetAngle());
		dash.AddFloat(CommandBase::shooter->GetDistance());
		dash.AddFloat(CommandBase::shooter->GetHeight());
		dash.AddBoolean(CommandBase::turret->IsAuto()); // true is automatic
	}
	else
	{
		dash.AddFloat(0.0);
		dash.AddFloat(0.0);
		dash.AddFloat(0.0);
		dash.AddFloat(0.0);
		dash.AddFloat(0.0);
		dash.AddBoolean(false);
	}
	
}

void DashboardSender::SendNav()
{
	dash.AddI8(NAVIGATION_TYPE);
	
	MyAddString("Navgation");
	
	dash.AddFloat(CommandBase::navigation->GetX());
	dash.AddFloat(CommandBase::navigation->GetY());
	dash.AddFloat(CommandBase::navigation->GetHeading());
	//dash.AddFloat(CommandBase::navigation->GetLatVelocity());
	//dash.AddFloat(CommandBase::navigation->GetLonVelocity());
}

