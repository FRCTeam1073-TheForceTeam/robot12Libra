#ifndef COLLECTOR_OFF_H
#define COLLECTOR_OFF_H

#include "../CommandBase.h"

/**
 *
 *
 * @author ExampleAuthor
 */
class CollectorOff : public CommandBase {
public:
	CollectorOff();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
