#include "WPILib.h"
#include "UserDefinitions.h"
#include "LibraKraken.h"

static DigitalInput *isLibraKraken = NULL;

bool IsLibraKraken()
{
	if (isLibraKraken == NULL)
		isLibraKraken = new DigitalInput(IS_LIBRA_IO);

	return isLibraKraken->Get() == 1;//when jumper is not present get returns 1
}
