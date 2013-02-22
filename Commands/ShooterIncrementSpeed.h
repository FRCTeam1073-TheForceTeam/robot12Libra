#ifndef SHOOTER_INCREMENT_SPEED_H
#define SHOOTER_INCREMENT_SPEED_H
#include "../CommandBase.h"
#include "../Subsystems/Shooter.h"
#include "ShootCommandButton.h"

class ShooterIncrementSpeed : public CommandBase
{
public:
	ShooterIncrementSpeed(bool up);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	bool up;
};

class ShooterFineSpeed : public CommandBase
{
public:
	ShooterFineSpeed(bool isFine);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	bool settingFineMode;
};
#endif

