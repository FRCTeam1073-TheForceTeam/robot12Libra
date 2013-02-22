#include "DashboardReceiver.h"
#include "forceIPC.h"
#include "usrLib.h"
#include "../CommandBase.h"
#include "../RPC1073.h"
#include "forceIPC.h"


#ifndef ENV_VISUALSTUDIO
const int INVALID_SOCKET = -1;
const int SOCKET_ERROR   = -1;
#endif

UINT16 DashboardReceiver::nextSeqNo = 1;
int DashboardReceiverFunction();

DashboardReceiver::DashboardReceiver() :	Subsystem("DashboardReceiver")
{
	DashboardReceiverTask = new Task("DashboardReceiver", (FUNCPTR)DashboardReceiverFunction );
	DashboardReceiverTask->Start();

}

// At this point, we have created a socket and have associated it with the server port

/* Free dynamically-created entities
*/
DashboardReceiver::~DashboardReceiver(void)
{
#ifdef ENV_VISUALSTUDIO
	closesocket(robotListenSocket);
#else
	close(robotListenSocket);
#endif
}

/* If far end is not yet established, listen for a hello message. Else, receive data and dispatch the appropriate
* callback function. This method blocks on the recvfrom call so it's not wasting CPU doing busy polling.
*/
int DashboardReceiver::PollSocket(void)
{
	int tempResult;
	char buffer[2048];
	int len = 2048;
	
	int senderLen = sizeof(dbSockAddr);

	tempResult = recvfrom(robotListenSocket, buffer, len, 0, &dbSockAddr, &senderLen);

	if (tempResult > 0) {
		printf("Bytes received: %d\n", tempResult);
		buffer[tempResult] = '\0';
	//		printf("Data: %s\n", buffer);
	}
	else
		printf("DashboardReceiver::PollSocket: 0 bytes received\n");
	
	InvokeRegisteredCallback(buffer, tempResult);

	return tempResult;
}

/* InvokeRegisteredCallback: parses the IPC header to extract the command type which is used as an index
* into the callback function table to select an appropriate handler function. The receipt of the incoming 
* packet is acknowledged prior to invoking the callback so it is not necessary
*/
int DashboardReceiver::InvokeRegisteredCallback(char* packet, int packetLength)
{
	int sentDataLength = packetLength - sizeof(forceIpcHdr_t);
	char* sentData = &packet[sizeof(forceIpcHdr_t)];
	forceIPC fHdr;
	fHdr.loadHeader(packet, packetLength);
	callbackFuncPtr f = callbackFuncList[fHdr.cmdType];
	fHdr.printHeader();	// for debug purposes, comment out in real code
	int retcode = -1;
	
	// if received sequence number is expected next in sequence, dispatch and ack.  If out of sequence,
	// simply ack with flag.
	if (fHdr.sequenceNo == nextSeqNo)
	{
		// handle sequence number wrapping
		if (nextSeqNo == 0xffff)
			nextSeqNo = 1;
		else
			nextSeqNo++;
	
//		printf("Invoking registered callback\n");
		retcode = (*f)(sentData, sentDataLength);
	}
	else
	{
		if (fHdr.cmdType == RESET_CMD)
		{
			retcode = (*f)(sentData, sentDataLength);
		}
		else
			printf("DashboardReceiver: received out-of-sequence command\n");
	}
	// Even though the ack is just for having received the message, don't send it before processing the callback.
	// One of the callbacks is what sets up the socket you're going to send the ack to.
	int ackStatus = AckMessage(packet, packetLength);
	if (ackStatus == packetLength)
		return retcode;
	else
	{
		printf("AckStatus: %d", ackStatus);
		return retcode | ACK_FAILED;
	}
}

/* SendToDashboard: use targetSubsystem to select the dashboard socket to which this message will be sent, then
* send it.
*/
int DashboardReceiver::SendToDashboard(char *buf, int len)
{
	int sendToStatus = 0;
	
	sendToStatus = sendto(robotListenSocket, buf, len, 0, &dbSockAddr, sizeof(dbSockAddr));

	return sendToStatus;
}

