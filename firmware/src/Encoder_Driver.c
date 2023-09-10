//-----------------------------------------------------------
// File Name    : Encoder_Driver.c                          |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "Encoder_Driver.h"

PEC12 pec12;

void ScanPec12()
{
	pec12.sigA.currentState = PEC_AStateGet();
	pec12.sigB.currentState = PEC_BStateGet();
	pec12.sigPB.currentState = PEC_PBStateGet();

	if(pec12.sigA.currentState < pec12.sigA.oldState)
	{
		if(pec12.sigB.currentState != pec12.sigA.currentState)
			Pec12IncSet();
		else
			Pec12DecSet();
	}
	if(pec12.sigPB.currentState < pec12.sigPB.oldState)
		Pec12PbSet();

	pec12.sigA.oldState = pec12.sigA.currentState;
	pec12.sigB.oldState = pec12.sigB.currentState;
	pec12.sigPB.oldState = pec12.sigPB.currentState;
}

void Pec12IncSet()
{
	if(pec12.stateInc != true)
		pec12.stateInc = true;
}

void Pec12DecSet()
{
	if(pec12.stateDec != true)
		pec12.stateDec = true;
}

void Pec12PbSet()
{
	if(pec12.statePb != true)
		pec12.statePb = true;
}

void Pec12IncClear()
{
	pec12.stateInc = false;
}

void Pec12DecClear()
{
	pec12.stateDec = false;
}

void Pec12PbClear()
{
	pec12.statePb = false;
}