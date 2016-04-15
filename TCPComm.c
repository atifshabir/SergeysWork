
#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "TCPComm.h"
#include "global.h"
#include "CY8C9560A.h"

int TCPListenerSockFD;
struct sockaddr_in tcpServerAddr;
int numOfConnectedTCP = 0;

extern char I2CDevName[];
extern char TCP_Listener_IP[];
extern unsigned int TCP_Listener_Port;
extern char LogFileName[];

extern FILE *logFile;

void TCPListenerParams_Init()
{
	int optval = 1;
	int res = 0;

	tcpServerAddr.sin_family = AF_INET;
	tcpServerAddr.sin_port = htons((in_port_t)TCP_Listener_Port);
	tcpServerAddr.sin_addr.s_addr = inet_addr(TCP_Listener_IP);
	bzero(&(tcpServerAddr.sin_zero),sizeof(tcpServerAddr.sin_zero));

	if ((TCPListenerSockFD = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("\n****ERROR: %s: TCP: TCPListenerSockFD could not be opened",__PRETTY_FUNCTION__);
		perror("");
		fflush(stdout);
		exit(1);
	}

	res = setsockopt(TCPListenerSockFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	if(res == 0){
		printf("\nTCPListenerSockFD socket options have been set successfully");
	}else{
		printf("\n****ERROR: %s: TCP: TCPListenerSockFD options could not be set",__PRETTY_FUNCTION__);
		perror("");
		fprintf(logFile, "\n%s;%s; TCP Socket sockopt failed",__DATE__, __TIME__);
		fflush(logFile);
		exit(1);
	}

    if (bind(TCPListenerSockFD,(struct sockaddr *)&(tcpServerAddr),
        sizeof(struct sockaddr)) == -1)
    {
		printf("\n****ERROR: %s: TCP: TCPListenerSockFD could not be bound",__PRETTY_FUNCTION__);
		perror("");
		fprintf(logFile, "\n%s;%s; TCP Socket binding failed",__DATE__, __TIME__);
		fflush(logFile);
		exit(1);
    } else {
		printf("\nTCP: TCPListenerSockFD bound to %s:%u",
			inet_ntoa(tcpServerAddr.sin_addr), ntohs(tcpServerAddr.sin_port));
		fflush(stdout);
	}

}

void TCPComm_Start()
{
	TCPListenerParams_Init();
	TCPComm_ListenForIncommingConns();
}

void TCPComm_ListenForIncommingConns()
{
	socklen_t addrSize = sizeof(struct sockaddr_in);
	int sockFD = 0;
	struct sockaddr_in clientAddr;

	if (listen(TCPListenerSockFD, 1) < 0)
	{
		perror("ERROR on Listening for TCP Connections ");
		fprintf(logFile, "\n%s;%s; TCP Socket listen failed",__DATE__, __TIME__);
		fflush(logFile);
		exit(1);
	}

	while(1)
	{
		sockFD = accept
			(
			TCPListenerSockFD,
			(struct sockaddr *)&clientAddr,
			&addrSize
			);

		if (sockFD < 0)
		{
			perror("ERROR on accepting connection");
			fprintf(logFile, "\n%s;%s; TCP Socket accept failed",__DATE__, __TIME__);
			fflush(logFile);
			sleep(1); //accept again after one second
		}
		else
		{
			printf("\nAccepted TCP connection from %s:%u",
					inet_ntoa(clientAddr.sin_addr),
					ntohs(clientAddr.sin_port));
			fflush(stdout);

			fprintf(logFile, "\n%s;%s; TCP tcp connection accepted with %s:%u",
					__DATE__, __TIME__,
					inet_ntoa(clientAddr.sin_addr),
					ntohs(clientAddr.sin_port));
			fflush(logFile);

			TCPComm_ReadFromConnectedCon(sockFD); //or create a thread for multiple clients
		}
	}
}

void TCPComm_ReadFromConnectedCon(int sockFD)
{
	unsigned char readBuf[SIZE_TCP_READ_BUF];
	int readLen = 0;
	int errorCodeToUser = 0;
	bzero(readBuf, SIZE_TCP_READ_BUF);

	while(1)
	{
		readLen = read(sockFD, readBuf, SIZE_TCP_READ_BUF);
	    if (readLen <= 0)
	    {
	      perror("ERROR reading from socket");
	      fprintf(logFile, "\n%s;%s; TCP Socket read invalid",__DATE__, __TIME__);
	      close(sockFD);
	      fprintf(logFile, "\n%s;%s; TCP tcp connection closed", __DATE__, __TIME__);
	      fflush(logFile);
	      return; //Reconnect for further transfers
	    }
	    else
	    {
	    	printf("Received %d bytes",readLen);

	    	switch(readBuf[IDX_TCP_MSG_TYPE])
	    	{
	    	case msgTypeCommand:
	    		errorCodeToUser = HandleMsg_Command(readBuf);
	    		break;
	    	case msgTypeOpenDev:
	    		errorCodeToUser = HandleMsg_OpenDev(readBuf);
	    		break;
	    	case msgTypeCloseDevice:
	    		errorCodeToUser = HandleMsg_CloseDev(readBuf);
	    		break;
	    	case msgTypeACK:
	    		HandleMsg_ACK(readBuf);
	    		break;
	    	case msgTypeNACK:
	    		HandleMsg_NACK(readBuf);
	    		break;
	    	default:
	    		errorCodeToUser = errCodeInvalidMsgType;
	    		fprintf(logFile, "\n%s;%s; TCP msg type invalid [0x%X]",
	    				__DATE__, __TIME__,readBuf[IDX_TCP_MSG_TYPE]);
	    		fflush(logFile);
	    		printf("\n%s: WARNING: Invalid tcpMsgType received [%u]",
	    				__PRETTY_FUNCTION__, readBuf[IDX_TCP_MSG_TYPE]);
	    		fflush(stdout);
	    	}
	    	readBuf[IDX_TCP_MSG_ERRCODE] = errorCodeToUser;
	    	TCPComm_SendResponseToUser(sockFD, readBuf, readLen);
	    }
	}
}

void TCPComm_SendResponseToUser(int sockFD, unsigned char *sendBuf, int len)
{
	int res = 0;

	res = write(sockFD, sendBuf, len);
	if(res <= 0)
	{
		perror("ERROR writing to socket");
		fprintf(logFile, "\n%s;%s; TCP Socket write failed",__DATE__, __TIME__);
		fflush(logFile);
	}
}
