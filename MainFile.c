
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

#include "global.h"
#include "TCPComm.h"
#include "I2CFunctions.h"

char buildTimeInfo[SIZE_MAX_OPTION_STR];

char configFileName[256];
char I2CDevName[256];
char TCP_Listener_IP[32];
unsigned int TCP_Listener_Port;
char LogFileName[256];
FILE *logFile = NULL;

void ReadConfigurationParams()
{
	FILE *configFileP;
	char configLine[256];
	char configKey[256];
	char configVal[256];

	I2CDevName[0] = 0;
	TCP_Listener_IP[0] = 0;
	TCP_Listener_Port = 0;
	LogFileName[0] = 0;

	configFileP = fopen(configFileName,"r");
	if(configFileP == NULL)
	{
		perror("");
		printf("\nCould not open configuration file at %s",configFileName);
		printf("\nExiting...");
		exit(-1);
	}

	while(fgets(configLine, 256, configFileP) != NULL)
	{
		if(configLine[0] == '#') continue;

		sscanf(configLine, "%s %s",configKey, configVal);


		if(strcmp(configKey, CONFIG_KEY_I2C_DEV) == 0)			strcpy(I2CDevName, configVal);
		else if(strcmp(configKey, CONFIG_KEY_TCP_IP) == 0)		strcpy(TCP_Listener_IP, configVal);
		else if(strcmp(configKey, CONFIG_KEY_TCP_PORT) == 0)	TCP_Listener_Port = (unsigned int)atoi(configVal);
		else if(strcmp(configKey, CONFIG_KEY_LOG_FILE) == 0)	strcpy(LogFileName, configVal);
	}

	if(I2CDevName[0] == 0)
	{
		printf("\nConfiguration for I2C_Dev_Name is missing. Exiting...\n");
		exit(-1);
	}
	if(TCP_Listener_IP[0] == 0)
	{
		printf("\nConfiguration for TCP_Listener_IP is missing. Exiting...\n");
		exit(-1);
	}
	if(TCP_Listener_Port == 0)
	{
		printf("\nConfiguration for TCP_Listener_Port is missing. Exiting...\n");
		exit(-1);
	}
	if(LogFileName[0] == 0)
	{
		printf("\nConfiguration for Log_File_Name is missing. Exiting...\n");
		exit(-1);
	}

	printf("\nI2C_Dev_Name---------%s",I2CDevName);
	printf("\nTCP_Listener_IP------%s",TCP_Listener_IP);
	printf("\nTCP_Listener_Port----%u",TCP_Listener_Port);
	printf("\nLogFileName----------%s",LogFileName);
	fflush(stdout);
}

void PrintHelp(char *programName)
{
	printf("\n");
	printf("usage: %s [options...]\n",programName);
	printf
	(
		"options:"
		"\n-c <Config File> Configuration file for all parameters"
		"\nDefault Values are:"
		"\n-c %s",
		DEFAULT_CONFIG_FILE_NAME
	);
	fflush(stdout);
}

void OpenLogFile()
{
	logFile = fopen(LogFileName,"a");
	if(logFile == NULL)
	{
		perror("");
		printf("\nCould not open log file at %s",LogFileName);
		printf("\nExiting...");
		exit(-1);
	}
	else
	{
		printf("\nLog file opened at %s",LogFileName);
		fflush(stdout);
	}
}

int main(int argc, char *argv[])
{
	///// Code for getting options starts here /////
	int optChar;

	strcpy(configFileName, DEFAULT_CONFIG_FILE_NAME);

	sprintf(buildTimeInfo,"Build time is %s %s",__DATE__, __TIME__);
	printf("\n%s\n",buildTimeInfo);
	fflush(stdout);

	while((optChar = getopt(argc, argv, "c:h")) != -1)
	{
		switch(optChar)
		{
		case 'c':
			strcpy(configFileName, optarg);
			break;

		case 'h':
			PrintHelp(argv[0]);
			exit(0);
		}
	}

	///// Code for getting optins ends here //////
	ReadConfigurationParams();
	OpenLogFile();
	fprintf(logFile, "\n=====================");
	fprintf(logFile, "\n%u; Demon started", GetMillis());
	fflush(logFile);
	TCPComm_Start();

	return 0;

	/*
	OpenI2C("dummy");
	EepromWriteEnable();

	DeviceAddress(ADDR_EEPROM);
	tempAddr[0] = 0xC0;
	tempAddr[1] = 0x01;
	tempRead[0] = 0X55;

	TxToDevice(tempRead,1,tempAddr,2);
	tempRead[0] = 0x22;
	RxToDevice(tempAddr,2, tempRead, 1);
	printf("\nValue Read  %X",(unsigned char)(tempRead[0]));
	fflush(stdout);


	SendValToMultiportReg(0x01, 0x08); //endable port output first

	SendValToMultiportReg(0x00, 0x18); //select port 0 for config

	SendValToMultiportReg(0x01, 0x1A); //select port 0 ping 0 as pwm out

	SendValToMultiportReg(0x07, 0x28); //selecte pwm no 7.

	SendValToMultiportReg(0x04, 0x29); //programmable clk source

	SendValToMultiportReg(0x04, 0x2A); //set period

	SendValToMultiportReg(0x03, 0x2B); //pulse widdth = 3/4. Duty cycle = 75%
*/



}


unsigned int GetMillis()
{
	struct timeval currentTimeVal;

	gettimeofday(&currentTimeVal, NULL);

	return (unsigned int)((currentTimeVal.tv_sec * 1000) + (currentTimeVal.tv_usec / 1000));
}
