
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
	switch((unsigned char)(msg[IDX_TCP_MSG_CMD_TPE]))
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
	case cmdTypePinOperation:
		HandleCmd_PinOperation(msg);
		break;
	case cmdTypePOROperation:
		HandleCmd_POROperation(msg);
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
	char regValue[SIZE_MAX_OPTION_STR];
	char regAddr[SIZE_MAX_OPTION_STR];

	regValue[0] = msg[IDX_TCP_MSG_PORT_OP_VALUE];
	regValue[1] = 0;
	regAddr[0] = addressListOutport[(int)portNo];
	regAddr[1] = 0;

	switch((unsigned char)(msg[IDX_TCP_MSG_PORT_OP_CODE]))
	{
	case portOperationwrite:
		TxToDevice(regValue, regAddr);
		break;

	case portOperationRead:
		RxToDevice(regValue, regAddr, 1);
		break;

	case portOperationConfig:
		regValue[0] = portNo;
		regAddr[0] = regAddrPortSelect;
		regAddr[1] = 0;
		TxToDevice(regValue, regAddr);
		switch((unsigned char)(msg[IDX_TCP_MSG_PORT_CONFIG_CODE]))
		{
		case portConfigIntMask:			regAddr[0] = regAddrIntMask;		break;
		case portConfigPWMOut:			regAddr[0] = regAddrPWMPortOut;	break;
		case portConfigInvertion:		regAddr[0] = regAddrInversion;		break;
		case portConfigPinDirection:	regAddr[0] = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		}
		regValue[0] = msg[IDX_TCP_MSG_PORT_CONFIG_VAL];
		TxToDevice(regValue, regAddr);
		break;
	}
	return -1;
}

int HandleCmd_PinOperation(char *msg)
{
	char portNo = msg[IDX_TCP_MSG_PIN_OP_PORTNO];
	char pinMask = msg[IDX_TCP_MSG_PIN_OP_PINMASK];
	char regValue[SIZE_MAX_OPTION_STR];
	char regAddr[SIZE_MAX_OPTION_STR];
	char newVal = 0;
	char oldVal = 0;
	char portRegAddr = addressListOutport[(int)portNo];

	switch((unsigned char)(msg[IDX_TCP_MSG_PORT_OP_CODE]))
	{
	case portOperationwrite:
		regAddr[0] = portRegAddr;
		regAddr[1] = 0;
		//RxToDevice(regValue, regAddr, 1);
		oldVal = regValue[0];
		oldVal &= ~pinMask; //clear the bits of old val accoriding to pinMask

		newVal = msg[IDX_TCP_MSG_PIN_OP_VALUE] == PIN_HIGH? 0xFF: 0x00;
		newVal &= pinMask; //all bits except the mask are cleared. Masked pins made accornding to value

		newVal |= oldVal; //Or teh newVal wih Oldval and store to newVal
		regValue[0] = newVal;
		TxToDevice(regValue, regAddr);
		break;

	case portOperationRead:
		break;

	case portOperationConfig:
		regValue[0] = portNo;
		regAddr[0] = regAddrPortSelect;
		regAddr[1] = 0;
		TxToDevice(regValue, regAddr);

		switch((unsigned char)(msg[IDX_TCP_MSG_PIN_CONFIG_CODE]))
		{
		case portConfigIntMask:			regAddr[0] = regAddrIntMask;		break;
		case portConfigPWMOut:			regAddr[0] = regAddrPWMPortOut;	break;
		case portConfigInvertion:		regAddr[0] = regAddrInversion;		break;
		case portConfigPinDirection:	regAddr[0] = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		}

		RxToDevice(regValue, regAddr, 1);
		oldVal = regValue[0];
		oldVal &= ~pinMask; //clear the bits of old val accoriding to pinMask

		newVal = msg[IDX_TCP_MSG_PIN_CONFIG_VAL] == PIN_HIGH? 0xFF: 0x00;
		newVal &= pinMask; //all bits except the mask are cleared. Masked pins made accornding to value

		newVal |= oldVal; //Or teh newVal wih Oldval and store to newVal
		regValue[0] = newVal;
		TxToDevice(regValue, regAddr);
		break;
	}
	return -1;
}

