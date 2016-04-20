
#include <stdio.h>
#include <string.h>

#include "CY8C9560A.h"
#include "I2CFunctions.h"
#include "global.h"
#include "TCPComm.h"

unsigned char addressListInport[8] = {regAddrInport0, regAddrInport1, regAddrInport2, regAddrInport3,
							regAddrInport4, regAddrInport5, regAddrInport6, regAddrInport7};


unsigned char addressListOutport[8] = {regAddrOutport0, regAddrOutport1, regAddrOutport2, regAddrOutport3,
							regAddrOutport4, regAddrOutport5, regAddrOutport6, regAddrOutport7};

unsigned char addressListIntStatus[8] ={regAddrintStatusPort0,regAddrintStatusPort1,regAddrintStatusPort2,regAddrintStatusPort3,
								regAddrintStatusPort4, regAddrintStatusPort5, regAddrintStatusPort6, regAddrintStatusPort7};

unsigned int flagI2CDevIsOpen = FLAG_FALSE;
//unsigned int flagI2CDevIsOpen = FLAG_TRUE;
unsigned int flagCurrentDevAddr = devAddrNULL;

extern char I2CDevName[];
extern FILE *logFile;

int SendValToMultiportReg(unsigned char addr, unsigned char value)
{
	unsigned char regAddr[4];
	unsigned char regVal[4];
	regAddr[0] = addr;
	regAddr[1] = 0;
	regVal[0] = value;
	fprintf(logFile, "\n%u; Write to multiport reg: addr[0x%2X], value[0x%2X]",
			GetMillis(), regAddr[0], regVal[0]);
	return TxToDevice(regVal, 1,  regAddr, 1);
}

int RxValFromMultiportReg(unsigned char addr, unsigned char *value)
{
	int retVal = 0;
	unsigned char regAddr[4];
	unsigned char regVal[4];
	regAddr[0] = addr;
	regAddr[1] = 0;
	retVal =  RxToDevice(regAddr, 1, regVal, 1);
	*value = regVal[0];
	fprintf(logFile, "\n%u; Read from multiport reg: addr[0x%2X], value[0x%2X]",
			GetMillis(),regAddr[0], regVal[0]);

	return retVal;
}

int HandleMsg_Command(unsigned char *msg)
{
	int retVal = errCodeSucces;

	if(flagI2CDevIsOpen == FLAG_TRUE)
	{
		if(msg[IDX_TCP_MSG_CMD_TPE] != cmdTypeEEPROMOperation)
		{
			if(flagCurrentDevAddr != devAddrMultiport)
			{
				if(DeviceAddress(ADDR_MULTI_IO_PORT_DEVICE) < 0) {return errCodeDevAddrFailed;}
				else { flagCurrentDevAddr = devAddrMultiport; }
			}
		}
		else
		{
			if(flagCurrentDevAddr != devAddrEEPROM)
			{
				if(DeviceAddress(ADDR_EEPROM) < 0) {return errCodeDevAddrFailed;}
				else { flagCurrentDevAddr = devAddrEEPROM; }
			}
		}

		switch(msg[IDX_TCP_MSG_CMD_TPE])
		{
		case cmdTypePortOperation:
			retVal = HandleCmd_PortOperation(msg);
			break;
		case cmdTypePWMOperation:
			retVal = HandleCmd_PWMOperation(msg);
			break;
		case cmdTypeMiscOperation:
			retVal = HandleCmd_MiscOperation(msg);
			break;
		case cmdTypePinOperation:
			retVal = HandleCmd_PinOperation(msg);
			break;
		case cmdTypePOROperation:
			retVal = HandleCmd_POROperation(msg);
			break;
		case cmdTypeIntStatusOperation:
			retVal = HandleCmd_IntStausOperation(msg);
			break;
		case cmdTypeMPortRegOperation:
			retVal = HandleCmd_MPortRegOperation(msg);
			break;
		case cmdTypeEEPROMOperation:
			retVal = HandleCmd_EEPRomOperation(msg);
			break;
		default:
			retVal = errCodeInvalidCmd;
    		fprintf(logFile, "\n%u; Msg type invalid [0x%X]", GetMillis(),msg[IDX_TCP_MSG_CMD_TPE]);
    		fflush(logFile);
    		printf("\n%s: WARNING: Invalid tcpMsgCmdType received [%u]",
					__PRETTY_FUNCTION__, msg[IDX_TCP_MSG_CMD_TPE]);
			fflush(stdout);
		}
	}
	else
	{
		fprintf(logFile, "\n%u; Dev command recived while dev not opened", GetMillis());
		fflush(logFile);
		retVal = errCodeI2CDevNotOpened;
	}
	return retVal;
}

