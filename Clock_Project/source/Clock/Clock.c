/*
 * Clock.c
 *
 *  Created on: 13 sep. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "OLED.h"
#include "OLEDAPI.h"
#include "Time.h"
#include "Comm.h"
#include "Comm_def.h"

#include "Clock.h"
#include "Clock_def.h"

typedef enum
{
	Clock_enSegMins = 0,
	Clock_enMinHrs,
	Clock_enTotalStates
}Clock_enClockStates;

typedef enum
{
	_enJanuary = 0,
	_enFebruary,
	_enMarch,
	_enApril,
	_enMay,
	_enJune,
	_enJuly,
	_enAugust,
	_enSeptember,
	_enOctober,
	_enNovember,
	_enDecember,
	_enTotalMonths
}Clock_MonthsNames;

typedef struct
{
	uint8 au8DeadLine[12];
	uint8 au8Name[12][3];
//	"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
}Clock_stMonth;

#define CLOCK_FULL_REFRESH_ND	(5U)
#define CLOCK_FIRST_DAY			(1U)

static uint8 u8Tiks = (uint8)False;
static uint8 u8StateMachine = Clock_enMinHrs;
static uint8 u8Digi2Display = 0;
static uint8 au8Time[Clock_enTotalTimeData] = {0};
static const Clock_stMonth stMonthProps = {{31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},	\
		{"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"}	\
};
void _vDisplayClock(void)
{
	uint8 u8Len = (uint8)(OLEDAPI_enTimeDigit - u8Digi2Display);

	if(u8Len < (uint8)OLEDAPI_enFullMin)
	{
		u8Len--;
	}
	else
	{
		/* Nothing to do. */
	}

	for(; u8Len < (uint8)OLEDAPI_enTimeDigit; u8Len++)
	{

		if(u8Len == (uint8)OLEDAPI_enColon)
		{
			u8Len++;
		}
		else
		{
			/*Nothing to do. */
		}

		OLEDAPI_vDispTime((uint8)OLEDAPI_enTimeString, u8Len);
	}
}


void _vSetTime(void)
{
	if((au8Time[Time_enMinId] == 0) && (au8Time[Time_enHrId] == 0))
	{
		if(au8Time[Clock_enDayId] == stMonthProps.au8DeadLine[au8Time[Clock_enMonthId]])
		{
			au8Time[Clock_enDayId] = (uint8)CLOCK_FIRST_DAY;
			if(au8Time[Clock_enMonthId] == (uint8)_enDecember)
			{
				au8Time[Clock_enMonthId] = (uint8)_enJanuary;
			}
			else
			{
				au8Time[Clock_enMonthId]++;
			}
		}
		else
		{
			au8Time[Clock_enDayId]++;
		}

		if(au8Time[Clock_enWeekId] == (uint8)OLEDAPI_enSaturday)
		{
			au8Time[Clock_enWeekId] = (uint8)OLEDAPI_enSunday;
		}
		else
		{
			au8Time[Clock_enWeekId]++;
		}

		OLEDAPI_vSetWeekDay(au8Time[Clock_enWeekId]);
		u8Digi2Display = (uint8)CLOCK_FULL_REFRESH_ND;
	}
	else
	{
		/* Nothing to do */
	}
}

void _vClock(void)
{
	uint8 u8TimeId = u8StateMachine;
	uint8 u8Units = 0;
	uint8 u8MaxDigits = u8TimeId + 2;

	while((u8TimeId < u8MaxDigits) && ( u8Digi2Display % 2 == 0))
	{
		au8Time[u8TimeId] = Time_u8GetReqTime(u8TimeId);

		u8Digi2Display += OLEDAPI_vSetTime(u8Units, au8Time[u8TimeId]);

		u8TimeId++;
		u8Units++;
	}

	_vSetTime();
}

uint8 _u8NewHour(uint8* pau8NewMsg, uint8 u8Len)
{
	uint8 u8RetVal;

	if((u8Len == (uint8)OLEDAPI_nClock_Len))
	{
		au8Time[Time_enMinId] = (uint8)String2Num(pau8NewMsg, OLEDAPI_enFullMin);
		au8Time[Time_enHrId] = (uint8)String2Num(pau8NewMsg, OLEDAPI_enFullHr);

		Time_vSetTime((uint32)au8Time[Time_enHrId], (uint32)au8Time[Time_enMinId]);

		u8RetVal = OLEDAPI_u8SetString(OLEDAPI_enTimeString, pau8NewMsg, u8Len);
		u8Digi2Display = 4;
	}
	else
	{
		u8RetVal = (uint8)N_OK;
	}

	return u8RetVal;
}

void _vCheckConfig(void)
{
	if(u8StateMachine != (uint8)Clock_enSegMins)
	{
		au8Time[Time_enSecId] = Time_u8GetSeconds();

		if(au8Time[Time_enSecId] == 0)
		{
			_vClock();
		}
		else
		{
			/* Nothing to do. */
		}
	}
	else
	{
		_vClock();
	}

}

void Clock_vMonitor(uint8 u8State)
{
	if(u8State != (uint8)OK)
	{
		_vCheckConfig();
	}
	else
	{
		/* Nothing to do. */
	}

	OLEDAPI_vToggleSec(u8Tiks);
	u8Tiks ^= (uint8)True;
}

void Clock_vDisplay(void)
{
	if( u8Digi2Display == (uint8)CLOCK_FULL_REFRESH_ND )
	{
		OLED_vRefresh();
	}
	else
	{
		_vDisplayClock();
	}

	OLEDAPI_vDispTime((uint8)OLEDAPI_enTimeString, (uint8)OLEDAPI_enColon);
	u8Digi2Display = 0;
}
