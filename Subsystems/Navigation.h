#ifndef NAVIGATION_H_
#define NAVIGATION_H_

#include "WPILib.h"
#include "SWPIDCANJaguar.h"
#ifdef ENV_VISUALSTUDIO
#include "..\userdefinitions.h"
#endif

//only used in simulator
#ifdef ENV_VISUALSTUDIO
#include "POINTF.h"
#endif

#include "math.h"



class SmartGyro;

class Navigation : public Subsystem
{
	float robotLength;
	float robotWidth;
	float robotHeading; //internal robot angle in radians
	bool hasStarted;

	public:
		Navigation(SWPIDCANJaguar* fl,SWPIDCANJaguar* fr,SWPIDCANJaguar* bl,SWPIDCANJaguar* br, Gyro * g);
		void Start();
		float GetX();
		float GetY();
		float GetXVelocity();
		float GetYVelocity();
		//float GetLatVelocity();
		//float GetLonVelocity();
		float GetXVar();
		float GetYVar();
		float GetHeadingVar();
		float GetHeading();
		void PeriodicService();
		void SetX(float xin);
		void SetY(float yin);
		void SetHeading(float headingin);
		float WrapRadianAngle(float angle);
		
		void UpdateNavData(float x, float y, float heading, float xVar, float yVar, float headingVar);
		bool GetHasStarted();
		void SetStartPosition(int position);
		float GetAngleToTarget();
		float GetDistanceToTarget();
	private:
		static int dashBoardcallback(char * buffer, int length );
		int HandleDashboardCommand( char * buffer, int length);
		Gyro *gyro;

		SWPIDCANJaguar* enc_fl;
		SWPIDCANJaguar* enc_fr;
		SWPIDCANJaguar* enc_bl;
		SWPIDCANJaguar* enc_br;

		Timer *timer;
		
		float x; //feet
		float y; //feet
		float vx;
		float vy;
		float vlat;
		float vlon;
		float dist_fl;
		float dist_fr;
		float dist_bl;
		float dist_br;
		float xVar; //feet^2
		float yVar; //feet^2
		float thVar; //radians^2
		
		float angleToTarget;
		float distanceToTarget;
};

#endif
