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
	//int Adapter_No = 0,Ret_Val=1;
	//char FileName[20];
	//snprintf(FileName, 19, "/dev/i2c-%d", Adapter_No);
	//snprintf(FileName, 19, devName, Adapter_No);
	//File = open(FileName, O_RDWR);
	int retVal = -1;
	File = open(devName, O_RDWR);
	if (File < 0)
	{
		perror("");
		printf("\nFAULURE in openning I2C dev at %s...\n",devName);
		retVal = -1;
	}
	else
	{
		printf("\nI2C dev opened susccessfuly at %s",devName); fflush(stdout);
		retVal = 0;
	}
	return retVal;
}

void CloseI2C()
{
	close(File);
}

int  DeviceAddress(int addr)
{	
	int Ret_Val = 0;
	Ret_Val = ioctl(File, I2C_SLAVE, addr);
	if (Ret_Val < 0)
	{
		printf("\n Invalid Device Address");
	}
	return Ret_Val;
}

//#ifdef _I2C_INTERFACE_

int  TxToDevice(unsigned char *Vals,int ValsLen,unsigned char* Reg,int RegLen)
{
	/*
	 * Vals : Values to be wriiten to the reg
	 * Reg	: Register address
	*/
	unsigned char *Buffer,Ret_Val=1;
	int Len = ValsLen + RegLen;
	Buffer = (unsigned char *)malloc( Len * (sizeof(unsigned char)) );
	memcpy(Buffer,Reg,RegLen);
	if(ValsLen != 0)
		memcpy(&Buffer[RegLen],Vals,ValsLen);
	if ( write(File, Buffer, Len) != Len)
	{
		printf("\n Error Occurred in Writing to Device");
		Ret_Val=0;
	}
	return Ret_Val;
}


int  RxToDevice(unsigned char *Reg,int RegLen,unsigned char * ReadVals,int Len)
{
	/*
	 * Reg : Register to be read
	 * RegLen : Len of Register Address
	 * ReadVals : will have the read Value (Byte)
	 * Len : Len of data to be read
	 * Note: ReadVals should be of atleast ReadVals' size
	*/
	int Ret_Val=1;
	if(TxToDevice(0,0,Reg,RegLen) == 1)
	{
		if (read(File, ReadVals, Len) != Len)
		{
			printf("\n Error Occurred in Reading from Device ");
			Ret_Val=0;
		}
	}else
	{
		Ret_Val=0;
	}
	return Ret_Val;
}

int  EepromDisable()
{
	unsigned char Data[10];
	unsigned char Addr[10];
	int Ret_Val=1;
	Ret_Val= DeviceAddress(ADDR_MULTI_IO_PORT_DEVICE);
	Addr[0] =ADDR_ENABLE_REGISTER;
	Data[0] = 0X43;
	Data[1] = 0X4D;
	Data[2] = 0X53;
	Data[3] = 0X00;
	Ret_Val |= TxToDevice(Data,4,Addr,1);
	Ret_Val |= RxToDevice(Addr,1, Data, 1); // ok*/
	printf("\nValue Read is  %X",(unsigned char)(Data[0]));
	fflush(stdout);
	return Ret_Val;
}
int  EepromReadOnly()
{
	unsigned char Data[10];
	unsigned char Addr[10];
	int Ret_Val=1;
	Ret_Val= DeviceAddress(ADDR_MULTI_IO_PORT_DEVICE);
	Addr[0] =ADDR_ENABLE_REGISTER;
	Data[0] = 0X43;
	Data[1] = 0X4D;
	Data[2] = 0X53;
	Data[3] = 0X04;
	Ret_Val |= TxToDevice(Data,4,Addr,1);
	Ret_Val |= RxToDevice(Addr,1, Data, 1); // ok*/
	printf("\nValue Read is  %X",(unsigned char)(Data[0]));
	fflush(stdout);
	return Ret_Val;
}
int EepromWriteEnable()
{
	unsigned char Data[10];
	unsigned char Addr[10];
	int Ret_Val=1;
	Ret_Val= DeviceAddress(ADDR_MULTI_IO_PORT_DEVICE);
	Addr[0] =ADDR_ENABLE_REGISTER;
	Data[0] = 0X43;
	Data[1] = 0X4D;
	Data[2] = 0X53;
	Data[3] = 0X02;
	Ret_Val |= TxToDevice(Data,4,Addr,1);
	Ret_Val |= RxToDevice(Addr,1, Data, 1); // ok*/
	printf("\nValue Read is  %X",(unsigned char)(Data[0]));
	fflush(stdout);
	return Ret_Val;
}


