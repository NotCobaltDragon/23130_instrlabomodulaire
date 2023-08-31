//-----------------------------------------------------------
// File Name    : RS485_Driver.c	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "RS485_Driver.h"
#include "system_config.h"
#include "system_definitions.h"

#define MESS_SIZE

RS485_DATA rs485Data;

S_TX_CARRIER txCarrier;

//const char *cmdStrings[] = {CMD_STRINGS};

void Init_RS485(bool defaultMode)
{
	//Init
	rs485Data.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_NONBLOCKING);

	if(defaultMode == SENDING)
		RS485_Sending_Mode();
	else
		RS485_Receiving_Mode();
}

void SendMessage(E_ID id, E_Command command, uint8_t parameter)
{
	char sendString[20];
	uint8_t nbByteWritten;
	uint8_t sizeString = 0;
	uint8_t bufferSize;

	sprintf(sendString, "ID%d%s", id, cmdStrings[command]);
	bufferSize = strlen(sendString);


	do
	{
		nbByteWritten = DRV_USART_Write(rs485Data.usartHandle, &sendString[sizeString], bufferSize - sizeString);
		sizeString += nbByteWritten;
	}while(sizeString < bufferSize);
}

int GetMessage()
{
	int commStatus = 0;


	return commStatus;
}



void ParityCheckHandler()
{

}

void ResponseTimoutHandler()
{

}


void RS485_Sending_Mode()
{
	RS485_RE_On();
	RS485_DE_On();
}

void RS485_Receiving_Mode()
{
	RS485_RE_Off();
	RS485_DE_Off();
}


