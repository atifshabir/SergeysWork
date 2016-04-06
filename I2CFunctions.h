#ifndef _I2C_H
#define _I2C_H
void OpenI2C();
void DeviceAddress(int addr);
void WriteI2C(char *Buffer);
void WriteSMBus(char *Buffer,int DataLength);
void ReadI2C(char *Buffer);
void ReadSMBus(char Register,int DataLength,char *Res);

#endif