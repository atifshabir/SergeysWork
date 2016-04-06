
#include "TCPComm.h"
#include "global.h"

extern ProgamOptions programOptions;
TCPListenerParams tcpListenerParams;
TCPConnectedParams tcpConnectedParams;

void TCPListenerParams_Init(TCPListenerParams *params)
{
	int optval = 1;
	int res = 0;

	params->serverAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
	params->serverAddr->sin_family = AF_INET;
	params->serverAddr->sin_addr.s_addr = inet_addr(programOptions.localIPAddr);
	params->serverAddr->sin_port = htons((in_port_t)(programOptions.localPort));
	bzero(&(params->serverAddr->sin_zero),sizeof(params->serverAddr->sin_zero));

	params->sockFD = socket(AF_INET, SOCK_STREAM, 0);
	if(params->sockFD < 0)
	{
		perror("ERROR opening TCP socket");
		exit(1);
	}

	res = setsockopt(params->sockFD, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof optval);

	if(res != 0)
	{
		perror("ERROR setting TCP socket options");
		exit(1);
	}

    if(bind(params->sockFD,(struct sockaddr *)&(params->serverAddr), sizeof(struct sockaddr)) == -1)
    {
    	perror("ERROR bingind TCP socket");
    	exit(1);
    }
    else
    {
		printf("\nTCP socket bound to %s:%u",
			inet_ntoa(params->serverAddr->sin_addr), ntohs(params->serverAddr->sin_port));
		fflush(stdout);
	}
}

void TCPConnectedParams_Init(TCPConnectedParams *params)
{
	params->sockFD = 0;
	params->clientAddr = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in));
}


void TCPComm_Start()
{
	TCPListenerParams_Init(&tcpListenerParams);
	TCPConnectedParams_Init(&tcpConnectedParams);
	TCPComm_ListenForIncommingConns(&tcpListenerParams, &tcpConnectedParams);
	TCPComm_ReadFromConnectedCon(&tcpConnectedParams);
}

void TCPComm_ListenForIncommingConns(TCPListenerParams *listenerParams, TCPConnectedParams *connectedParams)
{
	socklen_t addrSize = sizeof(struct sockaddr_in);

	if (listen(listenerParams->sockFD, 1) < 0)
	{
		perror("ERROR on Listening for TCP Connections");
		exit(1);
	}

	while(1)
	{
		connectedParams->sockFD = accept
			(
			listenerParams->sockFD,
			(struct sockaddr *)(connectedParams->clientAddr),
			&addrSize
			);

		if (connectedParams->sockFD < 0)
		{
			perror("ERROR on accepting connection");
			sleep(1); //accept again after one second
		}
		else
		{
			printf("\nAccepted TCP connection from %s:%u",
					inet_ntoa(connectedParams->clientAddr->sin_addr),
					ntohs(connectedParams->clientAddr->sin_port));
			fflush(stdout);
			break;
		}
	}
}

void TCPComm_ReadFromConnectedCon(TCPConnectedParams *tcpParams)
{
	char readBuf[SIZE_TCP_READ_BUF];
	int res = 0;
	bzero(readBuf, SIZE_TCP_READ_BUF);

	while(1)
	{

	    res = read(tcpParams->sockFD, readBuf, SIZE_TCP_READ_BUF);
	    if (res < 0)
	    {
	      perror("ERROR reading from socket");
	    }
	    else
	    {
	    	printf("Received %d bytes",res);
	    	//do all post presseccing eg write, read from i2c here.
	    	TCPComm_SendResponseToUser(tcpParams, 0);
	    }
	}
}

void TCPComm_SendResponseToUser(TCPConnectedParams *tcpParams, int responseFlag)
{
	int res = 0;
	char sendBuf[SIZE_TCP_READ_BUF];
	sendBuf[0] = responseFlag;

	res = write(tcpParams->sockFD, sendBuf, 1);
	if(res)
	{
		perror("ERROR writing to socket");
	}
}
