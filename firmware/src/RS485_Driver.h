//-----------------------------------------------------------
// File Name    : RS485_Driver.h	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#ifndef RS485_DRIVER_H	/* Guard against multiple inclusion */
#define RS485_DRIVER_H
#include <stdio.h>
#include <string.h>
#include "system_definitions.h"
#include "RS485_Commands.h"

#define DEFAULT_RS485_MODE RECEIVE

#define LAST_CMD_CHAR '~'

#define SENDING false
#define RECEIVING true

//typedef enum
//{
//	ID_1 = 1,
//	ID_2,
//	ID_3,
//	ID_4,
//	ID_5,
//	ID_6,
//	ID_7,
//}E_ID_MODULES;

typedef struct
{
	DRV_HANDLE usartHandle;
	bool selectedDirection;
	bool isResponseTimeoutReached;

	uint8_t id;
	E_Command command;
	uint8_t parameter;
}RS485_DATA;

bool Init_RS485(bool defaultMode);

void SendMessage(uint8_t id, E_Command command, uint8_t parameter);

void RS485_Direction_Mode(bool directionMode);

void RS485_Sending_Mode(void);

void RS485_Receiving_Mode(void);


#endif