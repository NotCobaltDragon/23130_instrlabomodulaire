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
#include "RS485_Commands.h"
#include "system_definitions.h"
#include "system_config.h"

#define DEFAULT_RS485_MODE RECEIVE

#define LAST_CMD_CHAR '~'

typedef struct
{
	DRV_HANDLE usartHandle;
}RS485_DATA;

#define SENDING false
#define RECEIVING true

typedef enum
{
	E_ID_1 = 1,
	E_ID_2,
	E_ID_3,
	E_ID_4,
	E_ID_5,
	E_ID_6,
	E_ID_7,
} E_ID;

typedef struct
{
	E_ID id;
	E_Command command;
	uint8_t parameter;
} S_TX_CARRIER;

void Init_RS485(bool defaultMode);

void SendMessage(E_ID id, E_Command command, uint8_t parameter);

void RS485_Sending_Mode(void);
void RS485_Receiving_Mode(void);


#endif