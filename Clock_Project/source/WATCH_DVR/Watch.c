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

#include "Watch.h"
#include "Watch_def.h"
#include "MKL25Z4.h"
#include "OLED.h"


#define ASCIIZero							(48U)
#define xyTimeLoc(str,Loc)					Clock_astTimeProps[str].au8xLoc[Loc],  Clock_astTimeProps[str].u8yLoc
#define ColonProps(str)						xyTimeLoc(0,Clock_enColon), (char*)&str, 2
#define UnitsASCII(x)						(x % 10 + ASCIIZero)
#define DecASCII(x)							(x / 10 + ASCIIZero)

typedef struct
{
	uint8 au8xLoc[10];
	uint8 u8yLoc;
	uint8 u8Size;
}Clock_stStrProps;


/**
 * @fn void Clock_vDisplayMenu(void)
 * @brief Create the graphics to display the time
 * First create an horizontal line with the length
 * of the screen at the height of 12 from top to butter
 * @param void
 * @return void
 */
void Clock_vDisplayMenu(void);

/**
 * @fn void Clock_vSetDay (void)
 * @brief Mark the current day of the week depending on the value of
 * u8WeekDay
 * @param void
 * @return void
 */
void Clock_vSetDay (void);

/**
 * @fn void Clock_vSetFullDate(void)
 * @brief Write the current date on the screen. The data is
 * saved on Clock_stDate.sDate
 * @param void
 * @return void
 */
void Clock_vSetFullDate(void);

/**
 * fn void Clock_vSetFullTime(void)
 * @brief Write the current time on the screen. The data is
 * saved on Clock_stData.sTime
 * @param void
 * @return void
 */
void Clock_vSetFullTime(void);



//Private Variables

/**
 * @var Watch_stStrData Contain the actual data and time from the
 * "COMPNAME. This info is send to the OLED Driver so
 * you can see the time.
 */
static Watch_stStrData Clock_stDate = {"00:00","01.Jan.00",1};

//Todo make a macro to save the props of the strings wich are displayed on the screen.
static const Clock_stStrProps Clock_astTimeProps[2] = {{{35, 47, 59, 71, 83, 0, 0, 0, 0, 0},15,2},
										  {{0, 6, 12, 18, 24, 30, 36, 42, 48, 0},0,1}};

//Public Functions
void Clock_vInit(void)
{
	OLED_vInit();
	SSD1306_ClearDisplay();


	Clock_vDisplayMenu();
	Clock_vSetFullDate();
	Clock_vSetDay();
	Clock_vSetFullTime();
	SSD1306_FullDisplay();
}

void Clock_vSetTime(uint8 u8cLoc)
{
	SSD1306_DrawText(xyTimeLoc(0, u8cLoc), (char*)&Clock_stDate.sTime[u8cLoc], 2);
}


void Clock_vDispPags(uint8 u8TimeId, uint8 u8Index)
{
	uint8 u8Pag1;
	uint8 u8Pag2;
	uint8 u8FrstPx;
	uint8 u8SecPx;
	uint8 u8Len;
	uint8 u8Aux;

	if(u8TimeId == 0)
	{
		u8Len = (uint8)Clock_enTimeDigit;
	}
	else
	{
		u8Len = 1;		//Todo, change this condition.
	}

	u8Pag1 = (uint8)(Clock_astTimeProps[u8TimeId].u8yLoc / (uint8)OLED_PAG_LENGTH);
	u8Pag2 = u8Pag1 + Clock_astTimeProps[u8TimeId].u8Size;
	u8Aux = 6 * Clock_astTimeProps[u8TimeId].u8Size;

	for(; u8Index < u8Len; u8Index++)
	{
		u8FrstPx = Clock_astTimeProps[u8TimeId].au8xLoc[u8Index];
		u8SecPx = u8Aux - 1 + u8FrstPx;

		SSD1306_ShrtDisplay(u8Pag1, u8Pag2, u8FrstPx, u8SecPx);
	}
}

uint8* Clock_pu8GetTimePointer(void)
{
	return (uint8*)&Clock_stDate;
}

void Clock_vToggleSec(uint8 u8Toggle)
{
	uint8 s[2] = "";

	if(u8Toggle == (uint8)True)
	{
		s[0] = ' ';
	}
	else
	{
		s[0] = ':';
	}

	SSD1306_DrawText(ColonProps(s));
}

uint8 Clock_u8WriteDate(uint8 u8Id, uint8 u8Num, uint8* pu8Len)
{
	uint8 u8RetVal = (uint8)N_OK;
	uint8 u8Aux = ((uint8)LastDigitLoc+u8Id)-(u8Id*5)+u8Id;

	if(u8Num == 0)
	{
		Clock_stDate.sTime[u8Aux] = '0';
		u8Aux--;
		Clock_stDate.sTime[u8Aux] = '0';

		if(u8Id == 0)
		{
			*pu8Len = *pu8Len + 3;
		}
		else
		{
			*pu8Len = *pu8Len + 2;
		}

		u8RetVal = (uint8)OK;

	}
	else
	{
		Clock_stDate.sTime[u8Aux] = UnitsASCII(u8Num);

		if(Clock_stDate.sTime[u8Aux] == '0')
		{
			u8Aux--;
			Clock_stDate.sTime[u8Aux] = DecASCII(u8Num);

			*pu8Len = *pu8Len + 2;
		}
		else
		{
			*pu8Len = *pu8Len + 1;
		}
	}

	Clock_vSetTime(u8Aux);

	return u8RetVal;
}
//Private Functions
void Clock_vDisplayMenu(void)
{
	SSD1306_DrawRect(56, 0, 72, 11);
	SSD1306_DrawFastHLine(0,12,128, 1);
}

void Clock_vSetFullDate(void)
{
	SSD1306_DrawText(0, 0, (char*)&Clock_stDate.sDate, 1);
}

void Clock_vSetFullTime(void)
{
	SSD1306_DrawText(35, 15, (char*)&Clock_stDate.sTime, 2);
}

void Clock_vSetDay(void)
{
	switch (Clock_stDate.u8WeekDay)
	{
	case (uint8)Clock_enSunday:
		SSD1306_FillRect(57,1,70,9,0);
		SSD1306_DrawText(75, 2, "Sunday", 1);
		break;
	case (uint8)Clock_enMonday:
		SSD1306_DrawText(60, 2, "M Tu W Th F", 1);
		SSD1306_INVERSE (57, 1, 69, 9);					//Monday
	break;
	case (uint8)Clock_enTuesday:
		SSD1306_INVERSE (57, 1, 69, 9);
		SSD1306_INVERSE (69, 1, 84, 9);					//Tuesday
	break;
	case (uint8)Clock_enWednesday:
		SSD1306_INVERSE (69, 1, 84, 9);
		SSD1306_INVERSE (84, 1, 100, 9);				//Wednesday
	break;
	case (uint8)Clock_enThursday:
		SSD1306_INVERSE (88, 1, 100, 9);
		SSD1306_INVERSE (100, 1, 114, 9);				//Thursday
	break;
	case (uint8)Clock_enFriday:
		SSD1306_INVERSE (100, 1, 114, 9);
		SSD1306_INVERSE (114, 1, 126, 9);				//Friday
	break;
	default:
		SSD1306_INVERSE (114, 1, 126, 9);
		SSD1306_DrawText(68, 2, "Saturday", 1);
	break;
	}
}
