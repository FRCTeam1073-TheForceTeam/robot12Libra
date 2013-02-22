#ifndef CollectorMoveShooter_COMMAND_H
#define CollectorMoveShooter_COMMAND_H
#include "CollectorStart.h"
#include "../CommandBase.h"
#include "../Subsystems/Collector.h"
/**
 *
 * @author Cecille
 */

class CollectorMoveShooter: public CommandBase {
public:
	CollectorMoveShooter();
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
private:
	CollectorStart *collectorStart;
};



#endif
