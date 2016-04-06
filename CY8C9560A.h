
#ifndef CY8C9560A_H_
#define CY8C9560A_H_

enum MsgType
{
	openDevice		= 0x01,
	closeDevice		= 0x02,
	deviceCommand	= 0x03,
	ack				= 0x04,
	nack			= 0x05
};

enum CommandType
{
	portOperation	= 0x01,
	pwmOperation	= 0x02,
	miscOperation	= 0x03,
	EEPROMOperation	= 0x04
};

enum GPortCommand
{
	readPort		= 0x01,
	writePort		= 0x02,
	configPort		= 0x03
};

enum GPortCommandConfig
{
	intMask			= 0x01,
	pwmOut			= 0x02,
	invertion		= 0x03,
	pinDirection	= 0x04,
	driveMode		= 0x05
};


#endif // CY8C9560A_H_
