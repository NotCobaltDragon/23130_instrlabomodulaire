#ifndef MENU_DRIVER_h
#define MENU_DRIVER_h

#include "app.h"
#include "GesPec12.h"
#include <stdint.h>                   
#include <stdbool.h>

typedef enum{Menu_Main = 0, Menu_23132}E_Menu_State;

//S_Pec12_Descriptor Pec12;

void MENU_Task(void);


#endif