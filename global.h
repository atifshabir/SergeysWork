
#ifndef GLOBAL_H_
#define GLOBAL_H_

#define SIZE_MAX_OPTION_STR	128

typedef struct ProgamOptions
{
	char *localIPAddr;
	unsigned int localPort;
	char *i2cDevName;
} ProgamOptions;

#endif // GLOBAL_H_
