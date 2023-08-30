/*************************************************************************** */
/** 
 * @file:   Display.c
 * #        DISPLAY.c 
 * ## Module de gestion de l'affichage du TPA
 * 
 * @author  J. Jose Moreno
 * @date    16.04.2020
 * @version 1.0
 * 
 * ## Description
 * Fournit les fonctions necessaires pour afficher les donnees du TPA/VPA sur
 * l'ecran LCD du TPA. Display_Task effectue une mise a jour reguliere de 
 * l'affichage complet a partir d'une copie complete locale. Elle utilise l'API
 * OpenSource ugui.h. pour dessiner des formes et du texte sur l'ecran graphique.
 * 
 * @author  J. Jose Moreno
 * @date    11.08.2023
 * @version 2.0
 * 
 * ## Description
 * Generalisation pour utilisation dans projets ETML-ES.
 * Remplacement de D... par D...
 */
/* ************************************************************************** */

// *****************************************************************************
// Section: Included Files
// *****************************************************************************
#include "Display.h"

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: File Scope or Global Data                                  */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
static T_DISPLAY disp;
///@}

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Task Callback Routines                                     */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* -------------------------------------------------------------------------- */
/**
 @brief     Comptage des temps pour Display. Cette routine doit etre appelee
 *          par l'ISR d'un timer a une cadence de 10 ms.
 * 
 */
void Display_TimerCallback(void){
	disp.refreshDelayCount++;
	if (disp.refreshDelayCount >= DISPLAY_REFRESH_PERIOD){
		disp.refreshTickEvent = true;
		disp.refreshDelayCount = 0;
	}
	LED2Off();
}
/* -------------------------------------------------------------------------- */
/**
 * @brief   Fonction de Callback appelee par la lib ugui pour generer un pixel 
 *          pour l'affiche cible.      
 * @param   x   axe des x sur le LCD. La valeur est utilisable directement.
 * @param   y   axe des y sur le LCD, il est segmente en 'pages' de 8 bits (bytes).
 *              La position du bit est calculee pour la bonne page.
 * @param   c   couleur (noir ou blanc du pixel
 */
void DisplayPixelSetCallback(UG_S16 x, UG_S16 y, UG_COLOR c){
	UG_S16 column = x;
	UG_S16 row = DISPLAY_HEIGHT - y - 1;
	/* toute autre couleur que noir sera consideree comme blanche */
	uint8_t page = row / DISPLAY_PAGES; 
	uint8_t pageValue = 1 << (row % DISPLAY_PAGES);
	if (c == C_BLACK){
		/* noir pour afficher un pixel, val = val OR pageValue */
		disp.screenCopy[page][column] |= pageValue;
	} else {
		/* blanc pour effacer un pixel, val = val AND /pageValue */
		disp.screenCopy[page][column] &= ~pageValue;
	}
 }  /* DisplayPixelSetCallback */

///@}
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Local Functions                                            */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* -------------------------------------------------------------------------- */
/**
 * @brief   Efface la page LCD en fonce ou clair selon le mode utilise
 * 
 * @param   p           page to be cleared
 * @param   setToClear  false = dark ; true = clear
 * 
 * Texec = 245 us
 */
static void DisplayClearPage(uint8_t p, bool setToClear) {
	uint8_t c, color;

	if (setToClear) { color = 0; }
	else { color = 0xFF; }
	
	for (c = 0; c < DISPLAY_WIDTH; c++) {
		disp.screenCopy[p][c] = color;
	}
}   /* DisplayClearPage */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Displays the welcome screen
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
static void DisplayScreen_Welcome(bool setToDark){
	char str_Welcome_1[] = "2313";
	char str_Welcome_2[] = "v1.0.1";
	char str_Welcome_3[] = "Steffen Alex";
	
	if (setToDark){
		UG_SetBackcolor ( C_WHITE ) ;
		UG_SetForecolor ( C_BLACK ) ;
	}
	else { 
		UG_SetBackcolor ( C_BLACK ) ;
		UG_SetForecolor ( C_WHITE ) ;
	}
	
	UG_FontSetHSpace(0);
	
	/* Ecriture des chaines de caracteres */
	UG_FontSelect ( &FONT_12X16 );
	UG_PutString(0 ,0 , str_Welcome_1);
	UG_FontSelect ( &FONT_8X8 );
	UG_PutString(30 ,30 , str_Welcome_2);
	UG_PutString(4 ,50 , str_Welcome_3);
	
}   /* DisplayScreen_Welcome */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Displays Screen Nr n
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
/*static*/ void DisplayScreen_23132(bool setToDark)
{
	if (setToDark){
		UG_SetBackcolor (C_WHITE);
		UG_SetForecolor (C_BLACK);
	} else { 
		UG_SetBackcolor (C_BLACK);
		UG_SetForecolor (C_WHITE);
	}
	UG_FontSetHSpace(0);
	UG_FontSelect (&FONT_6X8);
	UG_PutString(1, 2, "Module 2");
	UG_DrawFrame(0, 0, 127, 63, C_BLACK);
	UG_DrawLine(0, 10, 127, 10, C_BLACK);
	UG_DrawLine(0, 53, 127, 53, C_BLACK);
	UG_DrawLine(32, 63, 32, 53, C_BLACK);
	UG_DrawLine(62, 63, 62, 53, C_BLACK);
	UG_DrawLine(77, 63, 77, 53, C_BLACK);
}  /* DisplayScreen_23132 */

