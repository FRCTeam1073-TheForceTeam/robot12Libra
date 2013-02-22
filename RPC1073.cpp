#include "RPC1073.h"

RPC1073::RPC1073(char *buffer, int length){
	int offset = 0;
	int paramId = 0;
	functionID = ntohl(*((UINT32 *) &buffer[offset]));
	offset += 4;
	paramCount = ntohl(*((UINT32 *) &buffer[offset]));
	offset += 4;
	
	for (paramId = 0; paramId < paramCount; paramId++) {
		params[paramId].type = buffer[offset++];
		params[paramId].length = buffer[offset++];
		memcpy((void *)&params[paramId].value[0], (void *) &buffer[offset], params[paramId].length);
		offset += params[paramId].length;
	}
}

int RPC1073::GetFunction() {
	return functionID;
}

int RPC1073::GetParamCount() {
	return paramCount;
}

int RPC1073::GetParamType(int paramIndex) {
	if (paramIndex > MAXPARAMS) {
		printf("RPC1073 Error: indexing parameter out of bounds %d\n", paramIndex);
		return 0;
	}
	return params[paramIndex].type;
}

int RPC1073::GetParameterInt(int paramIndex) {
	if (paramIndex > MAXPARAMS) {
		printf("RPC1073 Error: indexing parameter out of bounds %d\n", paramIndex);
		return 0;
	}
	if (params[paramIndex].type != PARAM_INT) {
		printf("RPC1073 Error: requested parameter at index %d is not an int\n", paramIndex);
		return 0;
	}
	return ntohl(*((int *)&params[paramIndex].value));
}

char *RPC1073::GetParameterString(int paramIndex, char *destString) {
	if (paramIndex > MAXPARAMS) {
		printf("RPC1073 Error: indexing parameter out of bounds %d\n", paramIndex);
		return 0;
	}
	if (params[paramIndex].type != PARAM_STRING) {
		printf("RPC1073 Error: requested parameter at index %d is not a string\n", paramIndex);
		return 0;
	}

	strncpy(destString, (char *) &params[paramIndex].value, params[paramIndex].length);
	destString[params[paramIndex].length] = '\0';

	return (char *) destString;
}

float RPC1073::GetParameterFloat(int paramIndex) {
	if (paramIndex > MAXPARAMS) {
		printf("RPC1073 Error: indexing parameter out of bounds %d\n", paramIndex);
		return 0;
	}
	if (params[paramIndex].type != PARAM_FLOAT) {
		printf("RPC1073 Error: requested parameter at index %d is not a string\n", paramIndex);
		return 0;
	}
	float tempVar;
	sscanf((char *)&params[paramIndex].value, "%f", &tempVar);
	return tempVar;
}