int HandleMsg_OpenDev(unsigned char *msg)
{
	int retVal = errCodeSucces;
	int res = 0;
	if(flagI2CDevIsOpen == FLAG_FALSE)
	{
		res = OpenI2C(I2CDevName);
		if(res != 0)
		{
			retVal = errCodeOpeningI2CDevFailed;
		}
		else
		{
			EepromWriteEnable();
			flagI2CDevIsOpen = FLAG_TRUE;
			fprintf(logFile, "\n%u; Dev opened", GetMillis());
			fflush(logFile);
		}
	}
	else
	{
		retVal = errCodeI2CDevAlreadyOpened;
	}

	return retVal;
}

int HandleMsg_CloseDev(unsigned char *msg)
{
	if(flagI2CDevIsOpen == FLAG_TRUE)
	{
		CloseI2C();
		fprintf(logFile, "\n%u; Dev closed", GetMillis());
		fflush(logFile);
		return errCodeSucces;
	}
	else
	{
		fprintf(logFile, "\n%u; CloseDev recived while dev not opened", GetMillis());
		fflush(logFile);
		return errCodeI2CDevNotOpened;
	}
}

int HandleMsg_ACK(unsigned char *msg)
{
	int retVal = -1;
	return retVal;
}

int HandleMsg_NACK(unsigned char *msg)
{
	int retVal = -1;
	return retVal;
}

int HandleCmd_PortOperation(unsigned char *msg)
{
	unsigned char portNo = msg[IDX_TCP_MSG_PORT_OP_PORTNO];
	int retVal = errCodeSucces;
	int res = 0;
	unsigned char value = 0;
	unsigned char regAddrForConfig = 0;

	if(portNo > 7)
	{
		retVal = errCodeInvalidPortNo;
		return retVal;
	}

	switch(msg[IDX_TCP_MSG_PORT_OP_CODE])
	{
	case portOperationwrite:
		res = SendValToMultiportReg(addressListOutport[(int)portNo], msg[IDX_TCP_MSG_PORT_OP_VALUE]);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		break;

	case portOperationRead:
		res = RxValFromMultiportReg(addressListInport[(int)portNo], &value);
		retVal = res < 0 ? errCodeReadFromDevFailed : errCodeSucces;
		msg[IDX_TCP_MSG_PORT_OP_VALUE] = value;
		break;

	case portOperationConfig:
		res = SendValToMultiportReg(regAddrPortSelect, portNo);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		switch((unsigned char)(msg[IDX_TCP_MSG_PORT_CONFIG_CODE]))
		{
		case portConfigIntMask:			regAddrForConfig = regAddrIntMask;		break;
		case portConfigPWMOut:			regAddrForConfig = regAddrPWMPortOut;	break;
		case portConfigInvertion:		regAddrForConfig = regAddrInversion;	break;
		case portConfigPinDirection:	regAddrForConfig = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		default: retVal = errCodeInvalidPortConfigCode;
		}
		if(retVal == errCodeSucces)
		{
			SendValToMultiportReg(regAddrForConfig, msg[IDX_TCP_MSG_PORT_CONFIG_VAL]);
		}
		break;

	default:
		retVal = errCodeInvalidPortOperation;
		fprintf(logFile, "\n%u; Port operation invalid [0x%X]",	GetMillis(),msg[IDX_TCP_MSG_PORT_OP_CODE]);
		fflush(logFile);

	}
	return retVal;
}

