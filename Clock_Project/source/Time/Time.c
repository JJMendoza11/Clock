/*
 * Time.c
 *
 *  Created on: 21 ago. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "Time.h"
#include "Watch.h"
#include "Watch_def.h"


static uint32 u32CntReg = 1;
static uint8 au8Time[3];

/*Tiene que ser esta variable tipo global? */
static uint8 u8Digit2Displ = 0;

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
uint8 _u8GetSeconds(void)
{
    uint8 u8RetVal;
    au8Time[2] = u32CntReg%60;

    u8RetVal = Clock_u8WriteDate(2,au8Time[2], &u8Digit2Displ);

    return u8RetVal;
}

uint8 _u8GetMin(void)
{
    uint8 u8RetVal;
    au8Time[1] = u32CntReg%3600/60;

    u8RetVal = Clock_u8WriteDate(1,au8Time[1], &u8Digit2Displ);

    return u8RetVal;
}

uint8 _u8GetHrs(void)
{
    uint8 u8RetVal;
    au8Time[0] = u32CntReg%86400/3600;

    u8RetVal = Clock_u8WriteDate(0,au8Time[0], &u8Digit2Displ);

    return u8RetVal;
}

/* ------------------------------------------------------------------- */
/* ------------------------------------------------------------------- */
/* ------------------------------------------------------------------- */

uint8 _u8GetTime(uint8 u8Id, uint32 u32Modul, uint32 u32Div)
{
    uint8 u8RetVal;
    au8Time[u8Id] = u32CntReg%u32Modul/u32Div;

    u8RetVal = Clock_u8WriteDate(u8Id,au8Time[u8Id], &u8Digit2Displ);

    return u8RetVal;
}

uint8 Time_u8Monitor(void)
{
  uint8 u8Index = 0;
  uint8 u8RetVal = 0;
  uint32 u32Vals[4] = {1, 60, 3600, 86400};


  u8Digit2Displ = 0;

  while(u8Index < 2 && u8RetVal == (uint8)OK)
  {
    u8RetVal += _u8GetTime(u8Index, u32Vals[u8Index+1], u32Vals[u8Index]);

    u8Index++;
  }

  return (uint8)Clock_enTimeDigit - u8Digit2Displ;
}

uint32* Time_pu8GetRealTime(void)
{
	return &u32CntReg;
}


