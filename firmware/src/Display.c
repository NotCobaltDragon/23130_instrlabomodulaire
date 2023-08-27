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
    LED1Toggle();
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
    char str_Welcome_1[] = "Nom Projet";
    char str_Welcome_2[] = "v1.0.0";
    char str_Welcome_3[] = "Auteur";
    
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
 * @brief   Displays the data for Screen Versions
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
static void DisplayScreen_Nr_1(bool setToDark){
    char str_1[] = "ECRAN Nr 1";
    char str_2[] = "Chaine a";
    char str_3[] = "Chaine b";

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
    UG_FontSelect ( &FONT_8X12 );
    UG_PutString(20 ,0 , str_1);
    UG_FontSelect ( &FONT_8X8 );
    UG_PutString(0 ,15 , str_2);
    UG_PutString(0 ,37 , str_3);
    
}   /* DisplayScreen_Nr_1 */


/* -------------------------------------------------------------------------- */
/**
 * @brief   Displays Screen Nr n
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
static void DisplayScreen_Nr_2(bool setToDark){
    if (setToDark){
        UG_SetBackcolor ( C_WHITE ) ;
        UG_SetForecolor ( C_BLACK ) ;
    } else { 
        UG_SetBackcolor ( C_BLACK ) ;
        UG_SetForecolor ( C_WHITE ) ;
    }
    UG_FontSetHSpace(0);
   
    /* Ecriture des chaines de caracteres */
    UG_FontSelect ( &FONT_6X8 );
    UG_PutString(0, 0,  "ECRAN No 2");
    
    UG_PutString(0, 12, "Item1");
    UG_PutString(0, 23, "Item2");
    UG_PutString(0, 34, "Item3");
    UG_PutString(0, 50, "Item4");
    UG_PutString(36, 46, "Itm5");
    UG_PutString(36, 56, "Item6");
        
    UG_DrawLine(0, 9, 127, 9, C_BLACK);
    UG_DrawLine(0, 43, 127, 43, C_BLACK);
    UG_DrawLine(33, 0, 33, 43, C_BLACK);
    UG_DrawLine(81, 0, 81, 63, C_BLACK);

}   /* DisplayScreen_Nr_2 */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Displays Screen Nr n
 * 
 * @param   setToDark   true = dark ; false = clear
 * 
 */
static void DisplayScreen_Nr_3(bool setToDark){
    if (setToDark){
        UG_SetBackcolor ( C_WHITE ) ;
        UG_SetForecolor ( C_BLACK ) ;
    } else { 
        UG_SetBackcolor ( C_BLACK ) ;
        UG_SetForecolor ( C_WHITE ) ;
    }
    UG_FontSetHSpace(0);
    
    /* Ecriture des chaines de caracteres */
    UG_FontSelect ( &FONT_6X8 );
    UG_PutString(0, 0, "SCREEN NR 3");
    
}   /* DisplayScreen_Nr_3 */


/* -------------------------------------------------------------------------- */
/**
 * @brief   Draws a lead acid battery
 * 
 * @param   x, y    Top left position of the drawing
 * 
 */
static void DrawBAT(uint8_t x, uint8_t y){
    UG_DrawFrame(x+3, y+2, x+14, y+8, C_BLACK);
    UG_DrawFrame(x+4, y, x+6, y+2, C_BLACK);
    UG_DrawFrame(x+11, y, x+13, y+2, C_BLACK);
    UG_DrawLine(x+1, y, x+1, y+2, C_BLACK);         // Signe +
    UG_DrawLine(x, y+1, x+2, y+1, C_BLACK);
    UG_DrawPixel(x+15, y+1, C_BLACK);               // signe -
    UG_DrawPixel(x+16, y+1, C_BLACK);  
    
}   /* DrawVBAT */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Draws a battery
 * 
 * @param   x, y    Top left position of the drawing
 * 
 */
