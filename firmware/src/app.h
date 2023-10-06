/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
	Microchip Technology Inc.

  File Name:
	app.h

  Summary:
	This header file provides prototypes and definitions for the application.

  Description:
	This header file provides function prototypes and data type definitions for
	the application.  Some of these are required by the system (such as the
	"APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
	internally by the application (such as the "APP_STATES" definition).  Both
	are defined here for convenience.
*******************************************************************************/

//DOM-IGNORE-BEGIN
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
//DOM-IGNORE-END

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "system_config.h"
#include "system_definitions.h"

#include "Display.h"
#include "rs485_driver.h"
//#include "RS485_Commands.h"
#include "Menu_Driver.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END 

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************
/** @name Time & delays definitions in tenths of a second (*0.1 = s)... */
#define APP_HEARTBEAT_DELAY_100ms   10      ///< ...to monitor app life

#define QTY_MODULES 7

#define GET_VOLTAGE_INTERVAL 200 // unit in *10[ms]

#define DC_MODE false
#define AC_MODE true

#define MODULE_VOLTMETER "23132"

// *****************************************************************************
/* Application states

  Summary:
	Application states enumeration

  Description:
	This enumeration defines the valid application states.  These states
	determine the behavior of the application at various times.
*/

typedef enum
{
	/* Application's state machine's initial state. */
	APP_STATE_INIT=0,
	APP_STATE_POWER_ON,
	APP_STATE_MODULE_SCANNING,
	APP_STATE_SERVICE_TASKS,
	APP_STATE_DISPLAY_CHANGE,
	APP_STATE_SEND_COMMAND,
	APP_STATE_RECEIVE_COMMAND,
	APP_STATE_WAIT,

	/* TODO: Define states used by the application state machine. */

} APP_STATES;

typedef enum
{
	NO_ERROR = 0,	//NO_ERR means no error
	ERROR_RS485,	//Error with RS485
	ERROR_USB,
} E_ERROR_HANDLER;

typedef enum
{
	MODULE_1 = 1,
	MODULE_2,
	MODULE_3,
	MODULE_4,
	MODULE_5,
	MODULE_6,
	MODULE_7,
} E_MODULE_ID;

//typedef enum
//{
//
//}E_POWER_ON_STATE;

// *****************************************************************************
/* Application Data

  Summary:
	Holds application data

  Description:
	This structure holds the application's data.

  Remarks:
	Application strings and buffers are be defined outside this structure.
 */

typedef struct
{
	/* The application's current state */
	APP_STATES state;

	uint32_t mainTimerCount;
	bool mainTimerDelayHasElapsed;

	uint8_t currentScreen;
	uint32_t backlightColor;

	bool needDisplayUpdate;
	bool needSendCommand;
	bool expectingResponse;

	uint32_t periodicVoltage;

	uint8_t positionCursor;
	uint8_t positionList;

	bool isUsartOpened;

	E_MODULE_ID moduleSelected;

	E_ERROR_HANDLER errorHandler;

} APP_DATA;

typedef enum
{
	EMPTY = 0,
	MODULE_23132,
}E_MODULE_MODEL;

typedef struct
{
	E_MODULE_ID id;
	E_MODULE_MODEL model;
} MODULE_SLOT_DATA;

typedef struct
{
	float valueVoltmeter;
	char valueVoltmeterString[7];
	bool currentMode;
	bool holdMode;
} VOLTMETER_23132;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/
void APP_MainTimerCallback();
	
// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
	void APP_Initialize ( void )

  Summary:
	 MPLAB Harmony application initialization routine.

  Description:
	This function initializes the Harmony application.  It places the 
	application in its initial state and prepares it to run so that its 
	APP_Tasks function can be called.

  Precondition:
	All other system initialization routines should be called before calling
	this routine (in "SYS_Initialize").

  Parameters:
	None.

  Returns:
	None.

  Example:
	<code>
	APP_Initialize();
	</code>

  Remarks:
	This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
	void APP_Tasks ( void )

  Summary:
	MPLAB Harmony Demo application tasks function

  Description:
	This routine is the Harmony Demo application's tasks function.  It
	defines the application's state machine and core logic.

  Precondition:
	The system and application initialization ("SYS_Initialize") should be
	called before calling this.

  Parameters:
	None.

  Returns:
	None.

  Example:
	<code>
	APP_Tasks();
	</code>

  Remarks:
	This routine must be called from SYS_Tasks() routine.
 */

void APP_Tasks( void );

void VoltmeterInit(void);

//void DisplayErrorScreen(E_ERROR_HANDLER error);

void NeedDisplayUpdate(void);

void NeedSendCommand(uint8_t id, const char* command, const char* parameter);

void ResetExternalModules(void);

#endif /* _APP_H */

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

/*******************************************************************************
 End of File
 */

