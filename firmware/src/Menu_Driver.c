//-----------------------------------------------------------
// File Name    : Menu_Driver.c		                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "Menu_Driver.h"


//TEMPORARY FOR DEBUG
#include "RS485_Driver.h"


//E_Menu_State menuState = Menu_23132;

extern APP_DATA appData;
extern PEC12 pec12;
extern VOLTMETER_23132 voltmeter23132;

extern RS485_DATA rs485Data;


void Menu_Task(E_DISPLAY_SCREENS menuState)
{
	switch(menuState)
	{
		case DISP_SCR_MAIN_MENU:
			break;
		case DISP_SCR_23132:
		{
			if(pec12.stateInc == true)
			{
				NeedDisplayUpdate();
				if(appData.positionCursor != (MAX_BUTTON - 1))
					appData.positionCursor++;
				Pec12IncClear();
			}
			else if(pec12.stateDec == true)
			{
				NeedDisplayUpdate();
				if(appData.positionCursor != 0)
					appData.positionCursor--;
				Pec12DecClear();
			}
			else if(pec12.statePb == true)
			{
				NeedDisplayUpdate();
				switch(appData.positionCursor)
				{
					case 0:
						//go to main menu
						break;
					case 1:
						voltmeter23132.currentMode = !voltmeter23132.currentMode;
						NeedDisplayUpdate();
						break;
					case 2:
						voltmeter23132.holdMode = !voltmeter23132.holdMode;
						NeedDisplayUpdate();
						break;
					case 3:
						rs485Data.selectedDirection = !rs485Data.selectedDirection;
						RS485_Direction_Mode(rs485Data.selectedDirection);
						break;
					default:
						break;
				}
				Pec12PbClear();
			}
			break;
		}

	}
}