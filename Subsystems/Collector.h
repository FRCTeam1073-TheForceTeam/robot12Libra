#ifndef COLLECTOR_H
#define COLLECTOR_H
#include "Commands/Subsystem.h"
#include "../UserDefinitions.h"
#include "WPILib.h"

/**
 *
 *
 * @author Cecille
 * 
 * 
 *
 */
class Collector: public Subsystem {
public:
	virtual void ChangeGlobalShooterVarible();
	Collector (DigitalInput *bSensor, DigitalInput *mSensor, DigitalInput *uSensor, Victor *bbMotor,
			Victor *bMotor, Victor *mMotor, Victor *uMotor);
	virtual void InitDefaultCommand();
	// Used by start shooting command...
	
	
	virtual bool isShootingFinished();

	// Used by Start Collecting Balls
	
	virtual bool IsDoneManaging();
	typedef enum  {CollectorMotor0, CollectorMotor1, CollectorMotor2, CollectorMotor3} CollectorMotors;
	typedef enum  {ON, OFF} MotorState;
	typedef enum  {CollectorSensor1, CollectorSensor2, CollectorSensor3} CollectorSensors;
	typedef enum  {YES, NO} SensorState;
	// Turn off all motors
	virtual void AllMotorsOff();
	virtual void AllMotorsOn();
	virtual void startShooterSequence();	
	virtual void StartManagingBalls();
	virtual void ManageBalls();
	//void ManageSensors();
	virtual void overrideSensors(CollectorSensors sensor, SensorState state);
	virtual void motorOnOff (CollectorMotors motor, MotorState state);
	virtual void AllMotorsReverse();
	
	virtual int BallCount();		// Returns the number of balls
	virtual bool isStage1();
	virtual bool isStage2();
	virtual bool isStage3();
	
	bool isCollecting;		//might be useful in dashboard sender
	bool shootingInProgress;
	

	
	
private:
	
	
	bool CollectBall();		//turn on motor until detect ball
	bool MoveToShooter();
	
	
	float GetMotorSpeed(CollectorMotors motor);
	
	
	
	

protected:
	bool Stage1;
	bool Stage2;
	bool Stage3;
	int HandleDashboardCommand( char * buffer, int length);

		bool isInShooter;
	static int dashBoardcallback(char * buffer, int length );
	Timer *tinyTim;
	bool haveBall;
	DigitalInput *bottomSensor;
	Victor *bottomMotor;
	DigitalInput *middleSensor;
	DigitalInput *topSensor;
	DigitalInput *collectingButton;
	Victor *bottombottomMotor;
	Victor *middleMotor;
	Victor *topMotor;
};
#endif
