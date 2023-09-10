//-----------------------------------------------------------
// File Name    : Menu_Driver.c		                        |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 10.09.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "Menu_Driver.h"

extern APP_DATA appData;
extern MODULE_SLOT_DATA slotData[7];
extern PEC12 pec12;
extern VOLTMETER_23132 voltmeter23132;

void Menu_Task()
{
	switch(appData.currentScreen)
	{
		case DISP_SCR_MAIN_MENU:
		{
			if(pec12.stateInc == true)
			{
				if(appData.positionCursor == (MENU_NB_DISPLAYED_ITEMS-1) && appData.positionList < (MENU_TOT_NB_ITEMS - MENU_NB_DISPLAYED_ITEMS + 1))
				{
					appData.positionList++;
				}
				else if(appData.positionCursor != 2)
				{
					appData.positionCursor++;
				}
				Pec12IncClear();
				NeedDisplayUpdate();
			}
			else if(pec12.stateDec == true)
			{
				if(appData.positionCursor == 0 && appData.positionList > 0)
				{
					appData.positionList--;
				}
				else if(appData.positionCursor != 0)
				{
					appData.positionCursor--;
				}
				Pec12DecClear();
				NeedDisplayUpdate();
			}
			else if(pec12.statePb == true)
			{
				switch(slotData[appData.positionCursor + appData.positionList].model)
				{
					case EMPTY:
						break;
					case MODULE_23132:
					{
						appData.currentScreen = DISP_SCR_23132;
						appData.moduleSelected = slotData[appData.positionCursor + appData.positionList].id;
						appData.positionCursor = 0;
						appData.positionList = 0;
						break;
					}
					default:
						break;
				}
				Pec12PbClear();
				NeedDisplayUpdate();
			}
			break;
		}
		case DISP_SCR_23132:
		{
			if(pec12.stateInc == true)
			{
				if(appData.positionCursor != (MAX_NB_BUTTON_23132 - 1))
					appData.positionCursor++;
				NeedDisplayUpdate();
				Pec12IncClear();
			}
			else if(pec12.stateDec == true)
			{
				if(appData.positionCursor != 0)
					appData.positionCursor--;
				NeedDisplayUpdate();
				Pec12DecClear();
			}
			else if(pec12.statePb == true)
			{
				switch(appData.positionCursor)
				{
					case 0:
						appData.currentScreen = DISP_SCR_MAIN_MENU;
						NeedDisplayUpdate();
						break;
					case 1:
						voltmeter23132.currentMode = !voltmeter23132.currentMode;
						NeedSendCommand(appData.moduleSelected, E_CMD_VOLTMMODE, voltmeter23132.currentMode);
						NeedDisplayUpdate();
						break;
					case 2:
						voltmeter23132.holdMode = !voltmeter23132.holdMode;
						NeedDisplayUpdate();
						break;
					//case 3:
						//break;
					default:
						break;
				}
				NeedDisplayUpdate();
				Pec12PbClear();
			}
			if(appData.needSendCommand == false && appData.expectingResponse == false && appData.periodicVoltage >= GET_VOLTAGE_FREQUENCY)
			{
				NeedSendCommand(appData.moduleSelected, E_CMD_VOLTMREAD, 0);
				appData.periodicVoltage = 0;
			}
			break;
		}

	}
}