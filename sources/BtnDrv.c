/*
 * BtnDrv.c
 *  Description: short description of source file
 *
 *  Created on: April 17, 2024
 *  Author: Peter Cristian
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include "BtnDrv.h"
#include <avr/io.h>
#include <stdio.h>
/*#################################*/
/*         Local defines           */
/*#################################*/
#define DEBOUNCEFACTOR 3 

/*#################################*/
/*        Local data types         */
/*#################################*/
typedef struct
{
   teBtnDrvID         tBtnID;
   volatile uint8_t * ucDataDirReg;
   volatile uint8_t * ucPinReg;
   uint8_t            ucPinNo;
   teBtnDrvState	  lastState;
   teBtnDrvState      stableState;
   uint8_t            debounceFactor;
} teBtnConfig;

/*#################################*/
/*        Global ROM data          */
/*#################################*/


/*#################################*/
/*        Global RAM data          */
/*#################################*/
teBtnConfig BtnConfigArray[MAX_NO_BTN]=
{
   /* Btn Left configuration */
   {
      BTN_LEFT,
      &DDRA,
      &PINA,
      PA1,
      NOT_PRESSED,
	  NOT_PRESSED,
	  0
   },
   /* Btn Right configuration */
   {
      BTN_RIGHT,
      &DDRA,
      &PINA,
      PA4,
      NOT_PRESSED,
	  NOT_PRESSED,
	  0
   }

};

/*#################################*/
/*        Local ROM data           */
/*#################################*/


/*#################################*/
/*        Local RAM data           */
/*#################################*/


/*#################################*/
/*    Local function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/


/*#################################*/
/*  Global function implementation */
/*#################################*/
void BtnDrvInit()
{
	uint8_t ucIdx;
	teBtnDrvState currentState;
	/* Set DDR register as output for all BTNs */
	for (ucIdx = 0; ucIdx < MAX_NO_BTN; ucIdx++) {
		* BtnConfigArray[ucIdx].ucDataDirReg	&= ~(1 << BtnConfigArray[ucIdx].ucPinNo);
		if (((1 << BtnConfigArray[ucIdx].ucPinNo) & * BtnConfigArray[ucIdx].ucPinReg) == 0)
			currentState = NOT_PRESSED;
		else
			currentState = PRESSED;
		BtnConfigArray[ucIdx].stableState = currentState;
		BtnConfigArray[ucIdx].lastState = currentState;
	}
}

teBtnDrvState BtnDrvGetButtonState (teBtnDrvID eBtnID)
{
	return BtnConfigArray[eBtnID].stableState;
}

void BtnDrvMain()
{
	uint8_t ucIdx;
	teBtnDrvState currentState;
	for (ucIdx = 0; ucIdx < MAX_NO_BTN; ucIdx++) 
		{
		if (((1 << BtnConfigArray[ucIdx].ucPinNo) & * BtnConfigArray[ucIdx].ucPinReg) == 0)
			currentState = NOT_PRESSED;
		else
			currentState = PRESSED;
		if (currentState != BtnConfigArray[ucIdx].lastState)
			BtnConfigArray[ucIdx].debounceFactor = 0;
		else if (BtnConfigArray[ucIdx].debounceFactor == DEBOUNCEFACTOR){
			if (BtnConfigArray[ucIdx].stableState != currentState)
			BtnConfigArray[ucIdx].stableState = currentState; 
		}
		else 
			BtnConfigArray[ucIdx].debounceFactor += 1;

	BtnConfigArray[ucIdx].lastState = currentState;

	}
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/
