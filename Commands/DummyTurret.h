#ifndef DUMMY_TURRET_H
#define DUMMY_TURRET_H
#include "../CommandBase.h"
class DummyTurretUp : public CommandBase
{
public:
	DummyTurretUp() {Requires(turret);}
	virtual void Initialize() {}
	virtual void Execute() {turret->TurnRelativeDegrees(40);}
	virtual bool IsFinished() {return true;}
	virtual void End() {}
	virtual void Interrupted() {}
};
class DummyTurretDown : public CommandBase
{
public:
	DummyTurretDown() {Requires(turret);}
	virtual void Initialize() {}
	virtual void Execute() {turret->TurnRelativeDegrees(-40);}
	virtual bool IsFinished() {return true;}
	virtual void End() {}
	virtual void Interrupted() {}
};
#endif