static void DrawCellBAT(uint8_t x, uint8_t y){
    UG_DrawFrame(x, y, x+13, y+5, C_BLACK);
    UG_DrawPixel(x+14, y+2, C_BLACK);
    UG_DrawPixel(x+14, y+3, C_BLACK);
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Display specific data according screen in use
 * 
 * @param   level       niveau 0..5, a afficher en barres
 * @param   x, y        coordonnees de position gauche en bas
 * 
 */
static void DrawLargeRSSI(uint8_t level, uint8_t x, uint8_t y){
    /* effacer la zone */
    UG_FillFrame(x,  y, x+RSSI_BARS_NR*4, y+1-RSSI_BARS_NR*2, C_WHITE);
    /* dessiner les barres */
    uint8_t i = 0;
    while ((i < level) & (i < RSSI_BARS_NR)) {
        UG_FillFrame(x+i*4, y, x+i*4+2, y-i*2-1, C_BLACK);
        i++;
    }
}   /* DrawLargeRSSI */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Display specific data according screen in use
 * 
 * @param   level       niveau 0..5, a afficher en barres
 * @param   x, y        coordonnees de position gauche en bas
 * 
 */
static void DrawSmallRSSI(uint8_t level, uint8_t x, uint8_t y){
    /* effacer la zone */
    UG_FillFrame(x,  y, x+RSSI_BARS_NR*2, y-RSSI_BARS_NR, C_WHITE);
    /* dessiner les barres */
    uint8_t i = 0;
    while ((i < level) & (i < RSSI_BARS_NR)) {
        UG_DrawLine(x+i*2, y, x+i*2, y-i-1, C_BLACK);
        i++;
    }
}   /* DrawSmallRSSI */

/* -------------------------------------------------------------------------- */
/**
 * @brief   Draws a heat symbol
 * 
 * @param   baseX   Top left position of the drawing X
 * @param   baseY   Top left position of the drawing Y
 * 
 */
static void DrawHeat(uint8_t baseX, uint8_t baseY){
    uint8_t x = baseX;
    uint8_t y = baseY;
    uint8_t offsetX = 0;
    
    UG_DrawFrame(x, y, x+10, y+11, C_WHITE);
    for (offsetX = 0; offsetX < 3; offsetX++) {
        x = offsetX * 4 + baseX;
        UG_DrawLine(x + 0, y + 0, x + 0, y + 1, C_BLACK);
        UG_DrawLine(x + 1, y + 2, x + 1, y + 3, C_BLACK);
        UG_DrawLine(x + 2, y + 4, x + 2, y + 5, C_BLACK);
        UG_DrawLine(x + 1, y + 6, x + 1, y + 7, C_BLACK);
        UG_DrawLine(x + 0, y + 8, x + 0, y + 9, C_BLACK);
        UG_DrawLine(x + 1, y + 10, x + 1, y + 11, C_BLACK);
    }
}

///@}

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Interface Functions                                        */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
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
 * @brief   Display specific data according screen in use
 *          -- EXEMPLE DE FONCTION A RENOMMER/ADAPTER --
 * 
 * @param   x, y, z     valeurs a afficher
 */
void DisplayValues_Ex_1(double x, double y, double z){
    char str[32] = "";
    double a = x;
    double b = y;
    double c = z;
    switch (disp.currentScreenNr) {
        case DISP_SCR_NR_1:
            UG_FontSelect(&FONT_6X8);
            sprintf(str, "%+04.0f %+03.1f %+02.2f", a, b, c);
            UG_PutString(36, 25, str);
            break;
        default:
            break;
    }    
}   /* DisplayValues_Ex_1 */

/* ----------------------------------------------------------------------------*/
/**
 * @brief   Display specific data according screen in use
 *          -- EXEMPLE DE FONCTION A RENOMMER/ADAPTER --
 * 
 * @param  a    valeur a 
 * @param  b    valeur b
 * @param  c    valeur c 
 */
void DisplayValues_Ex_2(double a, double b, double c){
    char str[20] = "";

    switch (disp.currentScreenNr) {
        case DISP_SCR_NR_2:
            UG_FontSelect(&FONT_6X8);
            sprintf(str, "z:%+06.1f h:%05.1f", a, b);
            UG_PutString(30, 28, str);
            sprintf(str, "G:%05.1f", c);
            UG_PutString(84, 54, str);           
            break;
        default:
            break;
    }    
}   /* DisplayValues_Ex_2 */

/* ----------------------------------------------------------------------------*/
/**
* @brief   Display specific data according screen in use**
*          -- EXEMPLE DE FONCTION A RENOMMER/ADAPTER --
 * 
 * @param   x, y, z, h    valeurs a afficher
 */
void DisplayValues_Ex_3(int16_t x, int16_t y, int16_t z, int16_t h){
    char str[16] = "";

    switch (disp.currentScreenNr) {
        case DISP_SCR_NR_3:
            UG_FontSelect(&FONT_6X8);
            sprintf(str, "%+04d %+04d %+04d", x, y, z);
            UG_PutString(30, 41, str);
            sprintf(str, "M:%03d", h);
            UG_PutString(36, 54, str);
    
            break;
        default:
            break;
    }    
}   /* DisplayValues_Ex_3 */


/* -------------------------------------------------------------------------- */
/**
 * @brief   Display specific data according screen in use
 * 
 * @param   tpaRSSILevel, vpaRSSILevel      level sur une echelle de 0 a 5
 */
void Display_RSSI(uint8_t tpaRSSILevel, uint8_t vpaRSSILevel){
   
    switch (disp.currentScreenNr){
            
        case DISP_SCR_NR_3:{
            DrawLargeRSSI(vpaRSSILevel, 33, 9);
            DrawLargeRSSI(tpaRSSILevel, 76, 9);
            UG_FontSelect(&FONT_4X6);
            if (vpaRSSILevel == 0) { UG_PutString(33, 0, "noSIG");}
            if (tpaRSSILevel == 0) { UG_PutString(76, 0, "noSIG");}
            
            /* pour affichage chiffre. passer alors les valeurs directement en dBm                                                                                                                                                                               */
//            char str_RSSI[7];
//            sprintf(str_RSSI, "%2ddBm", vpaRSSILevel);
//            UG_PutString(33, 0, str_RSSI);
            break;
        }
        default:
            break;
    }    
}   /* DisplayTPA_RSSI */


/* -------------------------------------------------------------------------- */
/**
 * @brief   Display specific data according screen in use
 * 
 * @param   heatIsOn        true -> symbol displayed, false -> no symbol
 * @param   temp_celsius    temperature
 */
void DisplayHeatSymbol(bool heatIsOn, double temp_celsius) {
    char str_deg[8] = "";
    uint8_t x, y;

    switch (disp.currentScreenNr) {
        case DISP_SCR_NR_3:
            x = 16;
            y = 16;
            if (heatIsOn) {
                DrawHeat(x, y);
            } else {
                UG_FillFrame(x, y, x + 10, y + 11, C_WHITE);
            }
            UG_FontSelect(&FONT_6X8);
            x = 32;
            y = 18;
            if ((temp_celsius < 10.0) & (temp_celsius > -10.0)) {
                sprintf(str_deg, "%+4.1f", temp_celsius);
                UG_PutString(x, y, str_deg);
                x += 24;
            } else {
                sprintf(str_deg, "%+.0f", temp_celsius);
                sprintf(str_deg, "%+.0f", temp_celsius);
                UG_PutString(x, y, str_deg);
                x += 18;
            }
            UG_PutChar('°', x, y, C_BLACK, C_WHITE); // PutString ne marche pas avec ce car.
            UG_PutChar('C', x + 5, y, C_BLACK, C_WHITE);
            break;
        default:
            break;
    }
}

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

    switch (screen){
        case DISP_SCR_WELCOME:
            DisplayScreen_Welcome(true);            
            break;
        case DISP_SCR_NR_1:
            DisplayScreen_Nr_1(true);
            break;
        case DISP_SCR_NR_2:
            DisplayScreen_Nr_2(true);
            break;
        case DISP_SCR_NR_3:
            DisplayScreen_Nr_3(true);
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
    if (LCD_Task_IsReady()) {
              
        switch (disp.state) {
            case DISP_STATE_WAIT_REFRESH:
            {
                if (disp.refreshTickEvent) {
                    disp.refreshTickEvent = false;
                    
                    /* traitement en fonction de la commande en cours */
                    switch (disp.command) {                                                 
                        case DISP_CMD_NOCMD: {
                            disp.page = 0;
                            break;  
                        }
                        case DISP_CMD_SCROLLDOWN_CLEAR: {
                            LCD_ScrollDown(DISPLAY_SCROLL_STEP);
                            DisplayClearPage(disp.scrollLine/DISPLAY_PAGE_HEIGHT, true);
                            disp.scrollLine+=DISPLAY_SCROLL_STEP;
                            if (disp.scrollLine>=DISPLAY_HEIGHT){
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
