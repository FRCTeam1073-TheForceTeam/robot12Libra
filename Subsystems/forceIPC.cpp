// #include "StdAfx.h"
// #include <WinSock.h>
#include "forceIPC.h"
#include "stdio.h"
#include <netinet/in.h> 

#ifdef ENV_VISUALSTUDIO
#include "winsock2.h"
#endif

forceIPC::forceIPC(void)
{
	headerInitialized = false;
}


forceIPC::~forceIPC(void)
{
}

int forceIPC::loadHeader(char* buffer, unsigned int bufferLength)
{
	if (bufferLength < sizeof(forceIpcHdr_t))
	{
		printf("Packet length %d smaller than header length %d\n", bufferLength, sizeof(forceIpcHdr_t));
		return (-1);
	}

	int offset = 0;
	this->protoVersion = buffer[offset++];
	this->flags = buffer[offset++];
	this->sequenceNo = ntohs(*(short *)&buffer[offset]); offset += 2;
	this->timestamp = ntohl(*(long *)&buffer[offset]); offset += 4;
	this->cmdType = ntohs(*(short *)&buffer[offset]); offset += 2;
	this->pktLen = ntohs(*(short *)&buffer[offset]); offset += 2;

	this->headerInitialized = true;
	return 0;
}

int forceIPC::printHeader()
{
	if (headerInitialized == true)
	{
		printf("Protocol Version: %d, Flags: 0x%02x, Sequence Number: %d\n", protoVersion, flags, sequenceNo);
		printf("Command Type: %d, Timestamp: 0x%08x, Packet Length: %d\n", cmdType, timestamp, pktLen);
		return 0;
	}
	else
		return -1;
}
