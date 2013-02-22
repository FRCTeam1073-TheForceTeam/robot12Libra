#ifndef RPC1073_H
#define RPC1073_H
#include "WPIlib.h"
class RPC1073
{
public:
	RPC1073(char *buffer, int length);
	int GetFunction();
	int GetParamCount();
	int GetParamType(int paramIndex);
	char *GetParameterString(int paramIndex, char *destString);
	int GetParameterInt(int paramIndex);
	float GetParameterFloat(int paramIndex); 
	enum PARAMTYPE {
		PARAM_INT = 1,
		PARAM_STRING, 
		PARAM_FLOAT,
		PARAM_DOUBLE,
		PARAM_BOOLEAN
	};
private:
	int functionID;
	int paramCount;
	const static int MAXPARAMLEN = 256;
	const static int MAXPARAMS = 16;
	struct {
		UINT8 type;
		UINT8 length;
		UCHAR value[MAXPARAMLEN];
	}params[MAXPARAMS];
};
#endif
