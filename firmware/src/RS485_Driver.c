//-----------------------------------------------------------
// File Name    : RS485_Driver.c	                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------


#include "RS485_Driver.h"
#include "RS485_Commands.h"


void Init_RS485()
{
	//Init
	if(DEFAULT_RS485_MODE == SENDING)
		RS485_Sending_Mode();
	else
		RS485_Receiving_Mode();

}

void SendMessage(uint8_t id, char command, uint8_t parameter)
{
	sendString[20];



	sprintf(sendString, "ID%d", id);


}

int GetMessage()
{


	return commStatus;
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