void DrawMenuIcon(bool selected)
{
	int foreColor = C_BLACK;
	int backColor = C_WHITE;

	if(selected)
	{
		foreColor = C_WHITE;
		backColor = C_BLACK;
	}
	UG_FillFrame(1, 54, 31, 62, backColor);
	UG_DrawPixel(2, 56, foreColor);
	UG_DrawPixel(2, 58, foreColor);
	UG_DrawPixel(2, 60, foreColor);
	UG_DrawLine(4, 56, 6, 56, foreColor);
	UG_DrawLine(4, 58, 6, 58, foreColor);
	UG_DrawLine(4, 60, 6, 60, foreColor);
	UG_SetBackcolor (backColor);
	UG_SetForecolor (foreColor);
	UG_FontSelect (&FONT_6X8);
	UG_PutString(8, 55, "Menu");
}

void DrawCurrentMode(bool currentMode, bool selected)
{
	int foreColor = C_BLACK;
	int backColor = C_WHITE;

	if(selected)
	{
		foreColor = C_WHITE;
		backColor = C_BLACK;
	}
	UG_FillFrame(33, 54, 61, 62, backColor);

	UG_SetBackcolor (backColor);
	UG_SetForecolor (foreColor);
	UG_FontSelect (&FONT_6X8);
	if(currentMode == AC_MODE)
	{
		UG_PutString(48, 55, "AC");
		UG_DrawLine(35, 58, 37, 55, foreColor);
		UG_DrawLine(38, 55, 42, 61, foreColor);
		UG_DrawLine(43, 61, 45, 58, foreColor);
	}
	else
	{
		UG_PutString(48, 55, "DC");
		UG_DrawLine(35, 56, 45, 56, foreColor);
	}		
}

void DrawHoldMode(bool holdMode, bool selected)
{
	int foreColor = C_BLACK;
	int backColor = C_WHITE;

	if(selected)
	{
		foreColor = C_WHITE;
		backColor = C_BLACK;
	}
	UG_FillFrame(63, 54, 76, 62, backColor);
	if(holdMode == true)
	{
		UG_DrawFrame(67, 58, 72, 61, foreColor);
		UG_DrawLine(70, 55, 71, 57, foreColor);
		UG_DrawLine(69, 55, 68, 57, foreColor);
	}
	else
	{
		UG_DrawFrame(68, 58, 73, 61, foreColor);
		UG_DrawLine(68, 55, 69, 57, foreColor);
		UG_DrawLine(67, 55, 66, 57, foreColor);
	}
	
	/*UG_SetBackcolor (backColor);
	UG_SetForecolor (foreColor);
	UG_FontSelect (&FONT_6X8);
	UG_PutString(66, 55, "HOLD");*/

}

///@}

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Interface Functions                                        */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* ----------------------------------------------------------------------------*/
/**
 * @brief   Display specific data according screen in use
 *          -- EXEMPLE DE FONCTION A RENOMMER/ADAPTER --
 * 
 * @param  a    valeur a 
 */
//void DisplayValues_23132(uint8_t position, float voltmeterValue, bool currentMode, bool hold)
void DisplayValues_23132(float voltmeterValue, bool currentMode, bool holdMode, uint8_t position)
{
	char str[7] = "test";

	if(disp.currentScreenNr == DISP_SCR_23132)
	{
		if(holdMode == false)
		{
			UG_FontSelect(&FONT_16X26);
			UG_SetBackcolor (C_WHITE);
			UG_SetForecolor (C_BLACK);
			//sprintf(str, "%d", position);
			sprintf(str, "%2.2f", voltmeterValue);
			UG_PutString(3, 17, str);
		}
		DrawMenuIcon(false);
		DrawCurrentMode(currentMode, false);
		DrawHoldMode(holdMode, false);
		switch(position)
		{
			case 0:
				DrawMenuIcon(true);
				break;
			case 1:
				DrawCurrentMode(currentMode, true);
				break;
			case 2:
				DrawHoldMode(holdMode, true);
				break;
			case 3:
				break;
		}
	}
}   /* DisplayValues_23132 */




