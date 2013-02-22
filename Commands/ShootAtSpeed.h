
#ifndef SHOOT_AT_SPEED_H
#define SHOOT_AT_SPEED_H
#include "../CommandBase.h"
#include "../Subsystems/Shooter.h"
#define DEFAULT_SPEED 2500

static const float CLOSE_ENOUGH = 0.95f;

class ShootAtSpeed : public CommandBase
{
public:
	ShootAtSpeed(float rpm);
	ShootAtSpeed(float rpm, float timeOutInSeconds);
	void Initialize();
	void Execute();
	bool IsFinished();
	void End();
	void Interrupted();
private:
	float targetSpeed;
};
#endif


