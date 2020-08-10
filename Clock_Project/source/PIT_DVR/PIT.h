/*
 * PIT.h
 *
 *  Created on: 17/07/2019
 *      Author: Poncho
 */

#ifndef PIT_H_
#define PIT_H_

#define PIT_IRQ   PIT_DriverIRQHandler

typedef unsigned char uint8;
typedef unsigned short uint16;

#define bool _Bool

	typedef enum{
		enPit0 = 0,
		enPit1,
		enTotalPit
	}tenPits;

	typedef struct{
		void (*u8Period_ms)(uint8, uint16);
		bool bInterup;
		void  (*bEnablePit)(uint8);
		bool bPitState;
	}tstInitPit;

	void PIT_vfnSetPit(uint8 u8Channel, uint16 u16Period_ms, bool bInterruption);
	void PIT_vfnStartPit(uint8 u8Channel, bool bSet);
	void PIT_IRQ(void);




#endif /* 1_APP_PIT_H_ */
