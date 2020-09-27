/*
 * Clock.c
 *
 *  Created on: 13 sep. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "Clock.h"

#include "OLEDAPI_def.h"
#include "OLEDAPI.h"
#include "Time.h"
#include "Comm.h"
#include "Comm_def.h"

typedef enum
{
	Clock_enSegMins = 0,
	Clock_enMinHrs,
	Clock_enTotalStates
}Clock_enClockStates;

typedef struct
{
	uint8 u8StringId;
	uint8 u8Padding;
	uint8 au8Msg[OLEDAPI_nString_Len];
}Clock_tstRxMsg;

uint8 _u8NewHour(uint8* pau8NewMsg, uint8 u8Len);

#define String2Uni(c)					(c - 48)
#define String2Dec(c)					((c - 48) * 10)
#define String2Num(Msg,pos)				(String2Dec(Msg[pos]) + String2Uni(Msg[pos+1]))
#define GetRealLen(x)					(x = x - 2)

static uint8 u8Tiks = (uint8)False;
static uint8 u8StateMachine = Clock_enMinHrs;
static uint8 u8Digi2Display = 0;
static uint8 au8Time[Time_enTotalReqId] = {0};
static uint8 (*pfnNewTask[9])(uint8*, uint8) = {_u8NewHour, NULL};

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
		u8RetVal = (uint8)-1;
	}

	return u8RetVal;
}

uint8 _u8CheckMsg(void)
{
	uint8 u8CommFlags;
	Clock_tstRxMsg stMsg;
	uint8 u8Len;
	uint8 u8RetVal = N_OK;

	u8CommFlags = Comm_u8GetStatus();

	if(u8CommFlags & Comm_nNew_Msg)
	{
		u8Len = Comm_u8GetMsg((uint8*)&stMsg);

		if(u8Len == 1 && stMsg.u8StringId == 'm')
		{
			OLEDAPI_vInvDisplay();
		}
		else
		{
			stMsg.u8StringId = String2Uni(stMsg.u8StringId);
			GetRealLen(u8Len);

			if(stMsg.u8StringId >= 0 && stMsg.u8StringId <= 9)
			{
				u8RetVal = pfnNewTask[stMsg.u8StringId](stMsg.au8Msg, u8Len);
			}
			else
			{
				/* Nothing to do. */
			}
		}
	}
	else
	{
		/* Nothing to do. */
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

void Clock_vMonitor(void)
{
	uint8 u8State;

	u8State = _u8CheckMsg();

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

	OLEDAPI_vDispTime((uint8)OLEDAPI_enTimeString, (uint8)OLEDAPI_enColon);
	u8Digi2Display = 0;
}
