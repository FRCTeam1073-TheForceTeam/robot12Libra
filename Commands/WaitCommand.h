/*

This command exists so that we can use the wait method in a COMMAND GROUP... aka have sequential commands run with delays.
We don't exactly want commands running in paralell (another command can run paralell with a command group of sequential commands)

There's workarounds, but this works and is relatively simple.

*/

#ifndef WAIT_COMMAND_H
#define WAIT_COMMAND_H

#ifdef ENV_VISUALSTUDIO
#include "stdafx.h"
#endif

#include "../CommandBase.h"

class EvinWaitCommand : public Command	/*created when there was more than one
										 wait command for wpi's scheduler...
										 i don't mean to keep my name in the
										 class title; sorry 'bout that...
										 */

{
public:
	EvinWaitCommand(double seconds) {SetTimeout(seconds);}
	void Initialize() {}
	void Execute() {}
	bool IsFinished() {return IsTimedOut();}
	void End(){}
	void Interrupted() {puts("Wait interrupted");}
private:
};
#endif






