#ifndef COLLECTOR_REVERSE_H
#define COLLECTOR_REVERSE_H

#include "../CommandBase.h"
#include "../UserDefinitions.h"
#include "../Subsystems/Collector.h"
class CollectorReverse: public CommandBase {
public:
	CollectorReverse();
	CollectorReverse(bool b);
	CollectorReverse(double time_out);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	bool isTimeOut;
	bool off;
};

#endif
