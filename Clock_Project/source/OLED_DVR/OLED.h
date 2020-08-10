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
void SSD1306_FullDisplay(void);
void SSD1306_ShrtDisplay(uint8 u8P1, uint8 u8P2, uint8 u8Px1, uint8 u8Px2);
void SSD1306_ClearDisplay(void);
void SSD1306_DrawPixel(uint8_t x, uint8_t y, _Bool color);
void SSD1306_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, _Bool color);
void SSD1306_DrawFastHLine(uint8_t x, uint8_t y, uint8_t w, _Bool color);
void SSD1306_DrawFastVLine(uint8_t x, uint8_t y, uint8_t h, _Bool color);
void SSD1306_FillRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h, _Bool color);
void SSD1306_DrawCircle(int16_t x0, int16_t y0, int16_t r);
void SSD1306_DrawRect(uint8_t x, uint8_t y, uint8_t w, uint8_t h);
void SSD1306_DrawTriangle(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);
void SSD1306_GotoXY(uint8_t x, uint8_t y);
void SSD1306_TextSize(uint8_t t_size);
void SSD1306_Print(uint8_t c);
void SSD1306_DrawText(uint8_t x, uint8_t y, char *_text, uint8_t size);
void SSD1306_ROMBMP(uint8_t x, uint8_t y, uint8_t *bitmap, uint8_t w, uint8_t h);
void SSD1306_INVERSE(uint8_t x, uint8_t y, uint8_t x0, uint8_t y0);

void SSD1306_StopScroll(void);

#endif /* OLED_H_ */
