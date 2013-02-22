#ifndef TURRETCOMMANDS_H
#define TURRETCOMMANDS_H
#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "../Subsystems/turret.h"

class TurretTurnLeftCommand : public CommandBase
{
public:
	
	TurretTurnLeftCommand()  { Requires(turret); }
	virtual void Initialize() { turret->TurnLeft(); }
	virtual void Execute() { ; }
	// Complete is when we hit a limit switch or detect a stall(future)
	virtual bool IsFinished() { return // turret ->IsAnyLimitPressed() ||  KC Jag does limit check itself
		turret->IsStallDetected(); }
	virtual void End() { turret->Stop(); }
	virtual void Interrupted() { turret->Stop(); }

};

class TurretTurnRightCommand : public CommandBase
{
public:
	TurretTurnRightCommand() { Requires(turret); }
	virtual void Initialize() { turret->TurnRight(); }
	virtual void Execute() {  ; }
	// Complete is when we hit a limit switch or detect a stall(future)
	virtual bool IsFinished() { return //turret ->IsAnyLimitPressed() || -- KC Jag does limit check itself
		turret->IsStallDetected(); }
	virtual void End() { turret->Stop(); }
	virtual void Interrupted() { turret->Stop(); }

};

class TurretStopCommand : public CommandBase
{
public:
	TurretStopCommand() {  Requires(turret); }
	virtual void Initialize() { ; }
	virtual void Execute() { ; }
	virtual bool IsFinished() { return true; }
	virtual void End() { turret->Stop(); }
	virtual void Interrupted() { turret->Stop(); }
};

// 
class TurretTurnRelativeAngleCommand : public CommandBase
{
public:
	TurretTurnRelativeAngleCommand()
	{
		Requires(turret);
		
	}
	virtual void Initialize() {
		
	} //
	virtual void Execute() {  turret->DriveMotorToAngle(); }
	// Complete is when we hit a limit switch or detect a stall(future)
	//removed the code that checked limit switches because it meant that we couldn't drive away from a limit.....
	virtual bool IsFinished() { return /*turret ->IsAnyLimitPressed() || */turret->IsStallDetected(); }
	virtual void End() { turret->Stop(); }
	virtual void Interrupted() { turret->Stop(); }
private:
	float degrees;
};

class TurretManualTurn : public CommandBase
{
public:
	bool off;
	TurretManualTurn()
	{
		Requires(turret);
	}
	TurretManualTurn(bool b)
	{
		Requires(turret);
		off = b;
	}
	virtual void Initialize() {SetInterruptible(false);}
	virtual void Execute() {  turret->DriveManualControl(oi->GetOperatorStick()); }
	// Complete is when we hit a limit switch or detect a stall(future)
	virtual bool IsFinished() { return (false || off);}
	virtual void End() { turret->Stop(); }
	virtual void Interrupted() { turret->Stop(); }
};

class TurretSetAngle : public CommandBase
{
public:	
	TurretSetAngle(){Requires(turret);}
	virtual void Initialize() {SetInterruptible(false);}
	virtual void Execute() {turret->DriveManualControl(oi->OI::GetDriveStick());}
	virtual bool IsFinished() {return false;}
	virtual void End() {turret->Stop();}
	virtual void Interrupted() {turret->Stop();}
};

#endif
