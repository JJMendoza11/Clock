/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    Clock_Project.c
 * @brief   Application entry point.
 */

/* TODO: insert other include files here. */
#include "Prototype.h"
#include "assert.h"

#include "Comm.h"
#include "Comm_cfg.h"

#include "Time.h"
#include "CLKSHELL.h"
#include "Clock.h"
#include "OLEDAPI_def.h"
#include "OLEDAPI.h"
#include "I2CDrive.h"
#include "Pit.h"

#define WelcomeMsg			"Reloj de Joje\n=====\n0. Nueva hora\n1. Woriking on it...\n"

typedef enum
{
	Idle = 0,
	SendingData,
	ImgCalculation,
	ErrorState,
	TotalStates
}enMachineStates;

static uint8 u8MachineStatus = (uint8)ImgCalculation;
static uint32* pu32Time;

void Main_vSetFlags(void)
{
	assert(u8MachineStatus == (uint8)Idle);
	if(u8MachineStatus != Idle)
	{
		u8MachineStatus = (uint8)ErrorState;
	}
	else
	{
		u8MachineStatus = (uint8)SendingData;
		*pu32Time = *pu32Time+1;
	}
}


int main(void) {
//	uint8 u8Sec = 0;
	Comm_stMsgProps stMsg;

	stMsg.u8Protocol = (uint8)Comm_enViaUART;
	stMsg.s8Data = WelcomeMsg;
	stMsg.u8BusLenght = sizeof(WelcomeMsg);

	I2C_vDriverInit();

	pu32Time = Time_pu8GetRealTime();

	Comm_vInit();
	Comm_vSendData(&stMsg);
	OLEDAPI_vDispMenu();
	PIT_vfnSetPit(0, 1000, 1, Main_vSetFlags);
	PIT_vfnStartPit(0,1);

    while(1)
    {
    	Comm_vMonitor();
    	if(u8MachineStatus == (uint8)SendingData)
    	{
    		Clock_vDisplay();
    		u8MachineStatus = (uint8)ImgCalculation;
    	}
    	else if(u8MachineStatus == (uint8)ImgCalculation)
    	{
    		vCLKSHELL();
    		u8MachineStatus = (uint8)Idle;
    	}
    	else if(u8MachineStatus == (uint8)ErrorState)
    	{
    		PIT_vfnStartPit(0,0);
    		OLEDAPI_vInvDisplay();
    	}
    	else
    	{
    		/*Nothing to do. */
    	}

    }
    return 0 ;
}
