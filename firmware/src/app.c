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
	DisplayValues_23132(voltmeter23132.id, voltmeter23132.valueVoltmeter, voltmeter23132.currentMode, voltmeter23132.holdMode, appData.positionCursor);
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
	appData.secondsCount = 0;
	appData.currentScreen = DISP_SCR_WELCOME;
	appData.backlightColor = COL_WHITE;
	appData.positionCursor = 0;
}


/******************************************************************************
	Function:
		void APP_Tasks ( void )

	Remarks:
		See prototype in app.h.
 */

uint8_t testCount = 0; //Test for counting, must be removed

void APP_Tasks (void)
{
	bool isUsartOpened = false;

	if(appData.mainTimerDelayHasElapsed)
	{
		appData.mainTimerDelayHasElapsed = false;
		LED2On();
		appData.secondsCount++;
		testCount++; //Test for counting, must be removed
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
			isUsartOpened = Init_RS485(SENDING);

			//isUsartOpened = DRV_HANDLE_INVALID; //FOR TESTING ERROR, TO DELETE

			//if(isUsartOpened == DRV_HANDLE_INVALID)
			//{
			//	appData.errorHandler = ERROR_RS485;
			//}

			//appData.errorHandler = ERROR_RS485;
			//
			//if(appData.errorHandler != NO_ERROR)
			//{
			//	DisplayErrorScreen(appData.errorHandler);
			//}
			//else
			//{
				DisplayScreen(DISP_SCR_WELCOME, false);
			//}

			appData.state = APP_STATE_POWER_ON;
			
			break;
		}

		case APP_STATE_POWER_ON:
		{
			//TODO: Function for reseting all modules (RST1...RST7)

			NeedSendCommand(MODULE_1, )

			VoltmeterInit();
			NeedDisplayUpdate();
			appData.state = APP_STATE_SERVICE_TASKS;	
			break;
		}

		case APP_STATE_SERVICE_TASKS:
		{
			Display_Task();
			Menu_Task(appData.currentScreen);

			
			
			if(appData.needSendCommand == true)
			{
				appData.state = APP_STATE_SEND_COMMAND;
			}
			else if(appData.expectingResponse == true)
			{
				appData.state = APP_STATE_RECEIVE_COMMAND;
			}

			//if(appData.secondsCount >= 15)
			//{
			//
			//	appData.state = APP_STATE_DISPLAY_CHANGE;
			//}

			if(appData.needDisplayUpdate == true)
			{
				appData.state = APP_STATE_DISPLAY_CHANGE;
			}

			if(testCount >= 20)
			{
				SendMessage(ID_1, E_CMD_VOLTMMODE, voltmeter23132.currentMode);
				testCount = 0;
			}
			break;
		}

		case APP_STATE_SEND_COMMAND:
		{
			SendMessage(rs485Data.id, rs485Data.command, rs485Data.parameter);


			appData.expectingResponse = true;
			break;
		}
		case APP_STATE_RECEIVE_COMMAND:
		{
			//GetMessage();
		}

		case APP_STATE_DISPLAY_CHANGE:
		{
			appData.needDisplayUpdate = false;
			appData.secondsCount = 0;
			appData.currentScreen = DISP_SCR_23132;
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

void DisplayErrorScreen(E_ERROR_HANDLER error)
{
	appData.secondsCount = 0;
	appData.backlightColor = COL_RED;
	appData.currentScreen = DISP_SCR_ERROR;
	DisplayScreen(appData.currentScreen, false);
	DisplaySetBacklightRGB(appData.backlightColor);
	while(1){}	//Error detected, must reset main module
}

void VoltmeterInit()
{
	voltmeter23132.id = ID_1;
	voltmeter23132.currentMode = DC_MODE;
	voltmeter23132.holdMode = false;
	voltmeter23132.valueVoltmeter = 53.29;
}

void NeedSendCommand(E_MODULES id, E_Command command, uint8_t parameter)
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

/*******************************************************************************
 End of File
 */
