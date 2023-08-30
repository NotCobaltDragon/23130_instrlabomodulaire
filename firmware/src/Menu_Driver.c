
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
				if(appData.position != 3){appData.position++;}
				appData.pec12Inc = false;
			}
			else if(appData.pec12Dec == true)
			{
				if(appData.position != 0){appData.position--;}
				appData.pec12Dec = false;
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
				//appData.selected = true;
				appData.pec12Pb = false;
			}



			break;
		}

	}
}