//-----------------------------------------------------------
// File Name    : RS485_Commands.h	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 01.09.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#ifndef RS485_COMMANDS_H	/* Guard against multiple inclusion */
#define RS485_COMMANDS_H

#include <stdio.h>
#include <string.h>

//Voltmeter 23132 specific commands
typedef enum
{
	E_CMD_IDQUESTION = 1,
	E_CMD_VOLTMGAIN,
	E_CMD_VOLTMMODE,
}E_Command;

//extern const char *cStrings[];

//#define CMD_STRINGS	\
//	"ERR" \
//	"ID?" \	//E_CMD_IDQUESTION
//	"VMG" \	//E_CMD_VOLTMGAIN
//	"VMM"	//E_CMD_VOLTMMODE

extern const char *cmdStrings[][3];

#endif