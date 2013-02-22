#ifndef COLLECTOR_ON_H
#define COLLECTOR_ON_H

#include "../CommandBase.h"

class CollectorOn : public CommandBase {
public:
	CollectorOn();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();

};

#endif
