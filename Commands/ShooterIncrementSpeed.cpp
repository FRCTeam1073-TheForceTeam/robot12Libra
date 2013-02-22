#include "ShooterIncrementSpeed.h"
#include "ShootCommandButton.h"

static float incrementValue = 0.05;
static float fineIncrement_Value = 0.01;
static bool isFine;

ShooterIncrementSpeed::ShooterIncrementSpeed(bool up) {
	Requires(shooter);
	this->up = up;
}
void ShooterIncrementSpeed::Initialize() {
	float value = (isFine ? fineIncrement_Value : incrementValue);
	
	if (up)
		shooter->IncrementSpeed(value);
	else
		shooter->IncrementSpeed(-1 * value);
}
void ShooterIncrementSpeed::Execute() {}
bool ShooterIncrementSpeed::IsFinished() {
	return true;
}
void ShooterIncrementSpeed::End() {}
void ShooterIncrementSpeed::Interrupted() {}
ShooterFineSpeed::ShooterFineSpeed(bool isFineParam) {
	Requires(shooter);
	settingFineMode = isFineParam;
}

void ShooterFineSpeed::Initialize() {
	isFine = settingFineMode;
}
void ShooterFineSpeed::Execute() {}
bool ShooterFineSpeed::IsFinished() {return true;}
void ShooterFineSpeed::End() {}
void ShooterFineSpeed::Interrupted() {}




