//-----------------------------------------------------------
// File Name    : Menu_Driver.h		                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 10.09.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#ifndef MENU_DRIVER_H	/* Guard against multiple inclusion */
#define MENU_DRIVER_H

#include "app.h"
#include "Display.h"
#include "Encoder_Driver.h"
#include <stdint.h>                   
#include <stdbool.h>

#define MENU_NB_DISPLAYED_ITEMS 3
#define MENU_TOT_NB_ITEMS 7

#define MAX_NB_BUTTON_23132 3


typedef enum
{
	Menu_Main = 0, 
	Menu_23132,
}E_Menu_State;

void Menu_Task(void);

#endif