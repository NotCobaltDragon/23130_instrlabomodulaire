//-----------------------------------------------------------
// File Name    : Encoder_Driver.c                          |
// Project Name : 23130_instrlabomodulaire                  |
// Version      : V1                                        |
// Date         : 30.08.2023                                |
// Author       : Alexandre Steffen                         |
//-----------------------------------------------------------
#include "Encoder_Driver.h"

PEC12 pec12;
extern APP_DATA appData;

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
	if(appData.pec12Inc != true)
		appData.pec12Inc = true;
		LED1Toggle();
}

void Pec12DecSet()
{
	if(appData.pec12Dec != true)
		appData.pec12Dec = true;
		LED1Toggle();
}

void Pec12PbSet()
{
	if(appData.pec12Pb != true)
		appData.pec12Pb = true;
		LED1Toggle();
}

void Pec12IncClear()
{
	appData.pec12Inc = false;
}

void Pec12DecClear()
{
	appData.pec12Dec = false;
}

void Pec12PbClear()
{
	appData.pec12Pb = false;
}