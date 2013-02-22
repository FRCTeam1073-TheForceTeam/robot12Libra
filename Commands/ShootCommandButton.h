
#ifndef SHOOT_COMMAND_BUTTON_H
#define SHOOT_COMMAND_BUTTON_H
#include "../CommandBase.h"
#include "../Subsystems/Shooter.h"
#define DEMO_SPEED 896

class ShootCommandButton : public CommandBase
{
public:

ShootCommandButton(bool b);
void Initialize();
void Execute();
bool IsFinished();
void End();
void Interrupted();
float shootSpeed;

private:
bool off;

};
#endif