int HandleCmd_IntStausOperation(char *msg)
{
	char portNo = msg[IDX_TCP_MSG_PORT_OP_PORTNO];
	char regValue[SIZE_MAX_OPTION_STR];
	char regAddr[SIZE_MAX_OPTION_STR];
	int i=0;
	int numPortsToRead = 1;

	if(portNo != PORT_NO_ALL_PORTS)
	{
		regValue[0] = msg[IDX_TCP_MSG_INT_ST_OP_VALUE];
		regValue[1] = 0;
		regAddr[0] = addressListIntStatus[(int)portNo];
		regAddr[1] = 0;
	}
	else
	{
		for(i=0; i<MAX_NUM_GPORTS; i++)
		{
			regAddr[i] = addressListIntStatus[i];
			regValue[i] = msg[IDX_TCP_MSG_INT_ST_OP_VALUE];
		}
		regAddr[i] = 0;
		numPortsToRead = 8;
	}

	switch((unsigned char)(msg[IDX_TCP_MSG_PORT_OP_CODE]))
	{
	case intStatusOperationwrite:
		TxToDevice(regValue, regAddr);
		break;

	case intStatusOperationRead:
		RxToDevice(regValue, regAddr, numPortsToRead);
		break;
	}
	return -1;
}

int HandleCmd_PWMOperation(char *msg)
{
	char pwmNo = msg[IDX_TCP_MSG_PWM_OP_PWMNO];
	char regValue[SIZE_MAX_OPTION_STR];
	char regAddr[SIZE_MAX_OPTION_STR];
	char oldValConfigReg = 0;
	char newValConfigReg = 0;

	//Select pwm for current operation
	regValue[0] = pwmNo;
	regAddr[0] = regAddrPWMSelect;
	regAddr[1]= 0;
	TxToDevice(regValue, regAddr);
/*
	switch((unsigned char)(msg[IDX_TCP_MSG_PWM_OP_CODE]))
	{
	case pwmOperationClkSource:
		regAddr[0] = regAddrPWMConfig;
		regAddr[1]= 0;
		RxToDevice(regValue, regAddr, 1);
		oldValConfigReg = regValue[0];
		oldValConfigReg &= 0xF8;

		newValConfigReg = msg[IDX_TCP_MSG_PWM_OP_VALUE];
		newValConfigReg &= 0x07;

		newValConfigReg |= oldValConfigReg; //Or both the new and old values to get the new one

		TxToDevice(regValue, regAddr);
		break;

	case pwmOperationIntEdge:
		regAddr[0] = regAddrPWMConfig;
		regAddr[1]= 0;
		RxToDevice(regValue, regAddr, 1);
		oldValConfigReg = regValue[0];
		oldValConfigReg &= 0xF7;

		newValConfigReg = msg[IDX_TCP_MSG_PWM_OP_VALUE] == 0x00? 0x00: 0x08;

		newValConfigReg |= oldValConfigReg; //Or both the new and old values to get the new one

		TxToDevice(regValue, regAddr);
		break;

	case pwmOperationPrgmDivider:
		regAddr[0] = regAddrPWMProgrammableDivider;
		regAddr[1]= 0;

	case pwmOperationSetPeriod:
		regValue[0] = msg[IDX_TCP_MSG_PWM_OP_VALUE];
		regAddr[0] = regAddrPWMPeriod;
		regAddr[1]= 0;
		TxToDevice(regValue, regAddr);
		break;

	case portOperationRead:
		RxToDevice(regValue, regAddr, 1);
		break;

	case portOperationConfig:
		regValue[0] = portNo;
		regAddr[0] = regAddrPortSelect;
		regAddr[1] = 0;
		TxToDevice(regValue, regAddr);
		switch((unsigned char)(msg[IDX_TCP_MSG_PORT_CONFIG_CODE]))
		{
		case portConfigIntMask:			regAddr[0] = regAddrIntMask;		break;
		case portConfigPWMOut:			regAddr[0] = regAddrPWMPortOut;	break;
		case portConfigInvertion:		regAddr[0] = regAddrInversion;		break;
		case portConfigPinDirection:	regAddr[0] = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		}
		regValue[0] = msg[IDX_TCP_MSG_PORT_CONFIG_VAL];
		TxToDevice(regValue, regAddr);
		break;
	}
*/
	return -1;
}

int HandleCmd_POROperation(char *msg)
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
