#ifndef _I2C_H
#define _I2C_H

#define BYTE 8
#define WORD 16
int OpenI2C(char *devName);
void DeviceAddress(int addr);
void WriteI2C(char *Buffer);
void WriteSMBus(char *Buffer,int DataLength);
void ReadI2C(char *Buffer);
void ReadSMBus(char Register,int DataLength,char *Res);

#endif
