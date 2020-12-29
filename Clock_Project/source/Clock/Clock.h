/*
 * Clock.h
 *
 *  Created on: 13 sep. 2020
 *      Author: lucky
 */

#ifndef CLOCK_CLOCK_H_
#define CLOCK_CLOCK_H_

#include "Prototype.h"

#include "OLEDAPI_def.h"

typedef struct
{
	uint8 u8StringId;
	uint8 u8Padding;
	uint8 au8Msg[OLEDAPI_nString_Len];
}Clock_tstRxMsg;

void Clock_vMonitor(uint8 u8State);

void Clock_vDisplay(void);


#endif /* CLOCK_CLOCK_H_ */