/* -------------------------------------------------------------------------- */
/**
 * @brief   Sets the background color of the display
 * 
 * @param   rgb     standard RGB value. Ex: C_AQUA = 0x00FFFF
 * 
 */
void DisplaySetBacklightRGB(uint32_t rgb){
	uint8_t col [3];
	memcpy(col, &rgb, 3);
	DRV_OC1_PulseWidthSet(col[2] * DISP_BLRGB_PWM_FACTOR);
	DRV_OC2_PulseWidthSet(col[1] * DISP_BLRGB_PWM_FACTOR);
	DRV_OC3_PulseWidthSet(col[0] * DISP_BLRGB_PWM_FACTOR);
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Initiate a scroll clear of the display
 * 
 */
void DisplayScrollClear(){
	disp.command = DISP_CMD_SCROLLDOWN_CLEAR;
	disp.scrollLine = 0;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Clear Display data
 *  
 * @param setToClear    false = dark ; true = clear 
 */
void DisplayClear(bool setToClear){
	uint8_t p,c, color;
	
	if (setToClear){ color = 0; }
	else { color = 0xFF; }
	
	for (p = 0; p < DISPLAY_PAGES; p++) {
		for (c = 0; c < DISPLAY_WIDTH; c++) {
			disp.screenCopy[p][c] = color;
		}
	}
}   /* DisplayClear */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Display Screen basics
 * 
 * @param   screen      image # to be displayed
 * @param   setToDark   true = dark ; false = clear
 * 
 */
void DisplayScreen(uint8_t screen, bool setToDark){
	
	disp.currentScreenNr = screen;
	DisplayClear(true);

	switch (screen)
	{
		case DISP_SCR_WELCOME:
			DisplayScreen_Welcome(true);            
			break;
		case DISP_SCR_23132:
			DisplayScreen_23132(true);
			break;
		default:
			break;   
	}
}   /* DisplayScreen */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Initialize communication to the LCD display, graphical user 
 *          interface (GUI) and data 
 * @return  false if init failed
 */
bool DisplayInit(){
	
	/* Allumage backlight, demarrage des OC/PWM  */
	DisplaySetBacklightRGB(COL_DEFAULT);
	DRV_OC1_Start();    
	DRV_OC2_Start();    
	DRV_OC3_Start();   
	
	/* initalisation de la GUI, structure global et fonction pixel set callback */
	UG_Init(&disp.gui, DisplayPixelSetCallback, DISPLAY_WIDTH, DISPLAY_HEIGHT); 
	
	/* init du LCD */
	bool isOk = LCD_Init();
	
	/* effacer le contenu de l'ecran en local (sera rafraichi periodiquement) */
	DisplayClear(true);
	disp.refreshTickEvent = false;
	disp.refreshDelayCount = 0;
	disp.currentScreenNr = 0;
	disp.graphTime = 0;
	
	/* init machine d'etat */
	disp.state = DISP_STATE_WAIT_REFRESH;
	disp.command = DISP_CMD_NOCMD;
	
	return isOk;
	
}   /* DisplayInit */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Task updates/refreshes the display, must be called periodically by the client
 * 
 */
void Display_Task(){
	if (LCD_Task_IsReady()) 
	{  
		switch (disp.state) 
		{
			case DISP_STATE_WAIT_REFRESH:
			{
				if (disp.refreshTickEvent) 
				{
					disp.refreshTickEvent = false;
					
					/* traitement en fonction de la commande en cours */
					switch (disp.command) 
					{                                                 
						case DISP_CMD_NOCMD: 
						{
							disp.page = 0;
							break;  
						}
						case DISP_CMD_SCROLLDOWN_CLEAR:
						{
							LCD_ScrollDown(DISPLAY_SCROLL_STEP);
							DisplayClearPage(disp.scrollLine/DISPLAY_PAGE_HEIGHT, true);
							disp.scrollLine+=DISPLAY_SCROLL_STEP;
							if (disp.scrollLine>=DISPLAY_HEIGHT)
							{
								disp.command = DISP_CMD_NOCMD;                               
							}
							break;  
						}
						default: {
							break;  
						}
					}
					disp.state = DISP_STATE_REFRESHING;
				}
				break;
			}
			
			case DISP_STATE_REFRESHING: {       // Total Texec = 7 ms
				/* mise a jour de la page courante -> enverra une trame au LCD */
				LCD_DataWrite(&disp.screenCopy[disp.page][0], DISPLAY_WIDTH, disp.page, 0);
				disp.page++;
				if (disp.page >= DISPLAY_PAGES) {
					disp.page = 0;
					disp.state = DISP_STATE_WAIT_REFRESH;
				}
				break;
			}
						
			default:
			{
				disp.state = DISP_STATE_WAIT_REFRESH;
				break;
			}
		}
	}
}   /* Display_Task */
///@}

/* *****************************************************************************
 End of File Display
 */
