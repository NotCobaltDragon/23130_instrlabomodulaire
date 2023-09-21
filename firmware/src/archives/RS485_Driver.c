//-----------------------------------------------------------
// File Name    : RS485_Driver.c	                        |
// Project Name : 2313_instrlabomodulaire                   |
// Version      : V1                                        |
// Date         : 11.09.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "RS485_Driver.h"

#define MESS_SIZE

RS485_DATA rs485Data;

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

	return isUsartOpened;
}

bool SendMessage(char txBuffer[8])
{
	bool needSendCommand = true;
	int nbByteWritten = 0;
	uint8_t bufferSize = strlen(txBuffer) + 1;


	while(nbByteWritten < bufferSize)
	{
		if(!DRV_USART_TransmitBufferIsFull(rs485Data.usartHandle))
			DRV_USART_WriteByte(rs485Data.usartHandle, txBuffer[nbByteWritten++]);
	}
	while(!(DRV_USART_TRANSFER_STATUS_TRANSMIT_EMPTY & DRV_USART_TransferStatus(rs485Data.usartHandle))){}
	needSendCommand = false;
	return needSendCommand;
}

bool GetMessage(char* rxBuffer)
{
	int nbByteReceived = 0;
	MessageDataTimeoutReset();

	do
	{
		if(DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT & DRV_USART_TransferStatus(rs485Data.usartHandle))
		{
			rxBuffer[nbByteReceived++] = DRV_USART_ReadByte(rs485Data.usartHandle);
			MessageDataTimeoutReset();
		}
	}while((nbByteReceived < 8) && (rs485Data.isResponseTimeoutReached != true));
	//if(rs485Data.isResponseTimeoutReached)
	return true;
}

void ClearReceiveBuffer()
{
	
}

void MessageDataTimeoutReset()
{
	rs485Data.messageDataTimeout = 0;
	rs485Data.isResponseTimeoutReached = false;
}

void MessageDataTimeoutCallback()
{
	rs485Data.messageDataTimeout++;

	if(rs485Data.messageDataTimeout >= MESSAGE_TIMEOUT)
		rs485Data.isResponseTimeoutReached = true;
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


