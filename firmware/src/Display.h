/** 
 * @file:   Display.h
 * #        DISPLAY.h 
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
 * Remplacement de TPA_D... par D...
 */
/* ************************************************************************** */

#ifndef DISPLAY_H    /* Guard against multiple inclusion */
#define DISPLAY_H


/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <ctype.h>
#include "system_definitions.h"
#include "ugui.h"
#include "LCD_Driver.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

// *****************************************************************************
// Section: Constants
// *****************************************************************************

/** @name Definitions pour l'ecran LCD */
/*------------------------------*/
/* display resolution. */
#define DISPLAY_WIDTH           128
#define DISPLAY_HEIGHT          64
#define DISPLAY_PAGE_HEIGHT     8
#define DISPLAY_PAGES           8
#define DISPLAY_SCROLL_STEP     8
	
/** @name Definitions pour le backlight RGB */
#define DISP_BLRGB_PWM_PERIOD   6249    // timer 3 (10ms)
#define DISP_BLRGB_PWM_FACTOR   DISP_BLRGB_PWM_PERIOD/255

/** @name Couleurs utilisees ou redefinies ayant une bonne luminosite */
#define COL_AQUA                C_AQUA
#define COL_AQUA_MARINE         C_AQUA_MARINE
#define COL_ORANGE_RED          C_ORANGE_RED
#define COL_RED                 C_RED
#define COL_BLACK               C_BLACK
#define COL_WHITE               C_WHITE
#define COL_YELLOW              C_YELLOW
#define COL_RED_CUSTOM          0xFF0010
#define COL_GREEN_CUSTOM        C_LIME
#define COL_MEDGREEN_CUSTOM     0x00FF2D
#define COL_RED2_CUSTOM         0xFF0018
#define COL_YELLOW_CUSTOM       0xFF8A00
#define COL_PINK_CUSTOM         0xFF004B

#define COL_DEFAULT             COL_WHITE
  
 /** @name Timing */
#define DISPLAY_REFRESH_PERIOD  20      //in 10 ms units

/** @name Definitions pour les graphismes et textes */
#define BAT_ZERO_PERCENTLEVEL           5       // below this %level, will show 0%
#define RSSI_BARS_NR                    5

// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
/** @name Liste des ecrans a afficher */
typedef enum{
	DISP_SCR_WELCOME=0,             ///< ecran d'accueil
	/* Liste des ecrans */
	DISP_SCR_MAIN_MENU,          ///< page ecran no ...
	DISP_SCR_ERROR,
	DISP_SCR_23132,
	DISP_SCR_LAST
} E_DISPLAY_SCREENS;

/** @name Liste de commandes d'affichage */
typedef enum {
	DISP_CMD_NOCMD=0,                   ///< Pas de commande
			DISP_CMD_SCROLLDOWN_CLEAR,  ///< Effacement pour mode de defilement
} T_DISP_COMMAND;

/** @name Etats de l'affichage */
typedef enum{
	DISP_STATE_WAIT_REFRESH=0,          ///< Etat d'attente d'un refresh
			DISP_STATE_REFRESHING,      ///< Etat d'execution du refresh du LCD
//            DISP_STATE_SCROLLDOWN_CLEAR
} DISPLAY_STATES;

/** name@ matrice pour la copie d'ecran a afficher */
typedef uint8_t T_DISPLAY_MATRIX [DISPLAY_PAGES] [DISPLAY_WIDTH] ;

/** @name structure des donnees pour l'affichage */
typedef struct{
	T_DISPLAY_MATRIX screenCopy;
	uint8_t currentScreenNr;
	bool warningDataCanBeDisplayed;
	uint8_t page;
	uint8_t scrollLine;
	bool refreshTickEvent;
	uint8_t refreshDelayCount;
	uint8_t graphTime;
	T_DISP_COMMAND command;
	DISPLAY_STATES state;
	UG_GUI gui;
} T_DISPLAY;

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
void Display_TimerCallback(void);

/* -------------------------------------------------------------------------- */
/**
 * @brief   Fonction de Callback appelee par la lib ugui pour generer un pixel 
 *          pour l'affiche cible.      
 * @param   x   axe des x sur le LCD. La valeur est utilisable directement.
 * @param   y   axe des y sur le LCD, il est segmente en 'pages' de 8 bits (bytes).
 *              La position du bit est calculee pour la bonne page.
 * @param   c   couleur (noir ou blanc du pixel
 */
void DisplayPixelSetCallback(UG_S16 x, UG_S16 y, UG_COLOR c);

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
void DisplaySetBacklightRGB(uint32_t rgb);

/* -------------------------------------------------------------------------- */
/**
 * @brief   Initiate a scroll clear of the display
 * 
 */
void DisplayScrollClear();

/* -------------------------------------------------------------------------- */
/**
 * @brief   Clear Display data
 *  
 * @param setToClear    false = dark ; true = clear
 * 
 * Texec = 245 us
 */
void DisplayClear(bool setToClear);

void DisplayScreen_Error(void);

/* ----------------------------------------------------------------------------*/
/**
 * @brief   Display specific data according screen in use
 *          -- EXEMPLE DE FONCTION A ADAPTER --
 * 
 * @param  a    valeur a 
 * @param  b    valeur b
 * @param  c    valeur c 
 */
void DisplayValues_23132(uint8_t id, float voltmeterValue, bool currentMode, bool holdMode, uint8_t position);

//void DrawMenuIcon(bool selected);

void DisplayScreen_23132(bool setToDark);

/* -------------------------------------------------------------------------- */
/**
 * @brief   Affiche un ecran 'fond' ou 'trame de base'
 * 
 * @param   screen      base screen # to be displayed
 * @param   setToDark   true = dark ; false = clear
 * 
 */
void DisplayScreen(uint8_t screen, bool setToDark);      
  
/* -------------------------------------------------------------------------- */
/**
 * @brief   Initialize communication to the LCD display, graphical user 
 *          interface (GUI) and data 
 * @return  false if init failed
 */
bool DisplayInit();

/* -------------------------------------------------------------------------- */
/**
 * @brief   Task updates/refreshes the display, must be called periodically by the client
 * 
 */
void Display_Task();


///@}

	/* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* DISPLAY_H */

/* *****************************************************************************
 End of File
 */
