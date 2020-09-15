/**
 * I2CDrive.h
 *
 *@file Watch.h
 *@brief Contain the public function that can be called.
 *
 *  Created on: 10 ago. 2020
 *  @author: lucky
 */

#ifndef OLEDAPI_OLEDAPI_H_
#define OLEDAPI_OLEDAPI_H_

#include "Prototype.h"

void OLEDAPI_vPrint(uint8 u8SignalId, uint8 c, uint8 u8Len);

void OLEDAPI_vDispMenu(void);

uint8 OLEDAPI_vSetTime(uint8 u8Time, uint8 u8Value);

void OLEDAPI_vDispTime(uint8 u8TimeId, uint8 u8Index);

void OLEDAPI_vSetWeekDay(uint8 u8WeekDay);

void OLEDAPI_vToggleSec(uint8 u8Tiks);

void OLEDAPI_vInvDisplay(void);

#endif /* OLEDAPI_OLEDAPI_H_ */
