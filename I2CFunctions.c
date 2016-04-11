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

#ifdef _I2C_INTERFACE_

void TxToDevice(char *Vals,char* Reg)
{
	/*
	 * Vals : Values to be wriiten to the reg
	 * Reg	: Register address
	*/
	char *Buffer;
	int Len = strlen(Vals) + strlen(Reg);
	Buffer = (char *)malloc( Len * (sizeof(char)) );
	memcpy(Buffer,Reg,strlen(Reg));
	memcpy(&Buffer[strlen(Reg)],Vals,strlen(Vals));

	if ( write(File, Buffer, Len) != Len)
		printf("\n Error Occurred in Writing to Device");	
}

void RxToDevice(char *Reg,char * ReadVals,int Len)
{
	/*
	 * Reg : Register to be read
	 * ReadVals : will have the read Value (Byte)
	 * Len : Len of data to be read
	 * Note: ReadVals should be of atleast ReadVals' size
	*/
	TxToDevice(NULL,Reg);
	if (read(File, ReadVals, Len) != Len)
		printf("Error Occurred in Reading from Device ");
}

#endif

