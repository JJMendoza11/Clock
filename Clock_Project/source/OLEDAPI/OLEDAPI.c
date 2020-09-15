/**
 *
 * Watch.c
 *
 * @file Watch.c
 * @brief Get the necessary info to show on the display
 *  Created on: 10 ago. 2020
 *
 *  @author: lucky
 */

#include "Prototype.h"
#include "MKL25Z4.h"

#include "OLEDAPI.h"
#include "OLEDAPI_cfg.h"
#include "OLEDAPI_def.h"


#include "OLED.h"


#define ASCIIZero							(48U)
#define xyTimeLoc(str,Loc)					Clock_astTimeProps[str].au8xLoc[Loc],  Clock_astTimeProps[str].u8yLoc
#define UnitsASCII(x)						(x % 10 + ASCIIZero)
#define DecASCII(x)							(x / 10 + ASCIIZero)

typedef struct
{
	uint8 au8xLoc[12];
	uint8 u8yLoc;
	uint8 u8Size;
	uint8 u8Len;
}Clock_stStrProps;

/**
 * @struct Watch_stStrData
 * @brief Contain all the information necessary to be displayed on the clock screen
 */
typedef struct
{
	uint8 au8Word[12];
	Clock_stStrProps stStrProp;
}Watch_stStrData;

//Private Variables

/**
 * @var Watch_stStrData Contain the actual data and time from the
 * "COMPNAME. This info is send to the OLED Driver so
 * you can see the time.
 */
static Watch_stStrData Clock_stWords[OLEDAPI_enTotalStrings] = {	{{"00:00"},{{35, 47, 59, 71, 83, 255, 255, 255, 255, 255, 255, 255},15,2,5}}, 				\
																	{{"01.Jan.00"}, {{0, 6, 12, 18, 24, 30, 36, 42, 48, 255, 255, 255},0,1,9}},					\
																	{{"M Tu W Th F"}, {{60, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},2,1,11}},	\
																	{{"Sunday"}, {{75, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},2,1,6}},			\
																	{{"Saturday"}, {{68, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255},2,1,8}} };
static uint8 Clock_u8WeekDay = OLEDAPI_enMonday;

void _vBasicMenu(void)
{
	OLED_vDrawRectangle(56, 0, 72, 11);
	OLED_vDrawHLine(0,12,128, 1);
}

void _vSetDay(void)
{
	switch (Clock_u8WeekDay)
	{
	case (uint8)OLEDAPI_enSunday:
		OLED_vFillRect(57,1,70,9,0);
		//SSD1306_DrawText(75, 2, "Sunday", 1);
		OLEDAPI_vPrint(3,0,Clock_stWords[3].stStrProp.u8Len);
		break;
	case (uint8)OLEDAPI_enMonday:
		//SSD1306_DrawText(60, 2, "M Tu W Th F", 1);
		OLEDAPI_vPrint(2,0,Clock_stWords[2].stStrProp.u8Len);
		OLED_vInverse (57, 1, 69, 9);				//Monday
	break;
	case (uint8)OLEDAPI_enTuesday:
		OLED_vInverse (57, 1, 69, 9);
		OLED_vInverse (69, 1, 84, 9);				//Tuesday
	break;
	case (uint8)OLEDAPI_enWednesday:
		OLED_vInverse (69, 1, 84, 9);
		OLED_vInverse (84, 1, 100, 9);				//Wednesday
	break;
	case (uint8)OLEDAPI_enThursday:
		OLED_vInverse (88, 1, 100, 9);
		OLED_vInverse (100, 1, 114, 9);				//Thursday
	break;
	case (uint8)OLEDAPI_enFriday:
		OLED_vInverse (100, 1, 114, 9);
		OLED_vInverse (114, 1, 126, 9);				//Friday
	break;
	default:
		OLED_vInverse (114, 1, 126, 9);
		//SSD1306_DrawText(68, 2, "Saturday", 1);
		OLEDAPI_vPrint(4,0,Clock_stWords[4].stStrProp.u8Len);
	break;
	}
}

