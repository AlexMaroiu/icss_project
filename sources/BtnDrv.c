/*
 * BtnDrv.c
 *  Description: Button Driver c source file
 *
 *  Created on: March 20, 2024
 *  Author: Maroiu Alex
 */

/*#################################*/
/*         Include-Files           */
/*#################################*/

/*Include project header files*/
#include <avr/io.h>
#include "BtnDrv.h"

/*#################################*/
/*         Local defines           */
/*#################################*/

#define DEBOUNCE_TIME      (10U)

/*#################################*/
/*        Local data types         */
/*#################################*/

typedef struct{
   volatile uint8_t * ddr;
   volatile uint8_t * pin;
   uint8_t bit_index;
} config_type_btn;

typedef struct
{
   uint8_t last_state;
   uint8_t output_state;
   uint8_t debounce_counter;
} BtnDrv_state_type;

/*#################################*/
/*        Global ROM data          */
/*#################################*/

/*#################################*/
/*        Global RAM data          */
/*#################################*/

/*#################################*/
/*        Local ROM data           */
/*#################################*/

const config_type_btn btn_config[] = 
{
    {&DDRA, &PINA, BTN_DRV_BTN_1_PIN},
    {&DDRA, &PINA, BTN_DRV_BTN_2_PIN},
};

/*#################################*/
/*        Local RAM data           */
/*#################################*/

BtnDrv_state_type buttons[BTN_DRV_MAX_NO_BUTTONS];

/*#################################*/
/*    Local function declaration   */
/*#################################*/

uint8_t BtnDrv_ReadPin(uint8_t index) __attribute__ ((section(".BUTTON_SECTION")));

/*#################################*/
/*  Global function implementation */
/*#################################*/


/*#################################*/
/*  Local function implementation  */
/*#################################*/

void BtnDrv_Init(void)
{
    uint8_t index;
    for (index = 0; index < BTN_DRV_MAX_NO_BUTTONS; index++)
    {
        *(btn_config[index].ddr) &= ~(1 << btn_config[index].bit_index); // direction input
        *(btn_config[index].pin) &= ~(1 << btn_config[index].bit_index); // input low
        
        buttons[index].output_state = 0;
        buttons[index].debounce_counter = 0;
    }
}

uint8_t BtnDrv_ReadButton(uint8_t index)
{
    return buttons[index].output_state;
}

void BtnDrv_Main(void)
{
    uint8_t index;
    uint8_t current_state;
   
    for (index = 0; index < BTN_DRV_MAX_NO_BUTTONS; index++)
    {
        current_state = BtnDrv_ReadPin(index);
        if(current_state != buttons[index].last_state)
        {
            buttons[index].debounce_counter = 0;
            buttons[index].last_state = current_state;
        }
        else
        {
            if(buttons[index].debounce_counter < DEBOUNCE_TIME)
            {
                buttons[index].debounce_counter++;
            }
            else
            {
                buttons[index].output_state = buttons[index].last_state;
            }
        }
    }
    
}

uint8_t BtnDrv_ReadPin(uint8_t index)
{
    return ((*(btn_config[index].pin) >> btn_config[index].bit_index)) & 0x01U;
}