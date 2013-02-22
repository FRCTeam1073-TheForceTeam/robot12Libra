#include "DashboardReceiver.h"
#include "../CommandBase.h"
#include "Navigation.h"
#include "forceIPC.h"


const float Pi = 3.14159;
#define TO_RADS Pi/180.0
#define GYROVAR 0.05f  //TODO: measure
#define ENCODERVAR (0.05f) // ***** test
#define SCALING .707107

Navigation::Navigation(SWPIDCANJaguar* fl, SWPIDCANJaguar* fr,SWPIDCANJaguar* bl,SWPIDCANJaguar* br, Gyro * g) : Subsystem("Navigation") {

	CommandBase::dashboardReceiver->RegisterDashboardCallback(NAVIGATION_CMD, dashBoardcallback);
	
	enc_fl = fl;
	enc_fr = fr;
	enc_bl = bl;
	enc_br = br;
	gyro = g;
	hasStarted = false;
	
	timer = new Timer();
	timer->Reset();
	timer->Start();
	
	//temporary (will be set properly in StartPositionRobotToColumn.) 
	
	xVar = 25.0f;
	yVar = 4.0f;
	thVar = 0.09f;
	robotHeading = -Pi / 2;
	//robotHeading = 0.0;
	//robotHeading = Pi;
	vx = 0;
	vy = 0;
	x = 0;
	y =0;
	dist_fl = 0.0;
	dist_fr = 0.0;
	dist_bl = 0.0;
	dist_br = 0.0;

}

void Navigation::Start()
{
	hasStarted = true;
}

bool Navigation::GetHasStarted()
{
	return hasStarted;
}

void Navigation::SetX(float xin)
{
	x = xin;
}
void Navigation::SetY(float yin)
{
	y = yin;
}
void Navigation::SetHeading(float thin)
{
	robotHeading = thin;
}

float Navigation::GetX()
{
	return x;
}
float Navigation::GetY()
{
	return y;
}

float Navigation::GetHeading()
{
	return robotHeading;
}

float Navigation::GetXVar()
{
	float xMax = 25.0f;
	if(xVar > xMax)
	{
		return xMax;
	}
	return xVar;
}

float Navigation::GetYVar()
{
	float yMax = 25.0f;
	if(yVar > yMax)
	{
		return yMax;
	}
	return yVar;
}

float Navigation::GetHeadingVar()
{
	return thVar;
}

float Navigation::GetXVelocity()
{




	return vx;
}

float Navigation::GetYVelocity()
{
	return vy;
}

void Navigation::PeriodicService()
{
	static float lastTime = 0;
	static float previoustheta = 1000;
	float theta;
	float currentTime;
	
	currentTime = (float)timer->Get();
	float dt = currentTime - lastTime;
	lastTime = currentTime;
	
	theta = gyro->GetAngle() * TO_RADS * -1;					//degrees, clockwise rotation is positive
	if (previoustheta == 1000) {
		previoustheta = 0;
	}
	float angleDifference = theta - previoustheta;
	previoustheta = theta;
	robotHeading = robotHeading + angleDifference;
	float s = sin(robotHeading);
	float c = cos(robotHeading);
	thVar += GYROVAR*dt;
	
	float currfl = enc_fl->GetFeetPosition();
	float currfr = enc_fr->GetFeetPosition();
	float currbl = enc_bl->GetFeetPosition();
	float currbr = enc_br->GetFeetPosition();

	float dfl = currfl - dist_fl;
	float dfr = currfr - dist_fr;
	float dbl = currbl - dist_bl;
	float dbr = currbr - dist_br;

	dist_fl = currfl;
	dist_fr = currfr;
	dist_bl = currbl;
	dist_br = currbr;

	float d_lat = SCALING * (dfl + dbr - dfr - dbl)/4;   // -left +right robot
	float d_lon = (dfl + dfr + dbl + dbr)/4;             // +forward -backward robot

	float dx = d_lon * c - d_lat * s; // ***** is this right?
	float dy = d_lon * s + d_lat * c;

	xVar += ENCODERVAR * (s + c*SCALING) + thVar;
	yVar += ENCODERVAR * (c + s*SCALING) + thVar;

	vx = dx/dt;
	vy = dy/dt;
	vlat = d_lat/dt;
	vlon = d_lat/dt;
	x += dx;
	y += dy;
}

float Navigation::WrapRadianAngle(float angle) 
{
	int i = (int)(floor(angle * (0.5f/Pi) + 0.5f));
	angle -= i*2.0f*Pi;
	return (angle);
}


void Navigation::UpdateNavData(float _x, float _y, float _th, float _xVar, float _yVar, float _thVar)
{
	x=_x;
	y=_y;
	robotHeading =_th;
	xVar=_xVar;
	yVar=_yVar;
	thVar=_thVar;
}
void Navigation::SetStartPosition(int position)
{
	x = 13; y = 40;

	if(position == 1)//
	{ x = 8; y= 44;robotHeading = -Pi/2;}
	else if(position == 2)
	{ x =11; y= 44;robotHeading = -Pi/2;}
	else if(position == 3)
	{ x = 13.5; y= 44;robotHeading = -Pi/2;}
	else if(position == 4)
	{ x = 16; y= 44;robotHeading = -Pi/2;}
	else if(position == 5 )
	{ x = 19; y= 44;robotHeading = -Pi/2;}
	else if(position == 6)
	{ x = 13.5; y= 36.5;robotHeading = -Pi/2;}
	else if(position == 7)//Restart Simulator Pos
	{x = 13.5; y = 20;}
}

int Navigation::dashBoardcallback(char * buffer, int length )
{
	return CommandBase::navigation->HandleDashboardCommand(buffer, length);
}

int Navigation::HandleDashboardCommand( char * buffer, int length)
{
	// Add code to interpret the command from the dashboard
	return 0;
}

float Navigation::GetAngleToTarget()
{
	return angleToTarget;
}

float Navigation::GetDistanceToTarget()
{
	return distanceToTarget;
}
