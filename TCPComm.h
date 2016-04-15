#ifndef TCPCOMM_H_
#define TCPCOMM_H_

#define SIZE_TCP_READ_BUF	1024 //1k buffer

#define IDX_TCP_MSG_TYPE	0
#define IDX_TCP_MSG_SEQ_NUM	1

#define TCP_RESPONSE_ACK	1
#define TCP_RESPONSE_NACK	0

#define TCP_MAX_NUM_CONNECTOINS	5

void TCPListenerParams_Init();

void TCPComm_Start();
void TCPComm_ListenForIncommingConns();
void TCPComm_ReadFromConnectedCon(int sockFD);
void TCPComm_SendResponseToUser(int sockFD, unsigned char *sendBuf, int len);
#endif // TCPCOMM_H
