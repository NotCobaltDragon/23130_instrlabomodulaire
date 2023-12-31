//-----------------------------------------------------------
// File Name    : Encoder_Driver.h                          |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#ifndef ENCODER_DRIVER_H    /* Guard against multiple inclusion */
#define ENCODER_DRIVER_H

#include <stdbool.h>
#include "system_definitions.h"

typedef struct
{
    bool currentState;
    bool oldState;
} CHANNEL;

typedef struct
{
    CHANNEL sigA;
    CHANNEL sigB;
    CHANNEL sigPB;

    //bool pec12Inc;
    //bool pec12Dec;
    //bool pec12Pb;

    bool stateInc;
    bool stateDec;
    bool statePb;
} PEC12;

void ScanPec12();
void Pec12IncSet();
void Pec12DecSet();
void Pec12PbSet();
void Pec12IncClear();
void Pec12DecClear();
void Pec12PbClear();

#endif
