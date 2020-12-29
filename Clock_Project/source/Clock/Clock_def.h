/*
 * Clock_def.h
 *
 *  Created on: 26 sep. 2020
 *      Author: lucky
 */

#ifndef CLOCK_CLOCK_DEF_H_
#define CLOCK_CLOCK_DEF_H_

#include "Prototype.h"

typedef enum
{
	Clock_enSecId = 0,
	Clock_enMinId,
	Clock_enHrsId,
	Clock_enDayId,
	Clock_enMonthId,
	Clock_enYearId,
	Clock_enWeekId,
	Clock_enTotalTimeData
}Clock_enTimeDataId;

#define String2Uni(c)					(c - 48)
#define String2Dec(c)					((c - 48) * 10)
#define String2Num(Msg,pos)				(String2Dec(Msg[pos]) + String2Uni(Msg[pos+1]))
#define GetRealLen(x)					(x = x - 2)

extern uint8 _u8NewHour(uint8* pau8NewMsg, uint8 u8Len);

#endif /* CLOCK_CLOCK_DEF_H_ */
