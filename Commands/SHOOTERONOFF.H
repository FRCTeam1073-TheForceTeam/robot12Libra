#ifndef SHOOTER_ON_OFF_H
#define SHOOTER_ON_OFF_H
#include "../CommandBase.h"
class ShooterOn : public CommandBase
{
public:
	ShooterOn()
	{
		noTimeOut = true;
		Requires(shooter);
	}
	ShooterOn(double t_out){
		noTimeOut = false;
		SetTimeout(t_out);
		Requires(shooter);
	}
	void Initialize() {shooter->MotorsOn();}
	void Execute() {shooter->MotorsOn();}
	bool IsFinished() {return noTimeOut || IsTimedOut();}
	void End() {puts("Shooter On Command Finished");}
	void Interrupted() {puts("Shooter On Command Interrupted");}
private:
	bool noTimeOut;
};
class ShooterOff : public CommandBase
{
public:
	ShooterOff()
	{
		noTimeOut = true;
		Requires(shooter);
	}
	ShooterOff(double t_out){
		noTimeOut = false;
		SetTimeout(t_out);
		Requires(shooter);
	}
	void Initialize() {shooter->MotorsOff();}
	void Execute () {}
	bool IsFinished() {return noTimeOut || IsTimedOut();}
	void End() {puts("Shooter Off Command Finished");}
	void Interrupted() {puts("Shooter Off Command Interrupted");}
private:
	bool noTimeOut;
};
#endif
