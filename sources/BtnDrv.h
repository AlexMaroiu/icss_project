/*
 * BtnDrv.h
 *  Description: short description of header file
 *
 *  Created on: April 17, 2024
 *  Author: Peter Cristian
 */

#ifndef BtnDrv_H_
#define BtnDrv_H_

/*#################################*/
/*       Include-Files             */
/*#################################*/

/*#################################*/
/*       Global defines            */
/*#################################*/

/*#################################*/
/*      Global data types          */
/*#################################*/
typedef enum
{
	BTN_LEFT = 0u,
	BTN_RIGHT = 1u,
	MAX_NO_BTN = 2u
}teBtnDrvID;

typedef enum
{
	PRESSED = 0u,
	NOT_PRESSED = 1u
}teBtnDrvState;

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

void BtnDrvInit() __attribute__((section (".Buton_Section")));
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
void BtnDrvMain() __attribute__((section (".Buton_Section")));
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/
teBtnDrvState BtnDrvGetButtonState (teBtnDrvID eBtnID) __attribute__((section (".Buton_Section"))); 
/* Function name:
   Description:
   Function parameters:
       param_name - description
*/



#endif /* BtnDrv_H_ */
