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
#include "MKL25Z4.h"
#include "OLED.h"


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

//Public Functions
void Clock_vInit(void)
{
	OLED_vInit();
	SSD1306_ClearDisplay();

//	Todo Create a getfun for the date and time.


	Clock_vDisplayMenu();
	Clock_vSetFullDate();
	Clock_vSetDay();
	Clock_vSetFullTime();
	SSD1306_FullDisplay();
}

//Private Functions
void Clock_vDisplayMenu(void)
{
	SSD1306_DrawRect( 56, 0, 72, 11);
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




