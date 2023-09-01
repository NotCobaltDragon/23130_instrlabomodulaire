//-----------------------------------------------------------
// File Name    : Menu_Driver.h		                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#ifndef MENU_DRIVER_H	/* Guard against multiple inclusion */
#define MENU_DRIVER_H

#include "app.h"
#include "Encoder_Driver.h"
#include <stdint.h>                   
#include <stdbool.h>

#define MAX_BUTTON 4

typedef enum{Menu_Main = 0, Menu_23132}E_Menu_State;

void Menu_Task(void);


#endif