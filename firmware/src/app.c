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

void APP_Uart1Callback()
{

}

void APP_Uart2Callback()
{

}

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************
void UpdateDisplayValues()
{
	//updates display values
	DisplayValues_23132(voltmeter23132.valueVoltmeter, voltmeter23132.currentMode, voltmeter23132.holdMode, appData.position);
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
	appData.position = 0;
}


/******************************************************************************
	Function:
		void APP_Tasks ( void )

	Remarks:
		See prototype in app.h.
 */

void APP_Tasks ( void )
{
	if(appData.mainTimerDelayHasElapsed)
	{
		appData.mainTimerDelayHasElapsed = false;
		LED2On();
		appData.secondsCount++;
		UpdateDisplayValues();
	}

	/* Check the application's current state. */
	switch ( appData.state )
	{
		/* Application's initial state. */
		case APP_STATE_INIT:
		{
			//Pec12Init();
			DRV_TMR0_Start();
			DRV_TMR1_Start();
			DRV_TMR2_Start();
			appData.mainTimerDelayHasElapsed = false;			
			
			DisplayInit();
			DisplayScreen(DISP_SCR_WELCOME, false);
			
			appData.state = APP_STATE_POWER_ON;
			
			break;
		}

		case APP_STATE_POWER_ON:
		{
			//TODO: Function for reseting all modules (RST1...RST7)
			VoltmeterInit();
			appData.state = APP_STATE_SERVICE_TASKS;	
			break;
		}

		case APP_STATE_SERVICE_TASKS:
		{
			
			Display_Task();
			Menu_Task();

			if(voltmeter23132.valueVoltmeter < 200)
			{
				voltmeter23132.valueVoltmeter += 0.1;
			}
			else
			{
				voltmeter23132.valueVoltmeter = 0.72;
			}

			if(appData.secondsCount >= 15)
			{
				//send RS485
				appData.state = APP_STATE_DISPLAY_CHANGE;
			}
			break;
		}

		case APP_STATE_DISPLAY_CHANGE:
		{
			appData.secondsCount = 0;
			appData.currentScreen = DISP_SCR_23132;
			DisplayScreen(appData.currentScreen, false);
			UpdateDisplayValues();
			//appData.backlightColor = COL_WHITE;
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

void VoltmeterInit()
{
	voltmeter23132.currentMode = DC_MODE;
	voltmeter23132.holdMode = false;
	voltmeter23132.valueVoltmeter = 0.72;
}

/*******************************************************************************
 End of File
 */