/* RegisterDashboardCallback: if there is no currently registered callback function for this type,
* register the provided one. If there's already a registered function, return an error and don't
* overwrite the currently registered function.
*/
int DashboardReceiver::RegisterDashboardCallback(UINT8 rxType, callbackFuncPtr f)
{
	if (rxType > MAX_CALLBACKS)
		return -1;
	
	if (callbackFuncList[rxType] != &DefaultCallback){
		return -2;
	}
	callbackFuncList[rxType] = f;
	return 0;
}

/* UnregisterDashboardCallback: if the registered function for this type is the one being unregistered, go 
* ahead and unregister it. If not, someone has called unregister with the wrong parameters so do not
* unregister this function and return an error.
*/
int DashboardReceiver::UnregisterDashboardCallback(UINT8 rxType, callbackFuncPtr f)
{
if (callbackFuncList[rxType] == f) {
	callbackFuncList[rxType] = NULL;
	return 0;
}
	return -1;
}

/* ackMessage: replies to the original message by copying the header to a new buffer, setting the ACK flag
* and sending. If individual command status is required, we will want to send back the entire buffer with the
* caller to ackMessage setting the status flag of each command before ack'ing.
*/
int DashboardReceiver::AckMessage(char *buffer, int length)
{
	forceIpcHdr_t hdr;
	memcpy(&hdr, buffer, sizeof(forceIpcHdr_t));
	printf("AckMessage(): SeqNo %d\n", ntohs(hdr.sequenceNo));
	hdr.flags |= flags_ACK;
	memcpy(buffer, &hdr, sizeof(forceIpcHdr_t));
	return SendToDashboard(buffer, length);
}

/* DefaultCallback: registered as default handler for any incoming message. It simply prints out the contents
* of the message in hexadecimal bytes as big-endian 32-bit words.
*/
int DashboardReceiver::DefaultCallback(char *buf, int len)
{
	int i;
	
	for (i = 0; i < len; i++) {
		printf("%02x", buf[i]);
		if ((i+1)%4 == 0)
			printf(" ");
		if ((i+1)%DashboardReceiver::linePrintWidth == 0)
			printf("\n");
	}
	
	if ((len % DashboardReceiver::linePrintWidth) != 0)
		printf("\n");
	return 0;
}

/* DashboardExampleHandler: registered as demo handler. It prints the buffer contents as a string.
*/
int DashboardReceiver::AutonomousDelayGesture(char *buf, int len)
{
	int i = -1;
	if(DriverStation::GetInstance()->IsAutonomous())
		{
		i = 0;
		RPC1073 msg(buf, len);
		for (int i  = 0; i <msg.GetParamCount(); i++)
		{
			if (msg.GetParamType(i) == RPC1073::PARAM_INT)	CommandBase::autoIn->DelayGesture(msg.GetParameterInt(i));
			else return -1;
		}
	}
	return i;
}
int DashboardReceiver::AvoidTheRamp(char *buf, int len)
{
	int i = -1;
	if(DriverStation::GetInstance()->IsAutonomous())
	{
		CommandBase::autoIn->DontBackup();
		i = 0;
	}
	return i;
}
int DashboardReceiver::DashboardExampleHandler(char *buffer, int length)
{
	RPC1073 msgGuts(buffer, length);
	int i;
	char tempString[256];

	printf("Function is %d\n", msgGuts.GetFunction());
	printf("Argument Count is %d\n", msgGuts.GetParamCount());
	printf("Arguments:\n");
	for (i = 0; i < msgGuts.GetParamCount();i++)
	{
		if (msgGuts.GetParamType(i) == RPC1073::PARAM_INT)
			printf("Param%d, type int, value %d\n", i, msgGuts.GetParameterInt(i));
		else if (msgGuts.GetParamType(i) == RPC1073::PARAM_STRING)
			printf("Param%d, type string, value %s\n", i, msgGuts.GetParameterString(i, tempString));
		else if (msgGuts.GetParamType(i) == RPC1073::PARAM_FLOAT)
			printf("Param%d, type float has value %f\n", i, msgGuts.GetParameterFloat(i));
		else
			printf("Param%d of unknown type\n", i);
	}
	return 0;
}

