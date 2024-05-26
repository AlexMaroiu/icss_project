/*
 * CySecDrv.h
 *  Description: Cyber Security Driver header
 *
 *  Created on: March 20, 2024
 *  Author: Maroiu Alex
 */

#ifndef CYSEC_DRV_H_
#define CYSEC_DRV_H_

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
    CYSEC_DRV_IS_NOT_SECURED,
    CYSEC_DRV_IS_SECURED,
} CySecDrv_StateType;

/*#################################*/
/*       Global ROM data           */
/*#################################*/

/*#################################*/
/*       Global RAM data           */
/*#################################*/

/*#################################*/
/*   Global function declaration   */
/*#################################*/

void CySecDrv_Init(void);
void CySecDrv_Main(void);
CySecDrv_StateType CySecDrv_IsHashValid(void);

#endif /* CYSEC_DRV_H_ */
