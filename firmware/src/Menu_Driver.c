#include "Menu_Driver.h"


E_Menu_State menuState = Menu_23132;

extern APP_DATA appData;
extern VOLTMETER_23132 voltmeter23132;

void Menu_Task()
{
	switch(menuState)
	{
		case Menu_Main:
			break;
		case Menu_23132:
		{
			if(appData.pec12Inc == true)
			{
				if(appData.position != 2){appData.position++;}

				//appData.pec12Inc = false;
				Pec12IncClear();
			}
			else if(appData.pec12Dec == true)
			{
				if(appData.position != 0){appData.position--;}
				//appData.pec12Dec = false;
				Pec12DecClear();
			}
			else if(appData.pec12Pb == true)
			{
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