/*
 * LedDrv.c
 *  Description: Led driver source file
 *
 *  Created on: April 3, 2024
 *  Author: Maroiu Alexandru
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/
#include <avr/io.h>
#include "LedDrv.h"


/*#################################*/
/*         Local defines           */
/*#################################*/



/*#################################*/
/*        Local data types         */
/*#################################*/

typedef struct{
   volatile uint8_t * ddr;
   volatile uint8_t * port;
   uint8_t bit_index;
} config_type;

/*#################################*/
/*        Global ROM data          */
/*#################################*/

const config_type config[LED_DRV_MAX_NR_LEDS] = 
{
    {&DDRC, &PORTC, LED1_PIN},
    {&DDRC, &PORTC, LED2_PIN},
};

/*#################################*/
/*        Global RAM data          */
/*#################################*/


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
void LedDrvInit()
{
    uint8_t index;
    for (index = 0; index < LED_DRV_MAX_NR_LEDS; index++)
    {
        *(config[index].ddr) |= (1 << config[index].bit_index); //direction output
        *(config[index].port) &= ~(1 << config[index].bit_index); // output low
    }
}

void LedDrvToggleLEDState(LedDrvID bit_index)
{
    *(config[bit_index].port) ^= (1<<config[bit_index].bit_index);
}

void LedDrvSetLedState(LedDrvID bit_index, LedDrvState state)
{
    if(state == LED_DRV_LED_ON)
    {
        *(config[bit_index].port) |= (1<<config[bit_index].bit_index);
    }
    else
    {
        *(config[bit_index].port) &= ~(1 << config[bit_index].bit_index);
    }
}

/*#################################*/
/*  Global function implementation */
/*#################################*/


/*#################################*/
/*  Local function implementation  */
/*#################################*/
