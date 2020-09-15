/*
 * OLED.h
 *
 *  Created on: 25 jul. 2020
 *      Author: lucky
 */

#ifndef OLED_H_
#define OLED_H_

#include "Prototype.h"

typedef enum
{
	OLED_enPage1 = 0,
	OLED_enPage2,
	OLED_enPage3,
	OLED_enPage4,
	OLED_enTotalPages
}OLED_enNumPag;

void OLED_vInit(void);

void OLED_vRefresh(void);

void OLED_vPartialRefresh(uint8 u8Page1, uint8 u8Page2, uint16 u16Pxl1, uint16 u16Pxl2);

void OLED_vClearScreen(void);

void OLED_vDrawRectangle(uint16 x, uint16 y, uint8 b, uint8 a);

void OLED_vFillRect(uint16 x, uint16 y, uint16 b, uint16 a, uint8 u8Color);

void OLED_vGotoxy(uint16 x, uint8 y);

void OLED_vSetSize(uint8 u8Size);

void OLED_vDrawHLine(uint16 x, uint16 y, uint16 l, uint8 u8Color);

void OLED_vDrawVLine(uint16 x, uint16 y, uint16 l, uint8 u8Color);

void OLED_vInverse(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1);

void OLED_vPrint(const uint8* C, uint16 u16Len);

void OLED_vInvDisplay(void);


#endif /* OLED_H_ */