int HandleCmd_PinOperation(unsigned char *msg)
{
	int retVal = errCodeSucces;
	int res = 0;
	unsigned char portNo = msg[IDX_TCP_MSG_PIN_OP_PORTNO];
	unsigned char pinMask = msg[IDX_TCP_MSG_PIN_OP_PINMASK];
	unsigned char newPortVal = 0;
	unsigned char oldPortVal = 0;
	unsigned char regAddrForPortConfig = 0;

	if(portNo > 7)
	{
		retVal = errCodeInvalidPortNo;
		return retVal;
	}

	switch(msg[IDX_TCP_MSG_PIN_OP_CODE])
	{
	case pinOperationwrite:
		res = RxValFromMultiportReg(addressListOutport[(int)portNo], &oldPortVal);
		retVal = res < 0 ? errCodeReadFromDevFailed : errCodeSucces;
		oldPortVal &= ~pinMask; //clear the bits of old val accoriding to pinMask

		newPortVal = msg[IDX_TCP_MSG_PIN_OP_VALUE] == PIN_HIGH? 0xFF: 0x00;
		newPortVal &= pinMask; //all bits except the mask are cleared. Masked pins made accornding to value

		newPortVal |= oldPortVal; //Or teh newVal wih Oldval and store to newVal
		res = SendValToMultiportReg(addressListOutport[(int)portNo], newPortVal);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		break;

	case pinOperationRead:
		res = RxValFromMultiportReg(addressListOutport[(int)portNo], &oldPortVal);
		retVal = res < 0 ? errCodeReadFromDevFailed : errCodeSucces;
		oldPortVal &= pinMask; //clear the bits of old val accoriding to pinMask
		msg[IDX_TCP_MSG_PIN_OP_VALUE] = oldPortVal;
		break;

	case pinOperationConfig:
		res = SendValToMultiportReg(regAddrPortSelect, portNo);

		switch((unsigned char)(msg[IDX_TCP_MSG_PIN_CONFIG_CODE]))
		{
		case pinConfigIntMask:		regAddrForPortConfig = regAddrIntMask;		break;
		case pinConfigPWMOut:		regAddrForPortConfig = regAddrPWMPortOut;		break;
		case pinConfigInvertion:	regAddrForPortConfig = regAddrInversion;		break;
		case pinConfigPinDirection:	regAddrForPortConfig = regAddrPinDirection;	break;
		//case portConfigDriveMode:		msgOnI2C[0] = regAddrDriveMode;	break;
		default: retVal = errCodeInvalidPinConfigCode;
		}

		res = RxValFromMultiportReg(regAddrForPortConfig, &oldPortVal);
		retVal = res < 0 ? errCodeReadFromDevFailed : errCodeSucces;
		oldPortVal &= ~pinMask; //clear the bits of old val accoriding to pinMask

		newPortVal = msg[IDX_TCP_MSG_PIN_CONFIG_VAL] == PIN_HIGH? 0xFF: 0x00;
		newPortVal &= pinMask; //all bits except the mask are cleared. Masked pins made accornding to value

		newPortVal |= oldPortVal; //Or teh newVal wih Oldval and store to newVal
		if(retVal == errCodeSucces)
		{
			res = SendValToMultiportReg(regAddrForPortConfig, newPortVal);
			retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		}
		break;

	default:
		retVal = errCodeInvalidPinOperation;
		fprintf(logFile, "\n%u; Pin operation invalid [0x%X]", GetMillis(),msg[IDX_TCP_MSG_PIN_OP_CODE]);
		fflush(logFile);

	}
	return retVal;
}

