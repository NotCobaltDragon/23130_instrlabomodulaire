/** 
 * @file:   LCD_Driver.c
 * #        LCD_DRIVER.c 
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

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */
#include "LCD_Driver.h"

/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: File Scope or Global Data                                  */
/* ************************************************************************** */
/* ************************************************************************** */
///@{
static T_LCD LCD;

///@}
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Task Callback Routines                                     */
/* ************************************************************************** */
/* ************************************************************************** */
///@{

/* -------------------------------------------------------------------------- */
/**
 * @brief   Callback pour le SPI Driver. Actions au debut de transfert, par ex.
 *          activer le signal SS du SPI.
 * 
 */
void LCD_ComTransferStart(){
    LCD.transferIsInProgress = true;
    LCD_CS_Off();
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Callback pour le SPI Driver. Actions a la fin du transfert, par ex.
 *          desactiver le signal SS du SPI.
 * 
 */
void LCD_ComTransferEnd(){
    LCD_CS_On();
    LCD.transferIsInProgress = false;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Callback pour le SPI Driver. Actions en fin d'ecriture.
 * 
 */
void LCD_BufferWriteEnd (){
}

///@}
/* ************************************************************************** */
/* ************************************************************************** */
/** @name Section: Local Functions                                            */
/* ************************************************************************** */
/* ************************************************************************** */
///@{

/* -------------------------------------------------------------------------- */
/** GENERALITES:    les fonctions deposent les commandes a envoyer dans la 
 *                  structure LCD.cmdBuffer. La fonction LCD_WriteCommand envoie
 *                  le contenu au module LCD.
*/

/* -------------------------------------------------------------------------- */
/**
 * @brief   Envoie une commande dans le buffer du driver
 *          Les donnees sont prises dans le buffer de la variable LCD
 */
void LCD_WriteCommand(){
    LCD_A0_Off();       // pour commande           
    LCD.bufHandleSPI = DRV_SPI_BufferAddWrite2( 
            LCD.handleSPI,
            LCD.cmdBuffer, LCD.cmdBufSize,
            LCD_BufferWriteEnd, NULL, 
            &LCD.bufHandle2SPI );  
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Envoie une trame de donnees dans le buffer du driver
 *          Les donnees sont prises dans le buffer de la variable LCD
*/
void LCD_WriteData(){
    LCD_A0_On();        // pour des donnees
    LCD.bufHandleSPI = DRV_SPI_BufferAddWrite2( 
            LCD.handleSPI,
            LCD.dataBuffer, LCD.dataBufSize,
            LCD_BufferWriteEnd, NULL, 
            &LCD.bufHandle2SPI );  
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande d'allumage de l'affichage
*/
void LCD_Cmd_DisplayOn(){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xAF;       
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();         // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande d'extinction de l'affichage
*/
void LCD_Cmd_DisplayOff(){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xAE;       
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();         // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (3) pour initialiser la page d'ecriture (rangee)
*/
void LCD_Cmd_PageAddressSet (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xB0 | (0x0F & LCD.currentPage);
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (4) pour initialiser la page d'ecriture (rangee)
*/
void LCD_Cmd_ColumnAddressSet (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x10 | (LCD.currentColumn>>4);        // 4 MSbits
    LCD.cmdBufSize += 1;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x00 | (0x0F & LCD.currentColumn);    // 4 LSbits
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (8) selection ADC
*/
void LCD_Cmd_ADCSelect (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xA0;        // Bias = 1/9
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (9) selection ADC
 *  mode    : 0 = normal; 1 = reverse
*/
void LCD_Cmd_DisplayNormalReverse (uint8_t mode){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xA6 | (0x01 & mode);
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (11) pour initialiser l'alimentation PS bias rate
*/
void LCD_Cmd_PowerSupplyBiasRateSet (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xA2;        // Bias = 1/9
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (15) selection du mode de scan
 *
 * @remark  pour l'instant fixe en mode normal, 1/65 duty
*/
void LCD_Cmd_CommonOutputModeSelect (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0xC0;
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (16) pour initialiser l'alimentation interne
 * 
 * @remark  Booster ON, V regulator ON, V follower ON
*/
void LCD_Cmd_PowerControllerSet (){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x2F;
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (17) pour regler l'alimentation de contraste d'affichage
 * @param   ratio   rapport Rb/Ra valeur comprise entre 0 et 7
*/
void LCD_Cmd_V5_RegulatorInternalResistorRatio (uint8_t ratio){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x20 | (0x7 & ratio);    // Resistor ratio (value 3 LSB)
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();                 // envoi de la commande
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Commande LCD (18) pour initialiser l'alimentation interne
 * @param   volume  niveau de tension valeur comprise entre 0 et 63 (0x3F)
*/
void LCD_Cmd_ElectronicVolume (uint8_t volume){
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x81;           // Electronic Volume Mode Set
    LCD.cmdBufSize += 1;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x3F & volume;  // Electronic Volume register (level 6 LSB)
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();                // envoi de la commande
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
 * @brief   Sends the Display start line se cmd to the LCD
 * @param   lines   to be added to the current start line
 * 
 * @remark  call this function to make scroll down step by lines
 */
void LCD_ScrollDown(uint8_t lines){
    LCD.currentStartLine = (LCD.currentStartLine + lines) % LCD_HEIGHT;
    LCD.cmdBufSize = 0;
    LCD.cmdBuffer[LCD.cmdBufSize] = 0x40 | LCD.currentStartLine;           // Electronic Volume Mode Set
    LCD.cmdBufSize += 1;
    LCD_WriteCommand();             // envoi de la commande
    LCD.state = LCD_STATE_TX_BUSY;
}

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
void LCD_DataWrite(uint8_t *buf, uint8_t bufSize, uint8_t page, uint8_t column){
    LCD.currentPage = page;
    LCD.currentColumn = column;
    LCD.dataBufSize = bufSize;
    memcpy(&LCD.dataBuffer[0], buf, bufSize);
    LCD_Cmd_PageAddressSet(page);
    LCD.state = LCD_STATE_DATA_PAGE_SET;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Resets the LCD module, must be maintained min 0.5us
 * 
 */
void LCD_Reset(){
    LCD_RESET_Off();
    //RESET_DIS_Off();                // reset pin low -> hardware reset
    LCD.state = LCD_STATE_RESET;
    LCD.initStep = 10;              // pour boucler et stabiliser apres reset
}


/* -------------------------------------------------------------------------- */
/**
 * @brief   Initialize the LCD communication port and data
 * 
 * @return  false if init failed
 */
bool LCD_Init(){
    
    LCD_Reset();
    
    /* init du SPI, ouverture du driver */
    LCD.handleSPI = DRV_SPI_Open( LCD_DRV_SPI_INDEX, DRV_IO_INTENT_READWRITE );
    
    /* Define start & end callback routines */
    LCD.configData.operationStarting = LCD_ComTransferStart;
    LCD.configData.operationEnded = LCD_ComTransferEnd;
    LCD.configData.baudRate = LCD_BAUDRATE;
    int32_t cfg = DRV_SPI_ClientConfigure(LCD.handleSPI, &LCD.configData);
    
    /* init variables principales */
    LCD.currentPage = 0;
    LCD.currentColumn = 0;
    LCD.currentStartLine = 0;
    LCD.transferIsInProgress = false;
    
    return cfg>=0;
}

/* -------------------------------------------------------------------------- */
/**
 * @brief   Task running the display state machine, must be called periodically
 *          by the client.
 * 
 * @return  true if the LCD is ready for a new transfer
 */
bool LCD_Task_IsReady(){
    
    switch (LCD.state)
    {
        /* relacher la pulse de reset et passer a l'init du LCD */
        case LCD_STATE_RESET:{
            LCD_RESET_On();

            if (LCD.initStep == 0) { // boucle stabilisation apres reset (pas tres joli, mais bon...)
                LCD.state = LCD_STATE_INIT_7565;
            } else {
                LCD.initStep--;
            }
            break;
        }

        /* init du LCD selon datasheet 7565 */
        case LCD_STATE_INIT_7565:{
            if (!LCD.transferIsInProgress) {
                switch (LCD.initStep) {
                    case 0:{
                        LCD_Cmd_PowerSupplyBiasRateSet();
                        break;
                    }
                    case 1:{
                        LCD_Cmd_ADCSelect();
                        break;
                    }
                    case 2:{
                        LCD_Cmd_CommonOutputModeSelect();
                        break;
                    }
                    case 3:{
                        LCD_Cmd_V5_RegulatorInternalResistorRatio (0x06);
                        break;
                    }
                    case 4:{
                        LCD_Cmd_ElectronicVolume (0x10);
                        break;
                    }
                    case 5:{
                        LCD_Cmd_PowerControllerSet ();
                        break;
                    }
                    case 6:{
                        break;
                    }
                    case 7:{
                        break;
                    }
                    default:{ // fin de l'init
                        LCD_Cmd_DisplayOn();
                        LCD.state = LCD_STATE_TX_BUSY;
                        break;
                    }
                }
                LCD.initStep++;
            }
            break;
        }        
                                
        /* Attente fin d'envoi cmd PageAddresSet, puis init de la colonne */
        case LCD_STATE_DATA_PAGE_SET:{
            if (!LCD.transferIsInProgress){
                LCD_Cmd_ColumnAddressSet ();
                LCD.state = LCD_STATE_DATA_COL_SET;
            }
            break;
        }

        /* Attente fin d'envoi cmd ColumnAddresSet, puis envoi des donnees */
        case LCD_STATE_DATA_COL_SET:{
            if (!LCD.transferIsInProgress){
                LCD_WriteData ();
                LCD.state = LCD_STATE_TX_BUSY;
            }
            break;
        }

        /* Attente d'une trame envoyer */
        case LCD_STATE_READY:{
            if (LCD.transferIsInProgress){
                LCD.state = LCD_STATE_TX_BUSY;
            }
            break;
        }
        
        /* Transmission en cours, LCD occupe */
        case LCD_STATE_TX_BUSY:{
            if (!LCD.transferIsInProgress){
                LCD.state = LCD_STATE_READY;
            }
            break;
        }
        default:{
            break;
        }
    }
    
    return (LCD.state == LCD_STATE_READY);
} 

///@}

/* *****************************************************************************
 End of File LCD_Driver
 */
    