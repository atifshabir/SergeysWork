
#ifndef CY8C9560A_H_
#define CY8C9560A_H_

enum TCPMsgTypes
{
	msgTypeNULL,
	msgTypeOpenDev,
	msgTypeCloseDevice,
	msgTypeCommand,
	msgTypeACK,
	msgTypeNACK
};

enum DeviceCommandTypes
{
	cmdTypeNULL,
	cmdTypePortOperation,
	cmdTypePWMOperation,
	cmdTypeMiscOperation,
	cmdTypePinOperation,
	cmdTypePOROperation,
	cmdTypeIntStatusOperation,
	cmdTypeEEPROMOperation = 0x81
};

enum GPortOperations
{
	portOperationNULL,
	portOperationRead,
	portOperationwrite,
	portOperationConfig
};

enum GPortConfigs
{
	portConfigNULL,
	portConfigIntMask,
	portConfigPWMOut,
	portConfigInvertion,
	portConfigPinDirection,
	portConfigDriveMode
};

enum PWMOperations
{
	pwmOperationNULL,
	pwmOperationClkSource,
	pwmOperationPrgmDivider,
	pwmOperationIntEdge,
	pwmOperationPeriod,
	pwmOperationSetIntGen,
	pwmOperationSetPulsewidth
};

enum MiscOperatins
{
	miscOperationNULL,
	miscOperationGetDevID,
	miscOperationSetWatchDog,
	miscOperationSetPOR
};

enum PinOperations
{
	pinOperationNULL,
	pinOperationRead,
	pinOperationwrite,
	pinOperationConfig
};

enum PinConfigs
{
	pinConfigNULL,
	pinConfigIntMask,
	pinConfigPWMOut,
	pinConfigInvertion,
	pinConfigPinDirection,
	pinConfigDriveMode
};

enum POROperation
{
	porOperationNULL,
	porOperationConfig,
	porOperationFactoryReset
};

enum EEPROMOperations
{
	eepromOperationNULL,
	eepromOperationWrite,
	eepromOperationRead
};

enum IntStatusOperation
{
	intStatusOperationNULL,
	intStatusOperationRead,
	intStatusOperationwrite
};

#define IDX_TCP_MSG_CMD_TPE				1
#define IDX_TCP_MSG_DEV_NAME			1

#define IDX_TCP_MSG_PORT_OP_PORTNO		2 //Port operation port number
#define IDX_TCP_MSG_PORT_OP_CODE		3 //Port operation code eg write, read etc
#define IDX_TCP_MSG_PORT_OP_VALUE		4 //Port operation value eg what needs to be written to port.
#define IDX_TCP_MSG_PORT_CONFIG_CODE	4 //Port configuration
#define IDX_TCP_MSG_PORT_CONFIG_VAL		5 //Port configuration

#define IDX_TCP_MSG_PWM_OP_PWMNO		2 //Port operation port number
#define IDX_TCP_MSG_PWM_OP_CODE			3 //Port operation code eg write, read etc
#define IDX_TCP_MSG_PWM_OP_VALUE		4 //Port operation value eg what needs to be written to port.

#define IDX_TCP_MSG_PIN_OP_PORTNO		2 //Pin operation port number
#define IDX_TCP_MSG_PIN_OP_PINMASK		3 //Pin operation pin mask
#define IDX_TCP_MSG_PIN_OP_CODE			4 //Pin operation code eg write, read etc
#define IDX_TCP_MSG_PIN_OP_VALUE		5 //Pin operation value eg what needs to be written to port.
#define IDX_TCP_MSG_PIN_CONFIG_CODE		5 //Pin configuration
#define IDX_TCP_MSG_PIN_CONFIG_VAL		6 //Pin configuration

#define IDX_TCP_MSG_INT_ST_OP_PORTNO	2 //Port operation port number
#define IDX_TCP_MSG_INT_ST_OP_VALUE		3 //Port operation value eg what needs to be written to port.

#define PIN_HIGH			0x01
#define PIN_LOW				0x00
#define PORT_NO_ALL_PORTS	0xFF
#define MAX_NUM_GPORTS		8

enum RegAdressInport
{
	regAddrInport0,
	regAddrInport1,
	regAddrInport2,
	regAddrInport3,
	regAddrInport4,
	regAddrInport5,
	regAddrInport6,
	regAddrInport7
};

enum RegAdressOutPort
{
	regAddrOutport0 = 0x08,
	regAddrOutport1,
	regAddrOutport2,
	regAddrOutport3,
	regAddrOutport4,
	regAddrOutport5,
	regAddrOutport6,
	regAddrOutport7
};

enum RegAddressInterruptStatus
{
	regAddrintStatusPort0 = 0x10,
	regAddrintStatusPort1,
	regAddrintStatusPort2,
	regAddrintStatusPort3,
	regAddrintStatusPort4,
	regAddrintStatusPort5,
	regAddrintStatusPort6,
	regAddrintStatusPort7
};

enum RegAddressConfigPort
{
	regAddrPortSelect = 0x18,
	regAddrIntMask,
	regAddrPWMPortOut,
	regAddrInversion,
	regAddrPinDirection,
	regAddrDriveModePullUp,
	regAddrDriveModePullDown,
	regAddrDriveModeOpenDrainH,
	regAddrDriveModeOpenDrainL,
	regAddrDriveModeStrong,
	regAddrDriveModeSlowStrong,
	regAddrDriveModeHighZ
};

enum RegAddressConfigPWM
{
	regAddrPWMSelect = 0x28,
	regAddrPWMConfig,
	regAddrPWMPeriod,
	regAddrPWMPulseWidth,
	regAddrPWMProgrammableDivider
};

enum RegAddressOthers
{
	regAddrEnable_WDE_EEE_ERO = 0x2D,
	regAddrDevID_Status,
	regAddrWatchdog,
	regAddrCommand
};

int HandleMsg_Command(char *msg);
int HandleMsg_OpenDev(char *msg);
int HandleMsg_CloseDev(char *msg);
int HandleMsg_ACK(char *msg);
int HandleMsg_NACK(char *msg);

int HandleCmd_PortOperation(char *msg);
int HandleCmd_PinOperation(char *msg);
int HandleCmd_PWMOperation(char *msg);
int HandleCmd_MiscOperation(char *msg);
int HandleCmd_POROperation(char *msg);
int HandleCmd_IntStausOperation(char *msg);
int HandleCmd_EEPRomOperation(char *msg);
#endif // CY8C9560A_H_