int HandleCmd_IntStausOperation(unsigned char *msg)
{
	unsigned char portNo = msg[IDX_TCP_MSG_INT_ST_OP_PORTNO];
	int retVal = errCodeSucces;
	int res = 0;
	int i = 0;

	if(portNo < MAX_NUM_GPORTS || portNo == PORT_NO_ALL_PORTS)
	{
		switch((unsigned char)(msg[IDX_TCP_MSG_INT_ST_OP_CODE]))
		{
		case intStatusOperationwrite:
			if(portNo != PORT_NO_ALL_PORTS)
			{
				res = SendValToMultiportReg(addressListIntStatus[(int)portNo], msg[IDX_TCP_MSG_INT_ST_OP_VALUE]);
				retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
			}
			else
			{
				for(i = 0; i< MAX_NUM_GPORTS; i++)
				{
					res = SendValToMultiportReg(addressListIntStatus[(int)i], msg[IDX_TCP_MSG_INT_ST_OP_VALUE+i]);
					if(retVal < 0) { retVal = errCodeWriteToDevFailed; break;}
				}
			}
			break;

		case intStatusOperationRead:
			if(portNo != PORT_NO_ALL_PORTS)
			{
				res = RxValFromMultiportReg(addressListIntStatus[(int)portNo], &(msg[IDX_TCP_MSG_INT_ST_OP_VALUE]));
				retVal = res < 0 ? errCodeReadFromDevFailed : errCodeSucces;
			}
			else
			{
				for(i = 0; i< MAX_NUM_GPORTS; i++)
				{
					res = RxValFromMultiportReg(addressListIntStatus[(int)i], &(msg[IDX_TCP_MSG_INT_ST_OP_VALUE+i]));
					if(retVal < 0) { retVal = errCodeReadFromDevFailed; break;}
				}
			}
			break;

		default:
			retVal = errCodeInvalidIntStatusOperation;

		}
	}
	else
	{
		retVal = errCodeInvalidPortNo;
	}

	return retVal;
}

