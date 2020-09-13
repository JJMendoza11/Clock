/*
 * Comm.c
 *
 *  Created on: 15 may. 2020
 *      Author: lucky
 */

#include "Prototype.h"

#include "Comm_cfg.h"
#include "Comm_def.h"
#include "Comm.h"

#define Comm_s_CLEAN_FLG(x)				((uint8)~(1<<(4+x)))
#define Comm_s_SET_FLG(x)				((uint8)(1<<(4+x)))
#define Comm_s_nUART_ENABLE_FLG			(1<<4)
#define Comm_s_nI2C_ENABLE_FLG			(1<<5)
#define Comm_s_BUS_EMPTY				(0U)
#define Comm_s_NEW_MSG_ARRIVED			(1U)
#define Comm_s_END_COMM					(2U)
#define Comm_s_INTRO					(2U)
#define Comm_s_NEW_C					(0U)
#define Comm_s_DELETE_C					(1U)
#define Comm_s_nNEW_LINE				('\n')
#define Comm_s_nINIT_LINE				('\r')
#define Comm_s_nBACKSPACE				('\b')
#define Comm_s_nSPACE					(' ')

static uint8 au8MyWord[Comm_nBUS_LENGHT] = {0};
static uint8 u8Index = 0;
static volatile uint8 u8CommFlags = (uint8)Comm_s_BUS_EMPTY;
static const void (*pafnDisableComm[Comm_enTotalProtocols])(void) = Comm_s_fnDISABLE;
static const void (*pafnEnableComm[Comm_enTotalProtocols])(void) = Comm_s_fnENABLE;

void Comm_vCleanBuffer(void)
{
	uint8 i;
	for (i = 0; i <u8Index; i++)
	{
		au8MyWord[i] = 0;
	}
	u8Index = 0;
}

void Comm_vInit(void)
{

	UART_vInit();
	u8CommFlags = Comm_s_nUART_ENABLE_FLG;
}


uint8* Comm_u8GetPointer(void)
{
	return &au8MyWord[u8Index];
}

void Comm_vSetReadFlag(void)
{
	u8CommFlags |= (uint8)Comm_s_NEW_MSG_ARRIVED;
}

void Comm_SaveNewChar(void)
{
	uint8 u8BusSize = 1;
	uint8 u8Dir = Comm_s_NEW_C;
	if(au8MyWord[u8Index] == Comm_s_nINIT_LINE)
	{
		au8MyWord[u8Index+1] = Comm_s_nNEW_LINE;
		u8BusSize = 2;
		u8Dir = (uint8)Comm_s_INTRO;
	}
	if(au8MyWord[u8Index] == Comm_s_nBACKSPACE)
	{
		au8MyWord[u8Index+1] = Comm_s_nSPACE;
		au8MyWord[u8Index+2] = Comm_s_nBACKSPACE;
		u8BusSize = 3;
		u8Dir = Comm_s_DELETE_C;
	}
	if(u8Dir == 0 && u8Index < 5)
	{
		UART_u8SendData((char*)&au8MyWord[u8Index], u8BusSize);
		u8Index++;
	}
	else if(u8Dir == (uint8)Comm_s_DELETE_C && u8Index > 0)
	{
		UART_u8SendData((char*)&au8MyWord[u8Index], u8BusSize);
		u8Index--;
	}
	else if(u8Dir == (uint8)Comm_s_INTRO)
	{
		UART_u8SendData((char*)&au8MyWord[u8Index], u8BusSize);
		UART_DisableRx();
		u8CommFlags |= (uint8)Comm_s_END_COMM;
	}
	else
	{
		/* Nothing to do. */
	}
	u8CommFlags &= (uint8)~Comm_s_NEW_MSG_ARRIVED;
}


uint8 Comm_vSendData(Comm_stMsgProps* pstMsgProps)
{
	if(pstMsgProps->u8Protocol == Comm_enViaUART)
	{
		UART_u8SendData(pstMsgProps->s8Data, pstMsgProps->u8BusLenght);
	}
	else
	{
		/* Nothing to do. */
	}
	return 1;
}

uint8 Comm_u8GetMsg(uint8* pu8Data)
{
	uint8 i;
	uint8 u8Return = u8Index;

	Comm_vCleanBuffer();

	for(i = 0; i < u8Index; i++)
	{
		pu8Data[i] = (uint8)au8MyWord[i];
	}

	return u8Return;
}

uint8 Comm_u8GetStatus(void)
{
	return u8CommFlags;
}

void Comm_vDisableComm(uint8 u8Protocol)
{
	pafnDisableComm[u8Protocol]();
	u8CommFlags &= Comm_s_CLEAN_FLG(u8Protocol);
}

void Comm_vEnableComm(uint8 u8Protocol)
{
	Comm_vCleanBuffer();
	pafnEnableComm[u8Protocol]();
	u8CommFlags |= Comm_s_SET_FLG(u8Protocol);
}

void Comm_vCleanFlag(uint8 u8ClnFlag)
{
	u8CommFlags &= (uint8)~(1<<(u8ClnFlag-1));
}

void Comm_vMonitor(void)
{
	uint8 u8State;

	u8State = Comm_u8GetStatus();

	if (u8State & Comm_s_NEW_MSG_ARRIVED)
	{
		Comm_SaveNewChar();
	}
	else
	{
		/* Nothing to do. */
	}
}
