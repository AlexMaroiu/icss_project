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
    static uint8_t btn_right_last = BUTTON_PRESSED;
    static uint8_t btn_left_last = BUTTON_PRESSED;

    if ((BtnDrv_ReadButton(BTN_DRV_BTN_RIGHT) == BUTTON_PRESSED) && (btn_right_last != BtnDrv_ReadButton(BTN_DRV_BTN_RIGHT)))
    {
        LedDrvToggleLEDState(LED_DRV_LED_LEFT);
    }
    btn_right_last = BtnDrv_ReadButton(BTN_DRV_BTN_RIGHT);
   
   
    if ((BtnDrv_ReadButton(BTN_DRV_BTN_LEFT) == BUTTON_PRESSED) && (btn_left_last != BtnDrv_ReadButton(BTN_DRV_BTN_LEFT)))
    {
        LedDrvToggleLEDState(LED_DRV_LED_RIGHT);
    }
    btn_left_last = BtnDrv_ReadButton(BTN_DRV_BTN_LEFT);
}


/*#################################*/
/*  Local function implementation  */
/*#################################*/

