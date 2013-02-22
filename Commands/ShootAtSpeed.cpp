#include "ShootAtSpeed.h"

ShootAtSpeed::ShootAtSpeed(float rpm)
{
	Requires(shooter); 
	targetSpeed = rpm;
	SetTimeout(4.0f);	// max spin up time before we just go ahead and try to shoot

}
ShootAtSpeed::ShootAtSpeed(float rpm, float timeOutInSeconds){
	Requires(shooter);
	targetSpeed = rpm;
	SetTimeout(timeOutInSeconds);
}
void ShootAtSpeed::Initialize(){
	printf("ShootAtSpeed(%4.1f): Initialize %4.1f\n",  DriverStation::GetInstance()->GetMatchTime(),
			targetSpeed);
	CommandBase::shooter->SetSpeed(targetSpeed);
	if (targetSpeed == 0)
		CommandBase::shooter->MotorsOff();
	else
		CommandBase::shooter->MotorsOn();
}

void ShootAtSpeed::Execute() 
{
	if (targetSpeed != 0)
	{
		CommandBase::shooter->SetSpeed(targetSpeed);
		CommandBase::shooter->MotorsOn();
	}		
}

bool ShootAtSpeed::IsFinished(){
	if ((fabs(CommandBase::shooter->GetCurrentShooterSpeed()) >= fabs(targetSpeed * CLOSE_ENOUGH)) || IsTimedOut())
		return true;
	else
		return false;
}

void ShootAtSpeed::End() {
	printf("ShootAtSpeed(%4.1f): End\n",  DriverStation::GetInstance()->GetMatchTime());
}

void ShootAtSpeed::Interrupted() {
		printf("ShootAtSpeed(%4.1f): Interupted\n",  DriverStation::GetInstance()->GetMatchTime());
}

