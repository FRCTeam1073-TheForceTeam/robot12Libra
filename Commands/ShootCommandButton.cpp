#include "ShootCommandButton.h"


ShootCommandButton::ShootCommandButton(bool turnOff)
{
	Requires(shooter); 
	off = turnOff;
}

void ShootCommandButton::Initialize(){
	printf("ShootCommandButton(%4.1f): Initialize %s\n",  DriverStation::GetInstance()->GetMatchTime(),
			(off ? "off": "on"));
	if (off)
		CommandBase::shooter->MotorsOff();
	else
		CommandBase::shooter->MotorsOn();
}

void ShootCommandButton::Execute() 
{
}

bool ShootCommandButton::IsFinished(){ return true;}

void ShootCommandButton::End() {
	printf("ShootCommandButton(%4.1f): End\n",  DriverStation::GetInstance()->GetMatchTime());
}

void ShootCommandButton::Interrupted() {
		printf("ShootCommandButton(%4.1f): Interupted\n",  DriverStation::GetInstance()->GetMatchTime());
}