int HandleCmd_PWMOperation(unsigned char *msg)
{
	int retVal = errCodeSucces;
	int res = 0;
	unsigned char oldValConfigReg = 0;
	unsigned char newValConfigReg = 0;

	unsigned char pwmOpCode = msg[IDX_TCP_MSG_PWM_OP_CODE];
	unsigned char pwmNo = msg[IDX_TCP_MSG_PWM_OP_PWMNO];
	unsigned char clkSrc = msg[IDX_TCP_MSG_PWM_OP_CLK_SRC];
	unsigned char clkDivider = msg[IDX_TCP_MSG_PWM_OP_CLK_DIV];
	unsigned char period = msg[IDX_TCP_MSG_PWM_OP_PERIOD];
	unsigned char pulseWidth = msg[IDX_TCP_MSG_PWM_OP_PULSE_WIDTH];
	unsigned char intEdge = msg[IDX_TCP_MSG_PWM_OP_INT_EDGE];

	if(pwmNo > 15)
	{
		retVal = errCodeInvalidPWMNo;
		return retVal;
	}

	switch(pwmOpCode)
	{
	case pwmOperationSetConfig:
		//check the valididity of all params
		if(
			pwmNo > 15 ||
			clkSrc > 5 ||
			clkDivider == 0 ||
			period == 0 ||
			pulseWidth >= period ||
			intEdge > 1
			)
		{
			retVal = errCodeInvalidPWMParams;
		}
		else
		{
			do
			{
				//select pwm no
				res = SendValToMultiportReg(regAddrPWMSelect, pwmNo);
				if(res < 0) { retVal = errCodeWriteToDevFailed; break; }

				//select clk src
				res = RxValFromMultiportReg(regAddrPWMConfig, &oldValConfigReg);
				if(res < 0) { retVal = errCodeReadFromDevFailed; break; }
				oldValConfigReg &= 0xF8;
				newValConfigReg = clkSrc;
				newValConfigReg &= 0x07;
				newValConfigReg |= oldValConfigReg; //Or both the new and old values to get the new one
				res = SendValToMultiportReg(regAddrPWMConfig, clkSrc);
				if(res < 0) { retVal = errCodeWriteToDevFailed; break; }

				//select divider
				if(clkSrc == 0x04)
				{
					res = SendValToMultiportReg(regAddrPWMProgrammableDivider, clkDivider);
					if(res < 0) { retVal = errCodeWriteToDevFailed; break; }
				}
				//select period
				if(clkSrc == 0x04)
				{
					res = SendValToMultiportReg(regAddrPWMPeriod, period);
					if(res < 0) { retVal = errCodeWriteToDevFailed; break; }
				}

				//select pulse width
				if(clkSrc == 0x04)
				{
					res = SendValToMultiportReg(regAddrPWMPulseWidth, pulseWidth);
					if(res < 0) { retVal = errCodeWriteToDevFailed; break; }
				}

				//select int edge
				res = RxValFromMultiportReg(regAddrPWMConfig, &oldValConfigReg);
				if(res < 0) { retVal = errCodeReadFromDevFailed; break; }
				oldValConfigReg &= 0xF7;
				newValConfigReg = intEdge == 0x00? 0x00: 0x08;
				newValConfigReg |= oldValConfigReg; //Or both the new and old values to get the new one
				res = SendValToMultiportReg(regAddrPWMConfig, newValConfigReg);
				if(res < 0) { retVal = errCodeWriteToDevFailed; break; }

			} while(0); //this do while will run only once just to avoid goto statment for aborting on first error
		}
		break;

	case pwmOperationGetConfig:
		do
		{
			//read pwmSelect
			res = RxValFromMultiportReg(regAddrPWMSelect, &pwmNo);
			if(res < 0) { retVal = errCodeReadFromDevFailed; break; }

			//read clk source and intEdge
			res = RxValFromMultiportReg(regAddrPWMConfig, &clkSrc);
			if(res < 0) { retVal = errCodeReadFromDevFailed; break; }
			intEdge = (clkSrc & 0x08) >> 3;
			clkSrc &= 0x07;

			//read prgmableDivider
			res = RxValFromMultiportReg(regAddrPWMProgrammableDivider, &clkDivider);
			if(res < 0) { retVal = errCodeReadFromDevFailed; break; }

			//read period
			res = RxValFromMultiportReg(regAddrPWMPeriod, &period);
			if(res < 0) { retVal = errCodeReadFromDevFailed; break; }

			//read pulsewidth
			res = RxValFromMultiportReg(regAddrPWMPulseWidth, &pulseWidth);
			if(res < 0) { retVal = errCodeReadFromDevFailed; break; }

			msg[IDX_TCP_MSG_PWM_OP_CODE] = pwmOpCode;
			msg[IDX_TCP_MSG_PWM_OP_PWMNO] = pwmNo;
			msg[IDX_TCP_MSG_PWM_OP_CLK_SRC] = clkSrc;
			msg[IDX_TCP_MSG_PWM_OP_CLK_DIV] = clkDivider;
			msg[IDX_TCP_MSG_PWM_OP_PERIOD] = period;
			msg[IDX_TCP_MSG_PWM_OP_PULSE_WIDTH] = pulseWidth;
			msg[IDX_TCP_MSG_PWM_OP_INT_EDGE] = intEdge;

		} while(0); //this do while will run only once just to avoid goto statment for aborting on first error
		break;

	default:
		retVal = errCodeInvalidPWMOperation;
	}

	return retVal;
}

int HandleCmd_POROperation(unsigned char *msg)
{
	int retVal = errCodeSucces;
	int res = 0;

	switch(msg[IDX_TCP_MSG_POR_OP_CODE])
	{
	case porOperationSaveConfig:
		res = SendValToMultiportReg(regAddrCommand, 0x01);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		break;

	case porOperationFactoryReset:
		res = SendValToMultiportReg(regAddrCommand, 0x02);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		break;

	case porOperationPORReset:
		res = SendValToMultiportReg(regAddrCommand, 0x07);
		retVal = res < 0 ? errCodeWriteToDevFailed : errCodeSucces;
		break;

	default:
		retVal = errCodeInvalidPOROperation;
	}
	return retVal;
}

