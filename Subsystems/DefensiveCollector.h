#ifndef DEFENSIVE_COLLECTOR_H
#include "Collector.h" //to have type access to the enums
#define DEFENSIVE_COLLECTOR_H

class DefensiveCollector : public Collector {
public:
	virtual void ChangeGlobalShooterVarible();
		DefensiveCollector (DigitalInput *bSensor, Victor *bMotor );
		void InitDefaultCommand();
		// Used by start shooting command...
		
		
		bool isShootingFinished();

		// Used by Start Collecting Balls
		
		bool IsDoneManaging();
		
		// Turn off all motors
		void AllMotorsOff();
		void AllMotorsOn();
		void startShooterSequence();	
		void StartManagingBalls();
		void ManageBalls();
		
		void overrideSensors(CollectorSensors sensor, SensorState state);
		void motorOnOff (CollectorMotors motor, MotorState state);
		void AllMotorsReverse();
		float GetMotorSpeed(CollectorMotors motor);
		
		int BallCount();		// Returns the number of balls
		bool isStage1();
		bool isStage2();
		bool isStage3();
		
		bool isCollecting;		//might be useful in dashboard sender
		bool shootingInProgress;
		
private:
	bool Stage1;
		bool Stage2;
		bool Stage3;
		
protected:
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
