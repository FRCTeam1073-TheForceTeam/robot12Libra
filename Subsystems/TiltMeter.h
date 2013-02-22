#ifndef _TILTMETER_H_
#define _TILTMETER_H_

#include "math.h"
#include "Commands/Subsystem.h"
#include "WPILib.h"

class TiltMeter
{
private:
	ADXL345_I2C *accel;
public:
	TiltMeter();
	float GetTiltDegrees();
	void GetXYZAccel(float *XAccel, float *YAccel, float *ZAccel);
	//i'd much ratehr not take pointers but just have values be directylr returned
	float GetXAccel();
	float GetYAccel();
	float GetZAccel();
};

#endif
