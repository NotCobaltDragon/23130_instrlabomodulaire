/*******************************************************************************
	MPLAB Harmony Application Source File
	
	Company:
		Microchip Technology Inc.
	
	File Name:
		app.c

	Summary:
		This file contains the source code for the MPLAB Harmony application.

	Description:
		This file contains the source code for the MPLAB Harmony application.  It 
		implements the logic of the application's state machine and it may call 
		API routines of other MPLAB Harmony modules in the system, such as drivers,
		system services, and middleware.  However, it does not call any of the
		system interfaces (such as the "Initialize" and "Tasks" functions) of any of
		the modules in the system or make any assumptions about when those functions
		are called.  That is the responsibility of the configuration-specific system
		files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

	Summary:
		Holds application data

	Description:
		This structure holds the application's data.

	Remarks:
		This structure should be initialized by the APP_Initialize function.
		
		Application strings and buffers are be defined outside this structure.
*/

APP_DATA appData;

MODULE_SLOT_DATA slotData[7];

VOLTMETER_23132 voltmeter23132;

extern RS485_DATA rs485Data;

char txBuffer[8];
char rxBuffer[8];
E_MODULE_ID modulePointer = MODULE_1;

//E_POWER_ON_STATE powerOnState;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************
void APP_MainTimerCallback()
{
	appData.mainTimerCount++;
	if(appData.mainTimerCount >= APP_HEARTBEAT_DELAY_100ms)
	{
		appData.mainTimerCount = 0;
		appData.mainTimerDelayHasElapsed = true;
	}
	LED2Off();
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
void UpdateDisplayValues()
{
	//updates display values
	DisplayValues_MainMenu(appData.positionList, appData.positionCursor, slotData[appData.positionList].model, slotData[appData.positionList+1].model, slotData[appData.positionList+2].model);
	DisplayValues_23132(voltmeter23132.valueVoltmeter, voltmeter23132.currentMode, voltmeter23132.holdMode, appData.positionCursor);
}

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
	Function:
		void APP_Initialize ( void )

	Remarks:
		See prototype in app.h.
 */

void APP_Initialize ( void )
{
	/* Place the App state machine in its initial state. */
	appData.state = APP_STATE_INIT;

	appData.mainTimerCount = 0;
	appData.mainTimerDelayHasElapsed = false;
	appData.currentScreen = DISP_SCR_WELCOME;
	appData.backlightColor = COL_WHITE;
	appData.positionCursor = 0;
	appData.positionList = 0;
}


/******************************************************************************
	Function:
		void APP_Tasks ( void )

	Remarks:
		See prototype in app.h.
 */

void APP_Tasks (void)
{
	if(appData.mainTimerDelayHasElapsed)
	{
		appData.mainTimerDelayHasElapsed = false;
		LED2On();
		UpdateDisplayValues();
	}

	/* Check the application's current state. */
	switch (appData.state)
	{
		/* Application's initial state. */
		case APP_STATE_INIT:
		{
			appData.errorHandler = NO_ERROR;
			DRV_TMR0_Start();
			DRV_TMR1_Start();
			DRV_TMR2_Start();
			appData.mainTimerDelayHasElapsed = false;
			DisplayInit();

			//Open USART for RS485
			appData.isUsartOpened = Init_RS485(SENDING);

			//TODO add error handling if USART didn't opened

			appData.state = APP_STATE_POWER_ON;
			
			break;
		}

		case APP_STATE_POWER_ON:
		{
			/*if(modulePointer < QTY_MODULES)
			{
				switch(powerOnState)
				{
					case SEND_IDENTIFICATION:
						NeedSendCommand(modulePointer, E_CMD_IDQUESTION, 0);
						appData.state = APP_STATE_SEND_COMMAND;
						break;
					case RECEIVE_INDENTIFICATION:
						break;
					case 
						break;
					default:
						break;
				}
				
			}*/

			//TODO: Function for reseting all modules (RST1...RST7)

			//NeedSendCommand(MODULE_1, E_CMD_IDQUESTION, 0);



			//FOR TESTING, REPLACE BY AUTOMATIC DATA COLLECTION WITH
			//IDxID? COMMAND.
			slotData[1].model = MODULE_23132;
			slotData[1].id = MODULE_2;
			//slotData[2].model = MODULE_23132;
			//slotData[2].model = EMPTY;
			//slotData[3].model = EMPTY;
			//slotData[4].model = EMPTY;
			//slotData[5].model = EMPTY;
			//slotData[6].model = EMPTY;

			VoltmeterInit();
			
			//END CODE TESTING
			appData.currentScreen = DISP_SCR_MAIN_MENU;
			appData.state = APP_STATE_DISPLAY_CHANGE;	
			break;
		}

		case APP_STATE_SERVICE_TASKS:
		{
			Display_Task();
			Menu_Task();

			if((appData.expectingResponse == true)&&(!DRV_USART_ReceiverBufferIsEmpty(rs485Data.usartHandle)))
			{
				appData.state = APP_STATE_RECEIVE_COMMAND;
			}
			else if(appData.needSendCommand == true)
			{
				appData.state = APP_STATE_SEND_COMMAND;
			}

			if(appData.needDisplayUpdate == true)
			{
				appData.state = APP_STATE_DISPLAY_CHANGE;
			}
			break;
		}

		case APP_STATE_SEND_COMMAND:
		{
			RS485_Direction_Mode(SENDING);
			sprintf(txBuffer, "ID%d%s%d", rs485Data.id, cmdData[rs485Data.command], rs485Data.parameter);
			appData.needSendCommand = SendMessage(txBuffer);
			appData.expectingResponse = true;
			RS485_Direction_Mode(RECEIVING);
			
			appData.state = APP_STATE_SERVICE_TASKS;
			break;
		}
		case APP_STATE_RECEIVE_COMMAND:
		{
			rxBuffer[0] = GetMessage(rxBuffer);

			appData.expectingResponse = false;
			appData.state = APP_STATE_SERVICE_TASKS;
			break;
		}
		case APP_STATE_DISPLAY_CHANGE:
		{
			appData.needDisplayUpdate = false;
			DisplayScreen(appData.currentScreen, false);
			UpdateDisplayValues();
			DisplaySetBacklightRGB(appData.backlightColor);
			appData.state = APP_STATE_SERVICE_TASKS;
			break;
		}
		/* The default state should never be executed. */
		default:
		{
			/* TODO: Handle error in application's state machine. */
			break;
		}
	}
}

// FUNCTION DEPRECATED, needs revision for error handling

//void DisplayErrorScreen(E_ERROR_HANDLER error)
//{
//	appData.backlightColor = COL_RED;
//	appData.currentScreen = DISP_SCR_ERROR;
//	DisplayScreen(appData.currentScreen, false);
//	DisplaySetBacklightRGB(appData.backlightColor);
//	while(1){}	//Error detected, must reset main module
//}

void VoltmeterInit()
{
	//Sets default mode of the voltmeter menu
	//should ideally be the same on modules code
	voltmeter23132.currentMode = DC_MODE;
	voltmeter23132.holdMode = false;
	voltmeter23132.valueVoltmeter = 53.29;	//Example test value
}

void NeedSendCommand(E_MODULE_ID id, E_Command command, uint8_t parameter)
{
	appData.needSendCommand = true;
	rs485Data.id = id;
	rs485Data.command = command;
	rs485Data.parameter = parameter;
}

void NeedDisplayUpdate()
{
	appData.needDisplayUpdate = true;
}

void CommandSendIntervalCallback()
{
	appData.periodicVoltage++;
}
/*******************************************************************************
 End of File
 */
