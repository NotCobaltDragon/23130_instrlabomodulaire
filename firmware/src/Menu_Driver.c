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


E_Menu_State menuState = Menu_23132;

extern APP_DATA appData;
extern PEC12 pec12;
extern VOLTMETER_23132 voltmeter23132;

extern RS485_DATA rs485Data;


void Menu_Task()
{
	switch(menuState)
	{
		case Menu_Main:
			break;
		case Menu_23132:
		{
			if(pec12.stateInc == true)
			{
				NeedDisplayUpdate();
				if(appData.position != (MAX_BUTTON - 1))
					appData.position++;
				Pec12IncClear();
			}
			else if(pec12.stateDec == true)
			{
				NeedDisplayUpdate();
				if(appData.position != 0)
					appData.position--;
				Pec12DecClear();
			}
			else if(pec12.statePb == true)
			{
				NeedDisplayUpdate();
				switch(appData.position)
				{
					case 0:
						//go to main menu
						break;
					case 1:
						voltmeter23132.currentMode = !voltmeter23132.currentMode;
						break;
					case 2:
						voltmeter23132.holdMode = !voltmeter23132.holdMode;
						break;
					case 3:
						rs485Data.selectedDirection = !rs485Data.selectedDirection;
						RS485_Direction_Mode(rs485Data.selectedDirection);
						break;
					default:
						break;
				}
				//appData.pec12Pb = false;
				Pec12PbClear();
			}
			break;
		}

	}
}