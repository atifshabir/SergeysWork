#include <stdio.h>
#include <linux/i2c-dev.h>
int File;	
void OpenI2C()
{
	int Adapter_No = 2; /* probably dynamically determined */
	char Filename[20];
	snprintf(Filename, 19, "/dev/i2c-%d", Adapter_No);
	File = open(Filename, O_RDWR);
	if (File < 0)
		exit(1);
}

void DeviceAddress(int addr)
{	
	if (ioctl(file, I2C_SLAVE, addr) < 0)
		exit(1);

}
void WriteI2C(char *Buffer)
{
	/*
	Buffer[0] = //register;
	Buffer[1] = //Val;
	Buffer[2] = //Val;
	*/
	if (write(File, Buffer, 3) ! =3)
		printf("\n Error Occurred in Writing to Device");	
}
void WriteSMBus(char *Buffer,int DataLength)
{
	/*
	Buffer[0] = //register;
	Buffer[1] = //Val;
	Buffer[2] = //Val;
	DataLength = Byte
	DataLength = Word
	*/	
	int Val;
	short *DataBuffer;
	if(DataLength == BYTE)
		Val = i2c_smbus_write_byte_data(fd , Buffer[0] , Buffer[1]);
	else
	{
		DataBuffer = (short *)&(Buffer[1]);	
		Val = i2c_smbus_write_word_data(fd , Buffer[0] , *(DataBuffer));
	}
}
void ReadI2C(char *Buffer)
{
	/*
		Buffer will have the read Value (Byte)
	*/
	if (read(File, Buffer, 1) != 1)
		printf("Error Occurred in Reading from Device ");
}
void ReadSMBus(char Register,int DataLength,char *Res)
{
	/*
		Register has the register value intended to be read
		DataLength determines either a byte is intended or a word data
	*/
	if(DataLength == BYTE)
		Res = i2c_smbus_read_word_data(file, Register);
	else
		Res = i2c_smbus_read_byte_data(file, Register);	
		
}