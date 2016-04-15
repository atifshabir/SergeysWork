
#ifndef CY8C9560A_H_
#define CY8C9560A_H_

#define IDX_TCP_MSG_ERRCODE				5

#define IDX_TCP_MSG_CMD_TPE				6
#define IDX_TCP_MSG_DEV_NAME			6

#define IDX_TCP_MSG_PORT_OP_PORTNO		7 //Port operation port number
#define IDX_TCP_MSG_PORT_OP_CODE		8 //Port operation code eg write, read etc
#define IDX_TCP_MSG_PORT_OP_VALUE		9 //Port operation value eg what needs to be written to port.
#define IDX_TCP_MSG_PORT_CONFIG_CODE	9 //Port configuration
#define IDX_TCP_MSG_PORT_CONFIG_VAL		10 //Port configuration

#define IDX_TCP_MSG_PWM_OP_PWMNO		7 //Port operation port number
#define IDX_TCP_MSG_PWM_OP_CODE			8 //Port operation code eg write, read etc
#define IDX_TCP_MSG_PWM_OP_VALUE		9 //Port operation value eg what needs to be written to port.
#define IDX_TCP_MSG_PWM_OP_CLK_SRC		9
#define IDX_TCP_MSG_PWM_OP_CLK_DIV		10
#define IDX_TCP_MSG_PWM_OP_PERIOD		11
#define IDX_TCP_MSG_PWM_OP_PULSE_WIDTH	12
#define IDX_TCP_MSG_PWM_OP_INT_EDGE		13

#define IDX_TCP_MSG_PIN_OP_PORTNO		7 //Pin operation port number
#define IDX_TCP_MSG_PIN_OP_PINMASK		8 //Pin operation pin mask
#define IDX_TCP_MSG_PIN_OP_CODE			9 //Pin operation code eg write, read etc
#define IDX_TCP_MSG_PIN_OP_VALUE		10 //Pin operation value eg what needs to be written to port.
#define IDX_TCP_MSG_PIN_CONFIG_CODE		10 //Pin configuration
#define IDX_TCP_MSG_PIN_CONFIG_VAL		11 //Pin configuration

#define IDX_TCP_MSG_INT_ST_OP_PORTNO	7 //Port operation port number
#define IDX_TCP_MSG_INT_ST_OP_CODE		8 //Port operation port number
#define IDX_TCP_MSG_INT_ST_OP_VALUE		9 //Port operation value eg what needs to be written to port.

#define IDX_TCP_MSG_POR_OP_CODE			7 //Configuration loading and saving to eeprom

#define IDX_TCP_MSG_EEPROM_OP_CODE		7 //Opertion code for eeprom operation
#define IDX_TCP_MSG_EEPROM_OP_ADDR		8 //Address for eeprom operation
#define IDX_TCP_MSG_EEPROM_OP_LEN		10 //Len in bytes for eeprom operation
#define IDX_TCP_MSG_EEPROM_OP_DATA		12 //Data in eeprom operation

#define PIN_HIGH			0x01
#define PIN_LOW				0x00
#define PORT_NO_ALL_PORTS	0xFF
#define MAX_NUM_GPORTS		8

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
	pwmOperationSetConfig,
	pwmOperationGetConfig
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
	porOperationSaveConfig,
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

enum DevAddresses
{
	devAddrNULL,
	devAddrMultiport,
	devAddrEEPROM
};

int SendValToMultiportReg(unsigned char regAddr, unsigned char value);
int RxValFromMultiportReg(unsigned char regAddr, unsigned char *value);

int HandleMsg_Command(unsigned char *msg);
int HandleMsg_OpenDev(unsigned char *msg);
int HandleMsg_CloseDev(unsigned char *msg);
int HandleMsg_ACK(unsigned char *msg);
int HandleMsg_NACK(unsigned char *msg);

int HandleCmd_PortOperation(unsigned char *msg);
int HandleCmd_PinOperation(unsigned char *msg);
int HandleCmd_PWMOperation(unsigned char *msg);
int HandleCmd_MiscOperation(unsigned char *msg);
int HandleCmd_POROperation(unsigned char *msg);
int HandleCmd_IntStausOperation(unsigned char *msg);
int HandleCmd_EEPRomOperation(unsigned char *msg);

#endif // CY8C9560A_H_
