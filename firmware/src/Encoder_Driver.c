// GesPec12.c  Canevas pour réalisation  
// C. HUBER    09/02/2015

// Fonctions pour la gestion du Pec12
//
//
// Principe : Il est nécessaire d'appeler cycliquement la fonction ScanPec12
//            avec un cycle de 1 ms
//
//  Pour la gestion du Pec12, il y a 9 fonctions à disposition :
//       Pec12IsPlus       true indique un nouveau incrément
//       Pec12IsMinus      true indique un nouveau décrément
//       Pec12IsOK         true indique action OK
//       Pec12IsESC        true indique action ESC
//       Pec12NoActivity   true indique abscence d'activité sur PEC12
//  Fonctions pour quittance des indications
//       Pec12ClearPlus    annule indication d'incrément
//       Pec12ClearMinus   annule indication de décrément
//       Pec12ClearOK      annule indication action OK
//       Pec12ClearESC     annule indication action ESC
//
//
//---------------------------------------------------------------------------

// définitions des types qui seront utilisés dans cette application

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