/* Resync between dashboard and robot
 */
int DashboardReceiver::ResetSequence(char *buf, int len)
{
	DashboardReceiver::nextSeqNo = 1;
	return 0;
}

int DashboardReceiverFunction(void)
{
	CommandBase::dashboardReceiver->Init();
	
	while(true)
	{
//		printf("Read from the network here\n");
		Wait(0.05);										// Process up to 20 messages/second
		CommandBase::dashboardReceiver->PollSocket();	// Hack: using global pointer without ensuring singleton
	}
}

void DashboardReceiver::Init()
{
	int i, tempResult;
		char tempString[255] = "\0";
//	#ifndef ENV_VISUALSTUDIO
#ifdef ENV_VISUALSTUDIO
		// Initialize Winsock
		tempResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (tempResult != 0) {
			printf("WSAStartup failed: %d\n", tempResult);
			return;
		}
#endif //ENV_VISUALSTUDIO
		nextSeqNo = 1;

		// prepare holders for address resolution
		struct addrinfo *result = NULL, hints;

		// set up a hint that we are using UDP in datagram mode and that we want the socket layer to use
		// our IP address
		memset(&hints, 0, sizeof (hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_DGRAM;
		hints.ai_protocol = IPPROTO_UDP;
		hints.ai_flags = AI_PASSIVE;

		// Resolve the local address and port to be used by the server
		sprintf(tempString, "%d", robotListenPort);
		tempResult = getaddrinfo(NULL, tempString, &hints, &result);
		if (tempResult != 0) {
			printf("getaddrinfo failed: %d\n", tempResult);
#ifdef ENV_VISUALSTUDIO
			WSACleanup();
#endif
			return;
		}
		
		// Set up callback functions
		for (i = 0; i < MAX_CALLBACKS; i++)
		{
			callbackFuncList[i] = &DefaultCallback;
		}

		// Create a SOCKET for the server to listen for client connections

		robotListenSocket = INVALID_SOCKET;

		robotListenSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (robotListenSocket == INVALID_SOCKET) {
			printf("Error at socket(): %d\n", errno);
			freeaddrinfo(result);
#ifdef ENV_VISUALSTUDIO
			WSACleanup();
#endif
			return;
		}

		// Setup the UDP listening socket
	    tempResult = bind( robotListenSocket, result->ai_addr, (int)result->ai_addrlen);
	    if (tempResult == SOCKET_ERROR) {
	        printf("bind failed with error: %d\n", errno);
	        freeaddrinfo(result);
#ifdef ENV_VISUALSTUDIO
			closesocket(robotListenSocket);
			WSACleanup();
#else
	        close(robotListenSocket);
#endif
	      return;
	    }

		freeaddrinfo(result);		// release the prior structure before we lose its pointer

		RegisterDashboardCallback(CMDTYPE_CMD, DashboardReceiver::DashboardExampleHandler); // remove when real apps available.
		RegisterDashboardCallback(RESET_CMD, DashboardReceiver::ResetSequence); // remove when real apps available.
		RegisterDashboardCallback(DELAY_AUTONOMOUS, DashboardReceiver::AutonomousDelayGesture);	//call backs are doen in here, not exactly a callback
		RegisterDashboardCallback(IGNORE_BRIDGE_AUTONOMOUS, DashboardReceiver::AvoidTheRamp);	//but it works for the time being,.
		//RegisterDashboardCallback(SET_SHOOTER_CMD, ShooterOneWheel::DashboardShot);
//	#endif
	    printf("DashboardReceiver Initialized\n");
}

