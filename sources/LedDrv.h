/*
 * LedDrv.h
 *  Description: Led driver header file
 *
 *  Created on: April 3, 2024
 *  Author: Maroiu Alexandru
 */

#ifndef LED_DRV_H_
#define LED_DRV_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

#define LED_DRV_LED_RIGHT     (LED1_PIN)
#define LED_DRV_LED_LEFT      (LED2_PIN)

/*#################################*/
/*      Global data types          */
/*#################################*/

typedef enum
{
   LED1_PIN,
   LED2_PIN,
   LED_DRV_MAX_NR_LEDS,
} LedDrvID;

typedef enum
{
   LED_DRV_LED_OFF,
   LED_DRV_LED_ON,
} LedDrvState;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void LedDrvInit();

void LedDrvToggleLEDState(LedDrvID bit_index);

void LedDrvSetLedState(LedDrvID bit_index, LedDrvState state);

#endif /* LED_DRV_H_ */

