#include "DriverstationMessages.h"
#include "../JoystickButtonSheet.h"
#include "../CommandBase.h"
#include "../LibraKraken.h"

static int numverOfCallsToDisplayMessage = 0;
static int ourPeriod = 20;

DriverstationMessages::DriverstationMessages() : 
	Subsystem("Driverstation Messages")
{
	index = 0;
}

void DriverstationMessages::PeriodicService()
{
	DisplayMessage();
}

void DriverstationMessages::PrintDefaultDiags()
{
	int errCount = 0;
	errCount += CommandBase::leftFrontJag->GetErrorCount();
	errCount += CommandBase::leftBackJag->GetErrorCount();
	errCount += CommandBase::rightFrontJag->GetErrorCount();
	errCount += CommandBase::rightBackJag->GetErrorCount();
	
	if (CommandBase::turretJag != NULL) 
		errCount += CommandBase::turretJag->GetErrorCount(); 
	if (CommandBase::upperWheelJag != NULL)
		errCount += CommandBase::upperWheelJag->GetErrorCount();
	if (CommandBase::lowerWheelJag != NULL)
		errCount += CommandBase::lowerWheelJag->GetErrorCount();
	Print( 0, "CAN %d, g %2.2f", errCount, CommandBase::gyro->GetAngle() );
	Print( 1, "%2.1f, %2.1f, %2.1f, %2.1f", 
			(float)CommandBase::leftFrontJag->GetPosition(),
			(float)CommandBase::leftBackJag->GetPosition(),
			(float)CommandBase::rightFrontJag->GetPosition(),
			(float)CommandBase::rightBackJag->GetPosition() );
	if (IsLibraKraken())
	{
		bool isTankMode = CommandBase::mecanumDrive->isTankDriveModePublic();
		bool isFeild = CommandBase::mecanumDrive->isFieldOrientedPublic();
		const char *mode = isTankMode ? "tank" : (isFeild ? "field" : "robot");
		Print( 2, "%2.1f %2.0f ",
				(float)CommandBase::turretJag->GetPosition(),
				(float)CommandBase::lowerWheelJag->GetSpeed()			
		// for TwoWheel, add diags here
		 );
		
		// We sometimes see whacky numbers.  If it's more than 100 fps, set it
		// to -1.  Because we only have 21 (or so) characters, the long number
		// means we don't see the other things
		float ballSpeed =  CommandBase::ballSpeedSensor->GetBallSpeedFeetPerSec();
		if (ballSpeed > 100) ballSpeed = -1;
		Print( 3, "%2.2f %d %d %d", ballSpeed,
				CommandBase::collectorSensorLower->Get(),
				CommandBase::collectorSensorMiddle->Get(),
				CommandBase::collectorSensorUpper->Get() );
		Print( 4, "RPM %2.0f + %1.2f\%", CommandBase::shooter->GetShootSpeed(), 
				CommandBase::shooter->GetIncreasedSpeed() );
		Print( 5, "Mode %s", mode);
	}
}
void DriverstationMessages::PrintJag(SmartCANJaguar *jag, char *label)
{
	if (jag == NULL)
	{
		Print(0, "%s Not Connected", label);
		return;
	}
	
	CANJaguar::ControlMode mode = jag->GetControlMode();
	// Print voltage, current, Jag faults and, if there is encoder, position
	Print(0, "%s V: %2.1f", label, jag->GetBusVoltage()); 
	float value;
	if (mode == CANJaguar::kSpeed)
		value = jag->GetSpeed();
	else 
		value = jag->GetPosition();
	Print(1, "F: %08x, %2.1f", jag->GetFaults(), value);
	Print(2, "limF:%d, limB: %d", jag->GetForwardLimitOK(),
			jag->GetReverseLimitOK());
}

void DriverstationMessages::SendTextLines()
{
	DriverStationLCD *lcd =DriverStationLCD::GetInstance();
	for(size_t i = 0; i < LENGTH(text);i++){
			lcd->Printf((DriverStationLCD::Line)i, 1, "%s", (char *)text[i]);
	}
	lcd->UpdateLCD();	
}


void DriverstationMessages::Print(int line, char *data, ...)
{
	  va_list       args;
	  va_start (args, data);
	
	  if ((line < 0) || (line > 6))
		  return;
	  if(line == 0){		// If printing the first line, then assume 1,2, & 3 are blanked
		  for(size_t i = 0; i < LENGTH(text); i++){
			  strcpy((char *)text[i], "                     ");
		  }
	  }
	  
	  vsprintf (text[line], data, args);
	  strcat(text[line], "                     ");
	  va_end (args);
}

void DriverstationMessages::PrintVictor(Victor *vicky, char *label)
{
	if (vicky == NULL)
	{
		Print(0, "%s Not Connected", label);
		return;
	}
	
	Print(0, "%s", label);
	int i = vicky->IsAlive() ? 1 : 0;
	Print(1, "Is Alive:\t%i", i); 
	//Print(2, "Speed:\t%f", vicky->GetSpeed());
}
void DriverstationMessages::PrintJoy(SmartJoystick *stick, char *label)
{
	Print(0, "%s", label);
	Print(1, "X %2.1f, Y %2.1f, Z %2.1f", stick->GetX(), stick->GetY(), stick->GetZ());
	Print(2, "Thr %2.1f, twst %2.1f\n", stick->GetThrottle(), stick->GetTwist());
}

