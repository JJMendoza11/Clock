/*
 * Clock.c
 *
 *  Created on: 13 sep. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "Clock.h"

#include "OLEDAPI.h"
#include "Time.h"

typedef enum
{
	Clock_enSegMins = 0,
	Clock_enMinHrs,
	Clock_enTotalStates
}Clock_enClockStates;

static uint8 u8Tiks = (uint8)False;
static uint8 u8StateMachine = 0;
static uint8 u8Digi2Display = 0;
static uint8 au8Time[3] = {0};

void Clock_vMonitor(void)
{
	uint8 i = u8StateMachine;
	uint8 u8MaxDigits = i + 2;

	while((i < u8MaxDigits) && ( u8Digi2Display % 2 == 0))
	{
		au8Time[i] = Time_u8GetReqTime(i);

		u8Digi2Display += OLEDAPI_vSetTime(i, au8Time[i]);

		i++;
	}

	OLEDAPI_vToggleSec(u8Tiks);
	u8Tiks ^= (uint8)True;
}

void Clock_vDisplay(void)
{

}
