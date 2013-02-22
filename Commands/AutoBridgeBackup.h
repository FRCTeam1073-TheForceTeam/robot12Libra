#ifndef AUTO_BRIDGE_BACKUP_H
#define AUTO_BRIDGE_BACKUP_H
#include "../CommandBase.h"
#include "TurretTurnToRobotAngle.h"
#include "BackupUntilYouSeeBackboard.h"
#include "AutoAlign.h"
#include "ActuallyDriveBackToBridge.h"
#include "BridgeDown.h"
#include "DriveBackAndRunCollectors.h"
class AutoBridgeBackup : public CommandGroup
{
public:
	AutoBridgeBackup()
	{
		AddSequential(new TurretTurnToRobotAngle());
		AddSequential(new BackupUntilYouSeeBackboard());
		AddSequential(new AutoAlign());
		AddSequential(new ActuallyDriveBackToBridge());
		AddSequential(new BridgeDown());
		AddSequential(new DriveBackAndRunCollectors());
	}
};
#endif
