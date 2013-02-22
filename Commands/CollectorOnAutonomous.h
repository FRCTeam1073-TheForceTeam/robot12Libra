#ifndef COLLECTOR_ON_AUTONOMOUS_H
#define COLLECTOR_ON_AUTONOMOUS_H

#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "WPILib.h"
#include "Commands/Subsystem.h"
#include "CollectorOn.h"

class CollectorOnAutonomous : public CollectorOn {
public:
	CollectorOnAutonomous();
	void Execute();
	bool IsFinished();
	void Interrupted();

private:
	bool isAutonomousFinished();

};

#endif
