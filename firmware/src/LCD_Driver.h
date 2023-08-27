/** 
 * @file:   LCD_Driver.h
 * #        LCD_DRIVER.h 
 * ## Module pour l'affichage sur un LCD graphique Newhaven
 * 
 * @author  J. Jose Moreno
 * @date    16.04.2020
 * @version 1.0
 * 
 * ## Description
 * Ce module fournit les fonctions d'ecriture pour afficher sur un LCD au moyen
 * d'un SPI.
 */
/* ************************************************************************** */

#ifndef LCD_DRIVER_H    /* Guard against multiple inclusion */
#define LCD_DRIVER_H

/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "system_definitions.h"

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* definitions pour l'ecran LCD */
/*------------------------------*/
/* display resolution. */
#define LCD_WIDTH           128
#define LCD_HEIGHT          64
#define LCD_PAGES           8
#define LCD_BAUDRATE        1250000         // must be < 2.5 Mbits/s
#define LCD_DRV_SPI_INDEX   DRV_SPI_INDEX_0 // Changer si autre instance utilisee

#define LCD_CMD_BUF_SIZE    8
#define LCD_DATA_BUF_SIZE   128

// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************
typedef enum
{
	/* LCD's state machine's initial state. */
	LCD_STATE_RESET=0,
            LCD_STATE_INIT_7565,
            LCD_STATE_INIT_RGB,
            LCD_STATE_DATA_PAGE_SET,
            LCD_STATE_DATA_COL_SET,
            LCD_STATE_READY,
            LCD_STATE_TX_BUSY
} LCD_STATES;

typedef struct{
//    SYS_MODULE_OBJ spiObject;
    DRV_HANDLE handleSPI;
    DRV_SPI_BUFFER_HANDLE bufHandleSPI;
    DRV_SPI_BUFFER_HANDLE bufHandle2SPI;
    DRV_SPI_CLIENT_DATA configData;
    uint8_t cmdBuffer[LCD_CMD_BUF_SIZE];
    uint8_t cmdBufSize;
    uint8_t dataBuffer[LCD_DATA_BUF_SIZE];
    uint8_t dataBufSize;
    uint8_t currentPage;
    uint8_t currentColumn;
    uint8_t currentStartLine;
    LCD_STATES state;
    uint8_t initStep;
    bool transferIsInProgress;
} T_LCD;

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Task Callback Routines                                     */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
///@}

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Interface Functions                                        */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
/* -------------------------------------------------------------------------- */
/**
 * @brief   Sends the Display start line se cmd to the LCD
 * @param   lines   to be added to the current start line
 * 
 * @remark  call this function to make scroll down step by lines
 */
void LCD_ScrollDown(uint8_t lines);

/* -------------------------------------------------------------------------- */
/**
 * @brief   Sends the buffer data to the LCD
 * @param   *buf        pointer to the buffer
 * @param   bufSize     nr of bytes to send
 * @param   page        vertical position (1 page = 8 bits vertical)
 * @param   column      horizontal position of the 1st data
 * 
 * Note: call this function to write on another page.
 */
void LCD_DataWrite(uint8_t *buf, uint8_t bufSize, uint8_t page, uint8_t column);

/* -------------------------------------------------------------------------- */
/**
 * @brief   Resets the LCD module, must be maintained min 0.5us
 * 
 */
void LCD_Reset();
 
/* -------------------------------------------------------------------------- */
/**
 * @brief   Initialize the LCD communication port and data
 * 
 * @return  false if init failed
 */
bool LCD_Init();

/* -------------------------------------------------------------------------- */
/**
 * @brief   Task running the display state machine, must be called periodically
 *          by the client.
 * 
 * @return  true if the LCD is ready for a new transfer
 */
bool LCD_Task_IsReady();

///@}

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* LCD_DRIVER_H */

/* *****************************************************************************
 End of File
 */