int HandleCmd_MiscOperation(unsigned char *msg)
{
	return RESULT_SUCCESS;
}

void IncrementEEPROMAddr(unsigned char *regAddr)
{
	if(regAddr[1] == 0xFF)
	{
		regAddr[1] = 0x00;
		regAddr[0] ++;
	}
	else
	{
		regAddr[1] ++;
	}
}

int HandleCmd_EEPRomOperation(unsigned char *msg)
{
	unsigned char regValue[SIZE_MAX_OPTION_STR];
	unsigned char regAddr[SIZE_MAX_OPTION_STR];
	int retVal = errCodeSucces;
	int res = 0;
	unsigned int i = 0;
	unsigned int len = (msg[IDX_TCP_MSG_EEPROM_OP_LEN] << 8)
						+ msg[IDX_TCP_MSG_EEPROM_OP_LEN+1];

	regAddr[0] = msg[IDX_TCP_MSG_EEPROM_OP_ADDR];
	regAddr[1] = msg[IDX_TCP_MSG_EEPROM_OP_ADDR+1];

	switch(msg[IDX_TCP_MSG_EEPROM_OP_CODE])
	{
	case eepromOperationWrite:
		fprintf(logFile, "\n%u; EEPROM write len [%u]; Data:\n", GetMillis(),len);
		for(i = 0; i < len; i++)
		{
			regValue[0] = msg[IDX_TCP_MSG_EEPROM_OP_DATA+i];
			fprintf(logFile, "[0x%2X @ 0x%2X%2X]",regValue[0], regAddr[0], regAddr[1]);
			res  = TxToDevice(regValue,1,regAddr,2);
			if(res < 0) {retVal = errCodeWriteToDevFailed; break; }
			IncrementEEPROMAddr(regAddr);
		}
		break;

	case eepromOperationRead:
		fprintf(logFile, "\n%u; EEPROM read len [%u]; Data:\n", GetMillis(),len);
		for(i = 0; i < len; i++)
		{
			res = RxToDevice(regAddr,2, regValue, 1);
			if(res < 0) {retVal = errCodeReadFromDevFailed; break; }

			fprintf(logFile, "[0x%2X @ 0x%2X%2X]",regValue[0], regAddr[0], regAddr[1]);
			msg[IDX_TCP_MSG_EEPROM_OP_DATA+i] = regValue[0];
			IncrementEEPROMAddr(regAddr);
		}
		break;

	default:
		retVal = errCodeInvalidEEPROMOperation;
	}

	fflush(logFile);
	return retVal;
}

int HandleCmd_MPortRegOperation(unsigned char *msg)
{
	unsigned char regValue;
	unsigned char regAddr;
	int retVal = errCodeSucces;
	int res = 0;

	regAddr = msg[IDX_TCP_MSG_MPORT_REG_OP_ADDR];
	regValue = msg[IDX_TCP_MSG_MPORT_REG_OP_DATA];

	switch(msg[IDX_TCP_MSG_MPORT_REG_OP_CODE])
	{
	case mportRegOperationWrite:
		res  = SendValToMultiportReg(regAddr, regValue);
		retVal = res < 0? errCodeWriteToDevFailed: errCodeSucces;
		break;

	case mportRegOperationRead:
		res = RxValFromMultiportReg(regAddr, &regValue);
		retVal = res < 0? errCodeReadFromDevFailed: errCodeSucces;
		msg[IDX_TCP_MSG_MPORT_REG_OP_DATA] = regValue;
		break;

	default:
		retVal = errCodeInalidMPortRegOperation;
	}

	return retVal;
}
