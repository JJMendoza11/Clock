#include "MKL25Z4.h"
#include "PIT.h"
#include "core_cm0plus.h"

tstInitPit stPitConfiguration[enTotalPit];

void Pit_vfnSetTime(uint8 u8channel, uint16 Time_in_ms);
void PIT_vfnTimerEnable(uint8 u8Channel);
void PIT_vfnTimerDisable(uint8 u8Channel);

static _Bool bSetup=0;

void PIT_vfnSetPit( uint8 u8Channel, uint16 u16Period_ms, bool bInterruption )
{
	if(bInterruption)
		NVIC->ISER[0] |= (1<<PIT_IRQn);

	if(bSetup == 0)
	{
		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
		PIT->MCR &= ~(PIT_MCR_FRZ_MASK);
		PIT->MCR &= ~(PIT_MCR_MDIS_MASK);

		bSetup = 1;
	}

	if(bInterruption)
	{
		PIT->CHANNEL[u8Channel].TCTRL |= PIT_TCTRL_TIE_MASK;
		stPitConfiguration[u8Channel].bInterup = bInterruption;
	}
	stPitConfiguration[u8Channel].u8Period_ms = &Pit_vfnSetTime;
	stPitConfiguration[u8Channel].u8Period_ms (u8Channel, u16Period_ms);
}

void PIT_vfnTimerEnable(uint8 u8Channel){
	PIT->CHANNEL[u8Channel].TCTRL|=PIT_TCTRL_TEN_MASK;
}
void PIT_vfnTimerDisable(uint8 u8Channel){
	PIT->CHANNEL[u8Channel].TCTRL&=~(PIT_TCTRL_TEN_MASK);
}

void Pit_vfnSetTime(uint8 u8channel, uint16 Time_in_ms)
{
	uint32_t u32LDVal = 10500 * Time_in_ms;
	PIT->CHANNEL[u8channel].LDVAL = u32LDVal;
}

/**
 * @param u8Channel: Indicated the channel to use for the PIT's clock
 * @param bSet: Start/Stop the PIT counter.
 *
 */

void PIT_vfnStartPit(uint8 u8Channel, bool bSet)
{
	stPitConfiguration[u8Channel].bPitState = bSet;

	if(bSet)
		stPitConfiguration[u8Channel].bEnablePit = &PIT_vfnTimerEnable;
	else
		stPitConfiguration[u8Channel].bEnablePit = &PIT_vfnTimerDisable;

	stPitConfiguration[u8Channel].bEnablePit (u8Channel);

}

void PIT_IRQ(void){
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
	}
	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
	{
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
	}
}
