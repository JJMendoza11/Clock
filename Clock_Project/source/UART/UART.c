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
 * @file    UART.c
 * @brief   Application entry point.
 */
#include "Prototype.h"
/* TODO: insert other include files here. */
#include "MKL25Z4.h"
#include "UART_cfg.h"
#include "UART_def.h"
#include "UART.h"

#include "Comm.h"

/* TODO: insert other definitions and declarations here. */
#define UART_Baudrate     9600
#define UART0_IRQ     UART0_IRQHandler


/*
 * @brief   Application entry point.
 */

void UART_vInit(void)
{
	uint32 SBR;
	SBR=21000000/((16)*UART_Baudrate);               // {9600=21Mhz/((OSR+1)SBR)}  {SBR=21Mhz/((OSR+1)9600}
	SIM->SOPT2|=SIM_SOPT2_UART0SRC(1);
	SIM->SCGC4|=SIM_SCGC4_UART0_MASK;
	UART0->BDH|=UART_BDH_SBNS_MASK;         //Stop Bit Number Select
	UART0->BDH|=(SBR>>8 & 0X1F);            //Baud Rate Modulo Divisor (Se ingresa el bit 12,11,10,9,8)
	UART0->BDL=(SBR&0xFF);                  //Baud Rate Modulo Divisor(Se ingresan el bit 7,6,5,4,3,2,1,0)
	UART0->C1=0;

	UART0->C2|=UART_C2_RIE_MASK;
	NVIC->ISER[0]|=(1<<UART0_IRQn);
	UART0->C4|=UART0_C4_OSR(15);

	SIM->SCGC5|=SIM_SCGC5_PORTA_MASK;
	PORTA->PCR[1]=PORT_PCR_MUX(2); //RX
	PORTA->PCR[2]=PORT_PCR_MUX(2); //TX

	UART0->C2|=UART_C2_TE_MASK;
	UART0->C2|=UART_C2_RE_MASK;

}

uint8 UART_u8SendData(char* pu8Data, uint8 u8Size)
{
	uint8 u8i;
	for(u8i = 0; u8i<u8Size; u8i++)
	{
		while((UART0->S1&UART_S1_TC_MASK) == 0);
		UART0->D = pu8Data[u8i];
	}
	return 1;
}

void UART0_IRQ(void)
{
	uint8* pu8RecieveData;
	pu8RecieveData = Comm_u8GetPointer();
	*pu8RecieveData = UART0->D;
	Comm_vSetReadFlag();
}

void UART_DisableRx(void)
{
	UART0->C2 &= ~(UART_C2_RE_MASK);
}
void UART_EnableRx(void)
{
	UART0->C2 |= UART_C2_RE_MASK;
}