void DriverstationMessages::PrintDigitalIO(DigitalInput *dig1, DigitalInput *dig2, DigitalInput *dig3, char *label)
{
	if (dig1 == NULL || dig2 == NULL || dig3 == NULL)
	{
		Print(0, "%s not connected", label);
		return;
	}
	Print(0, "%s", label);
	Print(1, "V1: %d, V2: %d, V3: %d", dig1->Get(), dig2->Get(), 
			dig3 == NULL ? 0 : dig3->Get());	
}

void DriverstationMessages::PrintLimits(DigitalInput *upLimit, DigitalInput *downLimit, char *label)
{
	if (upLimit == NULL || downLimit == NULL)
	{
		Print(0, "%s not connected", label);
		return;
	}

	Print(0, "%s", label);
	Print(1, "Up: %s, Down: %s", upLimit->Get() == 0? "pressed" : "open", 
		downLimit->Get() == 0 ? "pressed" : "open" );
}

void DriverstationMessages::PrintGyro(Gyro *gyro, char *label)
{
	Print(0, "%s", label);
	if (gyro != NULL)
		Print(1,"theta=%f\n", gyro->GetAngle());
	else
		Print(1, "gyro did not initialize");
}

void DriverstationMessages::PrintMagEncoder(AnalogChannel *encoder, char *label)
{
	if (encoder == NULL)
	{
		Print(0, "%s not connected", label);
		return;
	}

	Print(0, "%s", label);
	Print(1, "%f", encoder->GetValue());
}

void DriverstationMessages::PrintBallSpeedSensor(BallSpeedSensor *sensor, char *label)
{
	if (sensor == NULL)
	{
		Print(0, "%s not connected", label);
		return;
	}
	
	Print(0, "%s", label);
	Print(1, "Count:%d Speed:%f", sensor->GetBallCount(), sensor->GetBallSpeedFeetPerSec());
}

void DriverstationMessages::Toggle()
{
	index++;
}

void DriverstationMessages::PrintFeetPosition()
{
	Print(0, "lf\t%4.2f", CommandBase::leftFrontJag->GetFeetPosition());
	Print(1, "lr\t%4.2f", CommandBase::leftBackJag->GetFeetPosition());
	Print(2, "rf\t%4.2f", CommandBase::rightFrontJag->GetFeetPosition());
	Print(3, "rr\t%4.2f", CommandBase::rightBackJag->GetFeetPosition());
}

void DriverstationMessages::DisplayMessage()
{
	// Instead of updating the screen at 50 hz (or whatever period the periodic service runs,
	/// do it every 20 times (where 20 is configurable).
	if (numverOfCallsToDisplayMessage % ourPeriod != 0) return;
	
	
	//the joystick should be coming in from the command.
	bool pressed = CommandBase::oi->GetDriveStick()->GetRawButton(DRIVER_STATION_TOGGLE);
	static bool wasPressed = false;
	
	switch(index)
	{
	case 0 :
		PrintDefaultDiags();
		break;		
	case 1 : 
		PrintJag(CommandBase::leftBackJag, "Left Back");
		break;
	case 2 :
		PrintJag(CommandBase::leftFrontJag, "Left Front");
		break;
	case 3 :
		PrintJag(CommandBase::rightBackJag, "Right Back");
		break;
	case 4 :
		PrintJag(CommandBase::rightFrontJag, "Right Front");
		break;
	case 5 :
		PrintJag(CommandBase::turretJag, "Turret");
		break;
	case 6 :
		PrintJag(CommandBase::upperWheelJag, "Upper");
		break;
	case 7 :
		PrintJag(CommandBase::lowerWheelJag, "Shooter");
		break;
	case 8 :
		if(IsLibraKraken())
		{ PrintVictor(CommandBase::armMotor, "Arm"); }
		break;
	case 9 :
		if(IsLibraKraken())
		{ PrintVictor(CommandBase::collectorVictor1, "Collector 1"); }
		break;
	case 10 :
		if(IsLibraKraken())
		{ PrintVictor(CommandBase::collectorVictor2, "Collector 2");}
		break; //etc...
	case 11:
		PrintVictor(CommandBase::collectorVictor3, "Collector 3");
		break;
	case 12:
		if(IsLibraKraken())
		{
			PrintDigitalIO(CommandBase::collectorSensorLower,
						   CommandBase::collectorSensorMiddle,
					       CommandBase::collectorSensorUpper, "sensor" );
		}
		break;
	case 13:
		if(IsLibraKraken())
		{ PrintLimits(CommandBase::armLimitUp, CommandBase::armLimitDown, "arm limit"); }
		break;
	case 14:
	{
		static unsigned int i = 0;
		if(0 == (i % 50))
		{
			PrintGyro(CommandBase::gyro, "Gyro");
		}
		i++;
	}
		break;
	case 15: 
		PrintMagEncoder(CommandBase::armMagneticEncoder, "arm encoder");
		break;
	case 16:
		PrintBallSpeedSensor(CommandBase::ballSpeedSensor, "Ball Speed Sensor");
		break;
	case 17:
		PrintJoy(CommandBase::oi->GetDriveStick(), "Drive");
		break;
	case 18:
		PrintJoy(CommandBase::oi->GetOperatorStick(), "Operator");
		break;
	case 19:
		PrintFeetPosition();
		break;
	default :
		PrintFeetPosition();
		index = 0;
	}

	if (pressed && !wasPressed)
		index++;
	wasPressed = pressed;
	
	SendTextLines();
}



