#ifndef ENCODER_DRIVER_H
#define ENCODER_DRIVER_H

// définitions des types qui seront utilisés dans cette application
#include "app.h"


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
} PEC12;

void ScanPec12();
void Pec12IncSet();
void Pec12DecSet();
void Pec12PbSet();
void Pec12IncClear();
void Pec12DecClear();
void Pec12PbClear();


#endif
