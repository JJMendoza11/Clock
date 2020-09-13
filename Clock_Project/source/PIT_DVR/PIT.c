#include "MKL25Z4.h"
#include "PIT.h"
#include "core_cm0plus.h"
#include "Prototype.h"
tstInitPit stPitConfiguration[enTotalPit];

void Pit_vfnSetTime(uint8 u8channel, uint16 Time_in_ms);
void PIT_vfnTimerEnable(uint8 u8Channel);
void PIT_vfnTimerDisable(uint8 u8Channel);

static uint8 u8Setup=0;

void PIT_vfnSetPit( uint8 u8Channel, uint16 u16Period_ms, uint8 u8Interruption, void(*ptrfn)(void))
{
	if(u8Interruption)
		NVIC->ISER[0] |= (1<<PIT_IRQn);

	if(u8Setup == 0)
	{
		SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
		PIT->MCR |= (PIT_MCR_FRZ_MASK);
		PIT->MCR &= ~(PIT_MCR_MDIS_MASK);

		u8Setup = 1;
	}

	if(u8Interruption)
	{
		PIT->CHANNEL[u8Channel].TCTRL |= PIT_TCTRL_TIE_MASK;
		stPitConfiguration[u8Channel].u8Interup = u8Interruption;
	}
	stPitConfiguration[u8Channel].pfnInterruption = ptrfn;
	Pit_vfnSetTime(u8Channel, u16Period_ms);
}

void PIT_vfnTimerEnable(uint8 u8Channel){
	PIT->CHANNEL[u8Channel].TCTRL|=PIT_TCTRL_TEN_MASK;
}
void PIT_vfnTimerDisable(uint8 u8Channel){
	PIT->CHANNEL[u8Channel].TCTRL&=~(PIT_TCTRL_TEN_MASK);



}

void Pit_vfnSetTime(uint8 u8channel, uint16 Time_in_ms)
{
	uint32_t u32LDVal = (uint32)(10500 * Time_in_ms);
	PIT->CHANNEL[u8channel].LDVAL = u32LDVal;
}

/**
 * @param u8Channel: Indicated the channel to use for the PIT's clock
 * @param bSet: Start/Stop the PIT counter.
 *
 */

void PIT_vfnStartPit(uint8 u8Channel, uint8 u8Set)
{
	stPitConfiguration[u8Channel].u8PitState = u8Set;

	if(u8Set)
		PIT_vfnTimerEnable(u8Channel);
	else
		PIT_vfnTimerDisable(u8Channel);

}

void PIT_IRQ(void){
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK)
	{
		PIT->CHANNEL[0].TFLG |= PIT_TFLG_TIF_MASK;
		stPitConfiguration[0].pfnInterruption();
	}
	if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK)
	{
		PIT->CHANNEL[1].TFLG |= PIT_TFLG_TIF_MASK;
		stPitConfiguration[1].pfnInterruption();
	}
}
