/*
 * Time.c
 *
 *  Created on: 21 ago. 2020
 *      Author: lucky
 */

#include "Prototype.h"
#include "assert.h"

#include <OLEDAPI/OLEDAPI.h>
#include <OLEDAPI/OLEDAPI_def.h>

#include "Time.h"

#define Time_Min2Sec(min)						(min = (uint32)(min * 60))
#define Time_Hrs2Sec(hrs)						(hrs = (uint32)(hrs * 3600))
#define Time_Operation(div, mod)				(u32CntReg%mod/div)
#define Time_Get_Requested_Time(ReqId)			Time_Operation(au32OperatorVals[ReqId], au32OperatorVals[ReqId+1])
#define Time_ArLEN								(Time_enTotalReqId + 1)

static uint32 u32CntReg = 1;
static uint32 au32OperatorVals[Time_ArLEN] = {1, 60, 3600, 86400, 604800};

/*Todo realizar APIS para poder configurar el reloj de una forma más facil y poder determinar si ver sec y min o min y hrs
 * Desacoplar la funcion de monitor, que no tenga tata dependencia con Clock_u8WriteDate. u8Write SOLO tiene que pasar el numero a
 * char y mandarlo a imprimir en la pantalla virtual. Se tiene que generar más independecia entre estas dos funciones
 * es decir, quitar el apuntador (si se puede) y la operacion para escribir en u8WriteDate tiene que mejorar.
 */

//El retorno puede regresar un valor del 0 al 2. Donde 0 significa que Es necesario decodifcar el siguiente numero.
//1 Significa que solo se cambio un digito y 2 significa que se cambio 2 digitos pero no hace falta ver el siguiente.
/*
*/

/* Se usaran o no...? */
uint8 Time_u8GetSeconds(void)
{
    return (uint8)Time_Get_Requested_Time(Time_enSecId);
}

uint8 Time_u8GetMin(void)
{
    return (uint8)Time_Get_Requested_Time(Time_enMinId);
}

uint8 Time_u8GetHrs(void)
{
    return (uint8)Time_Get_Requested_Time(Time_enHrId);
}

uint8 Time_u8GetDay(void)
{
	return (uint8)Time_Get_Requested_Time(Time_enDayId);
}

uint8 Time_u8GetReqTime(uint8 u8RequestId)
{
	assert(u8RequestId < Time_enTotalReqId);
    return (uint8)Time_Get_Requested_Time(u8RequestId);
}

uint32* Time_pu8GetRealTime(void)
{
	return &u32CntReg;
}

uint32 Time_u32GetRawTime(void)
{
	return u32CntReg;
}

void Time_vSetTime(uint32 u32Hrs, uint32 u32Min)
{
	uint32 u32Sec;

	assert(u32Hrs < 24);
	assert(u32Min < 60);

	u32Sec = (uint32)Time_u8GetSeconds();
	Time_Min2Sec(u32Min);
	Time_Hrs2Sec(u32Hrs);

	u32CntReg = u32Sec + u32Min + u32Hrs;
}


