
#include <stdio.h>

#include "CY8C9560A.h"
#include "I2CFunctions.h"
#include "global.h"

char addressListInport[8] = {regAddrInport0, regAddrInport1, regAddrInport2, regAddrInport3,
							regAddrInport4, regAddrInport5, regAddrInport6, regAddrInport7};


char addressListOutport[8] = {regAddrOutport0, regAddrOutport1, regAddrOutport2, regAddrOutport3,
							regAddrOutport4, regAddrOutport5, regAddrOutport6, regAddrOutport7};

char addressListIntStatus[8] ={regAddrintStatusPort0,regAddrintStatusPort1,regAddrintStatusPort2,regAddrintStatusPort3,
								regAddrintStatusPort4, regAddrintStatusPort5, regAddrintStatusPort6, regAddrintStatusPort7};

int HandleMsg_Command(char *msg)
{
	int retVal = -1;
	switch(msg[IDX_TCP_MSG_CMD_TPE])
	{
	case cmdTypePortOperation:
		HandleCmd_PortOperation(msg);
		break;
	case cmdTypePWMOperation:
		HandleCmd_PWMOperation(msg);
		break;
	case cmdTypeMiscOperation:
		HandleCmd_MiscOperation(msg);
		break;
	case cmdTypeEEPROMOperation:
		HandleCmd_EEPRomOperation(msg);
		break;
	default:
		printf("\n%s: WARNING: Invalid tcpMsgCmdType received [%u]",
				__PRETTY_FUNCTION__, msg[IDX_TCP_MSG_CMD_TPE]);
		fflush(stdout);
	}
	return retVal;

}

int HandleMsg_OpenDev(char *msg)
{
	return OpenI2C(&(msg[IDX_TCP_MSG_DEV_NAME]));
}

int HandleMsg_CloseDev(char *msg)
{
	int retVal = -1;
	return retVal;
}

int HandleMsg_ACK(char *msg)
{
	int retVal = -1;
	return retVal;
}

int HandleMsg_NACK(char *msg)
{
	int retVal = -1;
	return retVal;
}

int HandleCmd_PortOperation(char *msg)
{
	char portNo = msg[IDX_TCP_MSG_PORT_OP_PORTNO];
	char portOperation = msg[IDX_TCP_MSG_PORT_OP_CODE];
	char portValue = msg[IDX_TCP_MSG_PORT_OP_VALUE];
	char configCode = msg[IDX_TCP_MSG_PORT_CONFIG_CODE];
	char configValue = msg[IDX_TCP_MSG_PORT_CONFIG_VAL];
	char msgOnI2C[SIZE_MAX_OPTION_STR];

	switch(portOperation)
	{
	case portOperationwrite:
		msgOnI2C[0] = addressListOutport[(int)portNo];
		msgOnI2C[1] = portValue;
		WriteI2C(msgOnI2C);
		break;

	case portOperationRead:
		break;

	case portOperationConfig:
		switch(configCode)
		{
		case portConfigIntMask:			msgOnI2C[2] = regAddrIntMask;		break;
		case portConfigPWMOut:			msgOnI2C[2] = regAddrPWMPortOut;	break;
		case portConfigInvertion:		msgOnI2C[2] = regAddrInversion;		break;
		case portConfigPinDirection:	msgOnI2C[2] = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		}
		msgOnI2C[0] = regAddrPortSelect;
		msgOnI2C[1] = portNo;
		WriteI2C(msgOnI2C);
		msgOnI2C[3] = configValue;
		WriteI2C(&(msgOnI2C[2]));
		break;
	}
	return -1;
}

int HandleCmd_PWMOperation(char *msg)
{
	return -1;
}

int HandleCmd_MiscOperation(char *msg)
{
	return -1;
}

int HandleCmd_EEPRomOperation(char *msg)
{
	return -1;
}
