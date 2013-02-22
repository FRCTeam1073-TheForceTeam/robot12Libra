#ifndef SEND_DASHBOARD_H
#define SEND_DASHBOARD_H
#include "../CommandBase.h"
#include "../Subsystems/DashboardSender.h"
class SendDashboard: public CommandBase {
public:
	SendDashboard() {Requires(dashboardSender);}
	virtual void Initialize() { }//dashboardSender->SendConstants();}
	virtual void Execute() {dashboardSender->SendData();}
	virtual bool IsFinished() {return false;}
	virtual void End() {dashboardSender->DisablePeriodic();}
	virtual void Interrupted() {dashboardSender->DisablePeriodic();}
};
#endif
