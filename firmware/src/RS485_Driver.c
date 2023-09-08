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




/*char* GetMessage(char rxBuffer[])
{
	char receptionBuffer[10];
    uint32_t test = 0;
	rs485Data.isResponseTimeoutReached = false;

	int nbByteReceived = 0;

	do
	{
		if(DRV_USART_TRANSFER_STATUS_RECEIVER_DATA_PRESENT & DRV_USART_TransferStatus(rs485Data.usartHandle))
		{
			receptionBuffer[nbByteReceived++] = DRV_USART_ReadByte(rs485Data.usartHandle);
			//nbByteReceived++;
		}
        //LED1Toggle();
        if(0) //Detection of timeout
        {
            
        }
        

	//Manage TIMEOUT

	}while(nbByteReceived < 6);
    //&&(rs485Data.isResponseTimeoutReached == false)
    return receptionBuffer;
}*/

/*uint8_t parseUSARTMessage(char rxBuffer[], char txBuffer[])
{
	E_ID_MODULES receivedId = 0;
	E_Command receivedCommand[4];
	float receivedParameter;
	uint8_t counter;
	int compareMarker;
	E_PARSE_USART returnCode;


	if(strncmp(rxBuffer, "ID", 2) == 0 && strlen(rxBuffer) >= 6)
	{
		receivedId = rxBuffer[2];

		//strncpy(rxBuffer, (rxBuffer + 3), 3);
		//receivedCommand[3] = '\0';

		//compareMarker = strncmp(receivedCommand, cmdData[correctCommand], 3);

		if(strncmp(rxBuffer[3], txBuffer[3], 3) != 0)
		{
			returnCode = CMD_WRONG;
		}
		else
		{
			returnCode = CMD_CORRECT;
		}

		//receivedParameter = receivedMessage[]

		/*for (counter = 7; counter < (strlen(receivedMessage)-1); counter++)
		{

			receivedParameter += 
		}
	}
	else
	{
		returnCode = CMD_NOT_RECEIVED;
	}

	return returnCode;
}*/

//uint8_t ExtractId(char *rxBuffer)
//{
//	uint8_t id;
//	char tempBuffer[3];
//
//	strncpy(tempBuffer, rxBuffer, 3);
//	id = atoi(tempBuffer);
//
//	return id;
//}
//
//uint8_t ExtractCommand(char *rxBuffer)
//{
//	uint8_t commandPosition;
//	for(commandPosition = 0; commandPosition < NB_CMD; commandPosition++)
//	{
//		if(strcmp(cmdData[commandPosition], &rxBuffer) == 0)
//		{
//			return commandPosition;
//		}
//	}
//	return E_CMD_ERROR;
//}
//
//uint8_t ExtractParameter(char *rxBuffer)
//{
//	//uint8_t startIndex = 6;
//	uint8_t parameter;
//	char tempChar[10];
//	tempChar[0] = rxBuffer;
//
//	//tempChar = tempChar[6];
//	parameter = tempChar[6] - '0';
//
//	/*while((rxBuffer[startIndex] >= '0') && (rxBuffer[startIndex] <= '9'))
//	{
//		
//		startIndex++;
//	}*/
//	return parameter;
//}

void MessageDataTimeoutReset()
{
	rs485Data.messageDataTimeout = 0;
	rs485Data.isResponseTimeoutReached = false;
}

void MessageDataTimeoutCallback()
{
	rs485Data.messageDataTimeout++;

	if(rs485Data.messageDataTimeout >= MESSAGE_TIMEOUT)
	{
		rs485Data.isResponseTimeoutReached = true;
	}
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