void OLEDAPI_vPrint(uint8 u8SignalId, uint8 c, uint8 u8Len)
{
	uint8* pu8cLoc = (uint8*)&Clock_stWords[u8SignalId].au8Word;

	OLED_vSetSize(Clock_stWords[u8SignalId].stStrProp.u8Size);
	OLED_vGotoxy(Clock_stWords[u8SignalId].stStrProp.au8xLoc[c], Clock_stWords[u8SignalId].stStrProp.u8yLoc);
	OLED_vPrint(&pu8cLoc[c], u8Len);
}

void OLEDAPI_vDispMenu(void)
{
	OLED_vInit();
	OLED_vClearScreen();

	_vSetDay();

	OLEDAPI_vPrint(0,0,5);
	OLEDAPI_vPrint(1,0,9);

	_vBasicMenu();

	OLED_vRefresh();
}

uint8 OLEDAPI_vSetTime(uint8 u8Time, uint8 u8Value)
{
	uint8 u8Len = 1;
	uint8 u8Loc =  ((uint8)LastDigitLoc + u8Time)-(u8Time * 5) + u8Time;

	if(u8Value == 0)
	{
		Clock_stWords[OLEDAPI_enTimeString].au8Word[u8Loc] = '0';
		u8Loc--;
		Clock_stWords[OLEDAPI_enTimeString].au8Word[u8Loc] = '0';
		u8Len = 2;
	}
	else
	{
		Clock_stWords[OLEDAPI_enTimeString].au8Word[u8Loc] = UnitsASCII(u8Value);

		if(Clock_stWords[OLEDAPI_enTimeString].au8Word[u8Loc] == '0')
		{
			u8Loc--;
			Clock_stWords[OLEDAPI_enTimeString].au8Word[u8Loc] = DecASCII(u8Value);
			u8Len = 2;
		}
		else
		{
			/* Nothing to do */
		}
	}

	OLEDAPI_vPrint((uint8)OLEDAPI_enTimeString,u8Loc,u8Len);

	return u8Len;
}

void OLEDAPI_vDispTime(uint8 u8TimeId, uint8 u8Index)
{
	uint8 u8Pag1;
	uint8 u8Pag2;
	uint8 u8FrstPx;
	uint8 u8SecPx;
	uint8 u8cWidth;

	u8Pag1 = (uint8)(Clock_stWords[u8TimeId].stStrProp.u8yLoc / (uint8)8);
	u8Pag2 = u8Pag1 + Clock_stWords[u8TimeId].stStrProp.u8Size;

	u8FrstPx = Clock_stWords[u8TimeId].stStrProp.au8xLoc[u8Index];
	u8cWidth = 6 * Clock_stWords[u8TimeId].stStrProp.u8Size ;
	u8SecPx = u8cWidth + u8FrstPx - 1;

	OLED_vPartialRefresh(u8Pag1, u8Pag2, u8FrstPx, u8SecPx);
}

void OLEDAPI_vSetWeekDay(uint8 u8WeekDay)
{
	Clock_u8WeekDay = u8WeekDay;
	_vSetDay();
}

void OLEDAPI_vToggleSec(uint8 u8Tiks)
{
	if(u8Tiks == (uint8)True)
	{
		Clock_stWords[OLEDAPI_enTimeString].au8Word[OLEDAPI_enColon] = ':';
	}
	else
	{
		Clock_stWords[OLEDAPI_enTimeString].au8Word[OLEDAPI_enColon] = ' ';
	}

	OLEDAPI_vPrint((uint8)OLEDAPI_enTimeString,(uint8)OLEDAPI_enColon,1);
}

void OLEDAPI_vInvDisplay(void)
{
	OLED_vInvDisplay();
}
