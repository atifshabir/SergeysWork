#ifndef TCPCOMM_H_
#define TCPCOMM_H_

#define SIZE_TCP_READ_BUF	1024 //1k buffer

#define IDX_TCP_MSG_TYPE	0

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>


typedef struct TCPListenerParams
{
	int sockFD;
	struct sockaddr_in *serverAddr;
} TCPListenerParams;

typedef struct TCPConnectedParams
{
	int sockFD;
	struct sockaddr_in *clientAddr;
} TCPConnectedParams;

void TCPListenerParams_Init(TCPListenerParams *params);
void TCPConnectedParams_Init(TCPConnectedParams *params);

void TCPComm_Start();
void TCPComm_ListenForIncommingConns(TCPListenerParams *listenerParams, TCPConnectedParams *connectedParams);
void TCPComm_ReadFromConnectedCon(TCPConnectedParams *tcpParams);
void TCPComm_SendResponseToUser(TCPConnectedParams *tcpParams, int responseFlag);
#endif // TCPCOMM_H_
