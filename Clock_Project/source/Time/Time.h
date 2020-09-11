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
	Time_enTotalReqId
}Time_enAvailableIds;

uint8 Time_u8Monitor(void);
uint32* Time_pu8GetRealTime(void);

#endif /* TIME_TIME_H_ */
