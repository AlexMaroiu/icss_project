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
    SHA_SUCCESS = 0,
    SHA_NULL,            /* Null pointer parameter */
    SHA_INPUT_TOO_LONG,    /* input data too long */
    SHA_STATE_ERROR       /* called Input after Result */
} CySecDrv_SHA_StatusType;

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
uint8_t CySecDrv_IsHashValid(void);

#endif /* CYSEC_DRV_H_ */
