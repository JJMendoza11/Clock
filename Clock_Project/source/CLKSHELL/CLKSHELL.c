/*
 * CLKSHELL.c
 *
 *  Created on: 29 dic. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "CLKSHELL.h"

#include "Clock.h"
#include "Clock_def.h"

#include "OLEDAPI_def.h"
#include "OLEDAPI.h"
#include "Comm.h"
#include "Comm_def.h"


static uint8 (*pfnNewTask[2])(uint8*, uint8) = {_u8NewHour, NULL};

void vCLKSHELL(void)
{
	uint8 u8CommStatus;
	uint8 u8Len;
	uint8 u8State = N_OK;
	Clock_tstRxMsg stMsg;

	u8CommStatus = Comm_u8GetStatus();

	if(u8CommStatus & Comm_nNew_Msg)
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
				u8State = pfnNewTask[stMsg.u8StringId](stMsg.au8Msg, u8Len);
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

	Clock_vMonitor(u8State);
}

