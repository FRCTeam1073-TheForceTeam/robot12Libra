#ifndef DASHBOARDRECEIVER_H
#define DASHBOARDRECEIVER_H


#include "../UserDefinitions.h"
#include "Commands/Subsystem.h"
#include "WPILib.h"
#include <netdb.h>

#ifdef ENV_VISUALSTUDIO
#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#pragma comment(lib, "Ws2_32.lib")
#endif //ENV_VISUALSTUDIO

typedef int (*callbackFuncPtr)(char* buffer, int bufferLength);

#ifndef ENV_VISUALSTUDIO
typedef int SOCKET;
#else


#endif

#define MAX_CALLBACKS			16

// Responses from the robot to a reliable dashboard message
#define ACK_FAILED 0x01
#define COMMAND_ERROR 0x02
//#define COMMAND_ANGLETOTARGET 0x07
//#define COMMAND_OVERRIDE_COLLECTOR_SENSOR 0x08
#

class DashboardReceiver : public Subsystem
{
public:
	DashboardReceiver();
	~DashboardReceiver(void);
	void Init();
	int PollSocket(void);
	int SendToDashboard(char *buf, int len);
	int RegisterDashboardCallback(UINT8 rxType, callbackFuncPtr f); // fail if callbackFunc already registered
	int UnregisterDashboardCallback(UINT8 rxType, callbackFuncPtr f); // fail if different callbackFunc registered
private:
	void Receive();
	Task *DashboardReceiverTask;
#ifdef ENV_VISUALSTUDIO
	WSADATA wsaData;
#endif // ENV_VISUALSTUDIO
	int robotSendPort;
	static const int robotListenPort = 1130;	// TBD: change to correct port number
	sockaddr dbSockAddr;
	static const UINT8 protocolVersion = 1;
	SOCKET robotListenSocket;
	SOCKET dbListenSocket;
	callbackFuncPtr callbackFuncList[MAX_CALLBACKS];
	static const int linePrintWidth = 40;
	int AckMessage(char *buffer, int length);
	int InvokeRegisteredCallback(char* buffer, int bufferLength);
	static UINT16 nextSeqNo;

	static int DefaultCallback(char *buf, int len);
	static int DashboardExampleHandler(char *buf, int len);
	static int AutonomousDelayGesture(char *buf, int len);
	static int AvoidTheRamp(char *buf, int len);
	static int ResetSequence(char *buf, int len);
};

#endif
