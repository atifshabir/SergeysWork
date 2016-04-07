#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include "I2CFunctions.h"
#include "global.h"
static int File;
int OpenI2C(char *devName)
{
	int Adapter_No = 2; /* probably dynamically determined */
	char FileName[20];
	//snprintf(FileName, 19, "/dev/i2c-%d", Adapter_No);
	snprintf(FileName, 19, devName, Adapter_No);
	File = open(FileName, O_RDWR);
	if (File < 0)
	{
		return RESULT_FAILURE;
	}
	else
	{
		return RESULT_SUCCESS;
	}
}

void DeviceAddress(int addr)
{	
	if (ioctl(File, I2C_SLAVE, addr) < 0)
		exit(1);

}
void WriteI2C(char *Buffer)
{
	/*
	Buffer[0] = //register;
	Buffer[1] = //Val;
	Buffer[2] = //Val;
	*/
	if ( write(File, Buffer, 3) != 3)
		printf("\n Error Occurred in Writing to Device");	
}

void ReadI2C(char *Buffer)
{
	/*
		Buffer will have the read Value (Byte)
	*/
	if (read(File, Buffer, 1) != 1)
		printf("Error Occurred in Reading from Device ");
}
