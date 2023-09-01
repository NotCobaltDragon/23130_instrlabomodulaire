//-----------------------------------------------------------
// File Name    : RS485_Driver.c	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "RS485_Driver.h"
//#include "system_config.h"
//#include "system_definitions.h"

#define MESS_SIZE

RS485_DATA rs485Data;

S_TX_CARRIER txCarrier;

//const char cmdStrings[][4];

bool Init_RS485(bool defaultMode)
{
	bool isUsartOpened;

	RS485_Direction_Mode(defaultMode);
	rs485Data.selectedDirection = defaultMode;

	rs485Data.usartHandle = DRV_USART_Open(DRV_USART_INDEX_0, DRV_IO_INTENT_NONBLOCKING);
	if(rs485Data.usartHandle == DRV_HANDLE_INVALID)
	{
		isUsartOpened = false;
	}
	else
	{
		isUsartOpened = true;
	}

	return rs485Data.usartHandle;
}

void SendMessage(E_ID_MODULES id, E_Command command, uint8_t parameter)
{
	char sendString[10];
	char cmdString[4];
	int nbByteWritten;
	uint8_t sizeString = 0;
	uint8_t bufferSize = strlen(sendString);

	sprintf(sendString, "ID%d%s%d", id, cmdData[command], parameter);

	bufferSize = strlen(sendString);

	while(sizeString < bufferSize)
	{
		if(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_FULL & DRV_USART_TransferStatus(rs485Data.usartHandle)) )
		{
			DRV_USART_WriteByte(rs485Data.usartHandle, sendString[sizeString++]);
		}
	}
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

void RS485_Direction_Mode(bool directionMode)
{
	if(directionMode == SENDING)
	{
		RS485_Sending_Mode();
	}
	else if(directionMode == RECEIVING)
	{
		RS485_Receiving_Mode();
	}
}


