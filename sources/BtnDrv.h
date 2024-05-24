/*
 * BtnDrv.h
 *  Description: Button Driver header
 *
 *  Created on: March 20, 2024
 *  Author: Maroiu Alex
 */

#ifndef BTN_DRV_H_
#define BTN_DRV_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

#define BUTTON_PRESSED     (0U)
#define BUTTON_UNPRESSED   (1U)
#define BTN_DRV_BTN_RIGHT  (BTN_DRV_BTN_1_PIN)
#define BTN_DRV_BTN_LEFT   (BTN_DRV_BTN_2_PIN)

/*#################################*/
/*      Global data types          */
/*#################################*/
typedef enum
{
   BTN_DRV_BTN_1_PIN       = 0U,
   BTN_DRV_BTN_2_PIN       = 1U,
   BTN_DRV_MAX_NO_BUTTONS  = 2U,
} BtnDrv_pins_type;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/

void BtnDrv_Init(void) __attribute__ ((section(".BUTTON_SECTION")));
void BtnDrv_Main(void) __attribute__ ((section(".BUTTON_SECTION")));
uint8_t BtnDrv_ReadButton(uint8_t index) __attribute__ ((section(".BUTTON_SECTION")));

#endif /* BTN_DRV_H_ */
