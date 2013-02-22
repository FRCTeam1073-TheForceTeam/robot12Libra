#ifndef COLLECTOR_START_H
#define COLLECTOR_START_H

#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "../Subsystems/Collector.h"
class CollectorStart: public CommandBase {
public:
	CollectorStart();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
