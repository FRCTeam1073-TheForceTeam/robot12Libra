#include "DefensiveCollector.h"
#include "../UserDefinitions.h"
#include "../CommandBase.h"
#include "forceIPC.h"
#define ON_SPEED 0.65
#define OFF_SPEED 0.0
const float DANISH_CONSTANT = 1.0;


DefensiveCollector::DefensiveCollector (DigitalInput *bSensor, Victor *bMotor)
	: Collector(bSensor, NULL, NULL, bMotor, NULL, NULL, NULL)
{
	puts("defensive collector construction started... did i get here ok?");
	
	isCollecting = false;
	shootingInProgress = false;
	isInShooter = false;
	
	bottomSensor = bSensor;
	bottomMotor = bMotor;
	
	motorOnOff (CollectorMotor1, OFF);

	
}

void DefensiveCollector::InitDefaultCommand() {}


int DefensiveCollector::BallCount()
{
	int count = 0;
	if (isStage1())	count++;
	return count;
}
bool DefensiveCollector::isStage1()	{
	Stage1 = bottomSensor->Get();
	return !Stage1;}

void DefensiveCollector::StartManagingBalls()
{
	//printf("Ballcount = %d\n", BallCount());
	if(BallCount() == 0)
		motorOnOff(CollectorMotor1, ON);
}
void DefensiveCollector::ManageBalls(){
	if (isStage1())	motorOnOff(CollectorMotor1, OFF);
}

bool DefensiveCollector::IsDoneManaging()
{
	return (BallCount()==1);
}

void DefensiveCollector::motorOnOff(CollectorMotors motor, MotorState state)
{
	float speed = (state == ON ? ON_SPEED : OFF_SPEED); 
	bottomMotor->Set(speed);	
}

void DefensiveCollector::AllMotorsOff(){
	motorOnOff(CollectorMotor1, OFF);	
}

void DefensiveCollector::AllMotorsOn(){
	puts("You've reached defensive and not collector");
	motorOnOff(CollectorMotor1, ON);
}
float DefensiveCollector::GetMotorSpeed(CollectorMotors motor){
	return bottomMotor->Get();
}
// Used by start shooting command
void DefensiveCollector:: startShooterSequence(){
	motorOnOff(CollectorMotor1, ON);
	haveBall = true;
}
void DefensiveCollector::ChangeGlobalShooterVarible() {CommandBase::readyToShoot = haveBall && isStage1();}
bool DefensiveCollector::isShootingFinished(){
	if (haveBall && !isStage1()){
		tinyTim->Reset();
		tinyTim->Start();
		haveBall = false;
	}
	return (!haveBall && tinyTim -> Get()>= DANISH_CONSTANT);
}
void DefensiveCollector::overrideSensors(CollectorSensors sensor, SensorState state){
	bool sensorState = (state == YES ? true : false); 
	Stage1 = sensorState;		
}

void DefensiveCollector::AllMotorsReverse(){
	float speed = -ON_SPEED;
	bottomMotor->Set(speed);
	printf("DefensiveCollector::AllMotorsReverse() speed %4.1f\n", speed);
}
bool DefensiveCollector::isStage2() {return false;}
bool DefensiveCollector::isStage3() {return false;}
