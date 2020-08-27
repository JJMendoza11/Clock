/*
 * PIT.h
 *
 *  Created on: 17/07/2019
 *      Author: Poncho
 */

#ifndef PIT_H_
#define PIT_H_

#include "Prototype.h"

#define PIT_IRQ   PIT_DriverIRQHandler

	typedef enum{
		enPit0 = 0,
		enPit1,
		enTotalPit
	}tenPits;

	typedef struct{
		uint8 u8Interup;
		uint8 u8PitState;
		void (*pfnInterruption)();
	}tstInitPit;

	void PIT_vfnSetPit( uint8 u8Channel, uint16 u16Period_ms, uint8 u8Interruption, void(*ptrfn)(void));
	void PIT_vfnStartPit(uint8 u8Channel, uint8 u8Set);
	void PIT_IRQ(void);




#endif /* 1_APP_PIT_H_ */
