#include "Collector.h"
#include "../UserDefinitions.h"
#include "../CommandBase.h"
#include "forceIPC.h"


#define ON_SPEED 0.65
#define OFF_SPEED 0.0
const float DANISH_CONSTANT = 1.0;

/****

Software TBD: Identify the desired user and autonomous commands and change periodicService to support
those commands. Create the commands under Commands subdirectory and map them to Operator Input and this
subsystem.
*/
Collector::Collector (DigitalInput *bSensor, DigitalInput *mSensor, DigitalInput *uSensor, Victor *bbMotor, 
		Victor *bMotor, Victor *mMotor, Victor *uMotor )
: Subsystem("Collector"), bottomSensor(bSensor), middleSensor(mSensor), topSensor(uSensor), bottombottomMotor(bbMotor),
	bottomMotor(bMotor), middleMotor(mMotor), topMotor(uMotor)
{	
	if(topMotor != NULL){	// if we are NOT constructing the defensive collector... sort of a hack. it happens.
		motorOnOff (CollectorMotor0, OFF);
		motorOnOff (CollectorMotor1, OFF);
		motorOnOff (CollectorMotor2, OFF);
		motorOnOff (CollectorMotor3, OFF);
	}
	isCollecting = false;
	shootingInProgress = false;
	isInShooter = false;
	tinyTim = new Timer();	//TinyTim is very sensitive about his name.
	CommandBase::dashboardReceiver->RegisterDashboardCallback(OVERRIDESENSOR_CMD, dashBoardcallback);
}

void Collector::InitDefaultCommand() {}

bool Collector::CollectBall()
{
	isCollecting = true;
	return isCollecting;
}

int Collector::BallCount()
{
	int count = 0;
	if (isStage3())
	{
		count++;
		if (isStage2())
		{
			count++;
			if(isStage1())
				count++;
		}

	}
	return count;


	
}
bool Collector::isStage1()	{
	Stage1 = bottomSensor->Get();
	return !Stage1;}
bool Collector::isStage2()	{
	Stage2 = middleSensor->Get();
	return !Stage2;}
bool Collector::isStage3()	{
	Stage3 = topSensor->Get();
	return !Stage3;}


void
Collector::StartManagingBalls()
{
	//printf("Ballcount = %d\n", BallCount());
	if(BallCount() == 0)
	{
		motorOnOff(CollectorMotor0, ON);
		motorOnOff(CollectorMotor1, ON);
		motorOnOff(CollectorMotor2, ON);
		motorOnOff(CollectorMotor3, ON);
	}
	else if (BallCount() == 1)
	{
		motorOnOff(CollectorMotor0, ON);		
		motorOnOff(CollectorMotor1, ON);
		motorOnOff(CollectorMotor2, ON);
	}
	else if (BallCount() == 2)
	{
		motorOnOff(CollectorMotor0, ON);		
		motorOnOff(CollectorMotor1, ON);		
	}
	else if (BallCount() == 3)
	{
		AllMotorsOff();
	
	}

}
void Collector::ManageBalls()
{
	if (isStage3())
	{
		motorOnOff(CollectorMotor3, OFF);
		if (isStage2())
		{
			motorOnOff(CollectorMotor2, OFF);
			if (isStage1())
			{
				motorOnOff(CollectorMotor1, OFF);
				motorOnOff(CollectorMotor0, OFF);
			}
		}
	}
	
	
}

bool Collector::IsDoneManaging()
{
	//printf("Done managing. Balls = %d \n", BallCount());
	return (BallCount()==3);
	
}


#if 0
bool
Collector::MoveToShooter()	//Perhaps Gams would like a bool returned to say that the ball was succesfully delivered to shooter.
{
	shootingInProgress = true;
	return shootingInProgress;
}

#endif

void Collector::motorOnOff(CollectorMotors motor, MotorState state)
{
	float speed = (state == ON ? ON_SPEED : OFF_SPEED); 
	
//	printf("Collector:motorOnOff setting speed to %f\n", speed);
	switch (motor){
		case CollectorMotor0:	bottombottomMotor->Set(speed);		break;
		case CollectorMotor1:	bottomMotor->Set(speed);	break;
		case CollectorMotor2:	middleMotor->Set(speed);	break;
		case CollectorMotor3:	topMotor->Set(speed);		break;
		default:
			break;
			
	}
}

void Collector::AllMotorsOff()
{
	printf("Collector:AllMotorsOff called\n");
	motorOnOff(CollectorMotor0, OFF);
	motorOnOff(CollectorMotor1, OFF);
	motorOnOff(CollectorMotor2, OFF);
	motorOnOff(CollectorMotor3, OFF);
}

void Collector::AllMotorsOn()
{
	printf("Collector:AllMotorsOn called\n");
	motorOnOff(CollectorMotor0, ON);
	motorOnOff(CollectorMotor1, ON);
	motorOnOff(CollectorMotor2, ON);
	motorOnOff(CollectorMotor3, ON);
}

float Collector::GetMotorSpeed(CollectorMotors motor)
{
	float f = -1;
	switch(motor)
	{
	case CollectorMotor0:
			f = bottombottomMotor->Get();
			break;
	case CollectorMotor1:
		f = bottomMotor->Get();
		break;
	case CollectorMotor2:
		f = middleMotor->Get();
		break;
	case CollectorMotor3:
		f = topMotor->Get();
		break;
	default:
		break;
	}
	return f;
}

// Used by start shooting command
void Collector:: startShooterSequence()
{
	motorOnOff(CollectorMotor3, ON);
	haveBall = true;
}
void Collector::ChangeGlobalShooterVarible() {CommandBase::readyToShoot = haveBall && isStage3();}
bool Collector::isShootingFinished()
{
	
	if (haveBall && !isStage3())
	{
		tinyTim->Reset();
		tinyTim->Start();
		haveBall = false;
	}
	return (!haveBall && tinyTim -> Get()>= DANISH_CONSTANT);
}

void Collector::overrideSensors(CollectorSensors sensor, SensorState state)
{
	bool sensorState = (state == YES ? true : false); 
		
		switch (sensor){
		
			case CollectorSensor1:	Stage1 = sensorState;	break;
			case CollectorSensor2:	Stage2 = sensorState;	break;
			case CollectorSensor3:	Stage3 = sensorState;	break;
			default:   break;
				
		}
}


int Collector::dashBoardcallback(char * buffer, int length )
{
	return CommandBase::collector1->HandleDashboardCommand(buffer, length);
	
}

int Collector::HandleDashboardCommand(char * buffer, int length)
{
	// Add code to interpret the command from the dashboard
	printf("%d %d %d %d", buffer[0], buffer[1], buffer[2], buffer[3]);
	return 0;
}

void Collector::AllMotorsReverse()
{
	float speed = -ON_SPEED;
	bottombottomMotor->Set(speed);
	bottomMotor->Set(speed);
	middleMotor->Set(speed);
	topMotor->Set(speed);
	printf("Collector::AllMotorsReverse() speed %4.1f\n", speed);
}


