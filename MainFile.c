
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "global.h"
#include "TCPComm.h"

ProgamOptions programOptions;
char buildTimeInfo[SIZE_MAX_OPTION_STR];

void SetDefaultProgramOptions()
{
	programOptions.localIPAddr = (char *)malloc(SIZE_MAX_OPTION_STR);
	strcpy(programOptions.localIPAddr, "127.0.0.1");
	programOptions.localPort = 10002;
	programOptions.i2cDevName =  (char *)malloc(SIZE_MAX_OPTION_STR);
	strcpy(programOptions.i2cDevName, "/dev/CY8C9560A");
}

void PrintHelp(char *programName)
{
	printf("\n");
	printf("usage: %s [options...]\n",programName);
	printf
	(
		"options:"
		"\n-i <Local IP>     IP Address on which demon will listen for TCP connections"
		"\n-p <Local Port>   Port on which demon will listen for TCP connections"
		"\n-d <i2c dev name> Device name for CY8C9560A i2c connection"
		"\n-h                Prints help"
		"\n"
		"\nDefault Values are:"
		"\n-i <%s>"
		"\n-p <%u>"
		"\n-d <%s>"
		"\n",
		programOptions.localIPAddr,
		programOptions.localPort,
		programOptions.i2cDevName
	);
	fflush(stdout);
}

void PrintProgramOptions()
{
	printf
	(
		"Program options are:"
		"\nprogramOptions.localIPAddr-----%s"
		"\nprogramOptions.localPort-------%u"
		"\nprogramOptions.i2cDevName------%s"
		"\n=============="
		"\n"
		"\n",
		programOptions.localIPAddr,
		programOptions.localPort,
		programOptions.i2cDevName
	);
	fflush(stdout);

}

int main(int argc, char *argv[])
{
	SetDefaultProgramOptions();

	///// Code for getting options starts here /////
	int optChar;

	sprintf(buildTimeInfo,"Build time is %s %s",__DATE__, __TIME__);
	printf("\n%s\n",buildTimeInfo);
	fflush(stdout);

	while((optChar = getopt(argc, argv, "i:p:d:h")) != -1)
	{
		switch(optChar)
		{
		case 'i':
			strcpy(programOptions.localIPAddr, optarg);
			break;

		case 'p':
			programOptions.localPort = (unsigned int)(atoi(optarg));
			break;

		case 'd':
			strcpy(programOptions.i2cDevName, optarg);
			break;

		case 'h':
			PrintHelp(argv[0]);
			exit(0);
		}
	}

	PrintProgramOptions();
	///// Code for getting optins ends here //////

	TCPComm_Start();

	return 0;
}

