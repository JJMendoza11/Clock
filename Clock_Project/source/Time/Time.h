/*
 * Time.h
 *
 *  Created on: 21 ago. 2020
 *      Author: lucky
 */

#ifndef TIME_TIME_H_
#define TIME_TIME_H_

#include "Prototype.h"

typedef enum
{
	Time_enSecId = 0,
	Time_enMinId,
	Time_enHrId,
	Time_enDayId,
	Time_enTotalReqId
}Time_enAvailableIds;

uint8 Time_u8GetSeconds(void);
uint8 Time_u8GetMin(void);
uint8 Time_u8GetHrs(void);
uint8 Time_u8GetDay(void);
uint8 Time_u8GetReqTime(uint8 u8RequestId);
uint32* Time_pu8GetRealTime(void);

uint32 Time_u32GetRawTime(void);

void Time_vSetTime(uint32 u32Hrs, uint32 u32Min);

#endif /* TIME_TIME_H_ */
