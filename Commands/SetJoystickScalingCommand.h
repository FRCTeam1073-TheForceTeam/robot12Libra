#ifndef SETJOYSTICKSCALINGCOMMAND_H
#define SETJOYSTICKSCALINGCOMMAND_H

#include "../CommandBase.h"

#define JOYSTICK_LINEAR_MODE 0.0f
#define JOYSTICK_SEMICUBIC_MODE 0.5f
#define JOYSTICK_CUBIC_MODE 1.0f

/**
 *
 *
 * @author KC Cowan
 */
class SetJoystickScalingCommand: public CommandBase {
public:
	SetJoystickScalingCommand(float factor);
	virtual void Initialize();
	virtual void Execute();
	virtual bool IsFinished();
	virtual void End();
	virtual void Interrupted();
};

#endif
