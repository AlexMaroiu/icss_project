/*
 * Application.c
 *  Description: Application Driver c source file
 *
 *  Created on: March 20, 2024
 *  Author: Maroiu Alex
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/

/*Include project header files*/
#include <avr/io.h>
#include "Application.h"
#include "BtnDrv.h"
#include "LedDrv.h"
#include "CySecDrv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/


/*#################################*/
/*        Local data types         */
/*#################################*/


/*#################################*/
/*        Global ROM data          */
/*#################################*/

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


/*#################################*/
/*  Global function implementation */
/*#################################*/

void Applicaiton_Init(void)
{

}

void Application_Main(void)
{
    static uint8_t btn_right_last = PRESSED;
    static uint8_t btn_left_last = PRESSED;

    if(CySecDrv_IsHashValid() == CYSEC_DRV_IS_SECURED)
    {
        if ((BtnDrvGetButtonState(BTN_RIGHT) == PRESSED) && (btn_right_last != BtnDrvGetButtonState(BTN_RIGHT)))
        {
            LedDrvToggleLEDState(LED_DRV_LED_LEFT);
        }
        btn_right_last = BtnDrvGetButtonState(BTN_RIGHT);
    
    
        if ((BtnDrvGetButtonState(BTN_LEFT) == PRESSED) && (btn_left_last != BtnDrvGetButtonState(BTN_LEFT)))
        {
            LedDrvToggleLEDState(LED_DRV_LED_RIGHT);
        }
        btn_left_last = BtnDrvGetButtonState(BTN_LEFT);
    }
    else
    {
        LedDrvSetLedState(LED_DRV_LED_LEFT, LED_DRV_LED_OFF);
        LedDrvSetLedState(LED_DRV_LED_RIGHT, LED_DRV_LED_OFF);
    }
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/

