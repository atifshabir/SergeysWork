
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define SIZE_MAX_OPTION_STR	128
#define RESULT_SUCCESS	0
#define RESULT_FAILURE	-1

#define DEFAULT_CONFIG_FILE_NAME "/usr/local/config-demonCY8C9560A"

#define CONFIG_KEY_I2C_DEV		"DEV_Name"
#define CONFIG_KEY_TCP_IP		"TCP_Listen_IP"
#define CONFIG_KEY_TCP_PORT		"TCP_Listen_Port"
#define CONFIG_KEY_LOG_FILE		"Log_File"

#define FLAG_FALSE	0
#define FLAG_TRUE	1

enum ErrorCods
{
	errCodeSucces,
	errCodeInvalidMsgType,
	errCodeInvalidCmd,
	errCodeInvalidParams,
	errCodeInvalidPortNo,
	errCodeInvalidPortConfigCode,
	errCodeInvalidPortOperation,
	errCodeInvalidPinOperation,
	errCodeInvalidPinConfigCode,
	errCodeInvalidPWMOperation,
	errCodeInvalidPWMNo,
	errCodeInvalidPWMParams,
	errCodeInvalidIntStatusOperation,
	errCodeInvalidPOROperation,
	errCodeWriteToDevFailed,
	errCodeReadFromDevFailed,
	errCodeOpeningI2CDevFailed,
	errCodeI2CDevAlreadyOpened,
	errCodeI2CDevNotOpened,
	errCodeInvalidEEPROMOperation,
	errCodeDevAddrFailed
};

#endif // GLOBAL_H_
