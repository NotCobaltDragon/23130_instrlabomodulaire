//-----------------------------------------------------------
// File Name    : RS485_Commands.c	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 01.09.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "RS485_Commands.h"

const char cmdData[4][4] = 
{
	"ERR",	//DO NOT USE
	"ID?",	//E_CMD_IDQUESTION
	"VMG",	//E_CMD_VOLTMGAIN
	"VMM",	//E_CMD_VOLTMMODE
};

const char nameModulesData[2][12] =	//11 is the maximum characters on screen + 1
{
	"Not mounted",
	"Voltmeter",
};