#include "tiltmeter.h"

TiltMeter::TiltMeter(void)
{
	accel = new ADXL345_I2C(1, ADXL345_I2C::DataFormat_Range(0x00));
}
	
float TiltMeter::GetTiltDegrees()
{
	ADXL345_I2C::AllAxes allAxes = accel->GetAccelerations();
	float aY = allAxes.YAxis;
	float aX = allAxes.XAxis;
	float aZ = allAxes.ZAxis;
	float accelTotal = sqrt(aX*aX+aY*aY+aZ*aZ);
	float angle = acos(aZ/accelTotal);
	printf("Angle in degrees: %f",angle*180/3.14159);
	return angle;
}
float TiltMeter::GetXAccel() {
	ADXL345_I2C::AllAxes allAxes = accel->GetAccelerations();
	return allAxes.XAxis;
}
float TiltMeter::GetYAccel() {
	ADXL345_I2C::AllAxes allAxes = accel->GetAccelerations();
	return allAxes.YAxis;
}
float TiltMeter::GetZAccel() {
	ADXL345_I2C::AllAxes allAxes= accel->GetAccelerations();
	return allAxes.ZAxis;}
void TiltMeter::GetXYZAccel(float *XAccel, float *YAccel, float *ZAccel)
{
	ADXL345_I2C::AllAxes allAxes = accel->GetAccelerations();
	*XAccel = allAxes.YAxis;
	*YAccel = allAxes.XAxis;
	*ZAccel = allAxes.ZAxis;
}
