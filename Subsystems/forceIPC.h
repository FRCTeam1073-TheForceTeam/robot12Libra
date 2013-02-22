#ifndef FORCEIPC_H
#define FORCEIPC_H

#include <vxWorks.h>  // for type defs

// forceIPC - Inter Process Communication protocol defined for use by The Force Team 1073

// This file simply defines protocol headers to be used for parsing and forming packets

// flags should be powers of two, i.e. bit indicators
#define flags_ACK 0x01
#define flags_OOS 0x02

typedef struct forceIpcHdr {
	UINT8	protoVersion;
	UINT8	flags;
	UINT16	sequenceNo;
	UINT32	timestamp;
	UINT16	cmdType;
	UINT16	pktLen;
} forceIpcHdr_t;


//
// WARNING!  These values have to match the values in the dashboard
// class constants in forceIPC.cs
enum cmd_t {
	CMDTYPE_CMD = 1, 
	OVERRIDESENSOR_CMD = 2, 
	DISTANCE_ANGLE_HEIGHT_CMD = 3, 
	NAVIGATION_CMD = 4, 
	STARTUP_CMD = 5,
	DELAY_AUTONOMOUS = 6,
	IGNORE_BRIDGE_AUTONOMOUS = 7,
    TURRET_SHOOTER_CMD = 8,
	RESET_GYRO_CMD = 9,
	SET_SHOOTER_CMD = 10,
	RESET_CMD= 15
};

class forceIPC
{
public:
	UINT8	protoVersion;
	UINT8	flags;
	UINT16	sequenceNo;
	UINT32	timestamp;
	UINT16	cmdType;
	UINT16	pktLen;
	bool headerInitialized;
	forceIPC(void);
	~forceIPC(void);
	int loadHeader(char* buffer, unsigned int bufferLength);
	int printHeader();
};

#endif
