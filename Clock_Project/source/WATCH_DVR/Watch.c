/*
 * Watch.c
 *
 *  Created on: 10 ago. 2020
 *      Author: lucky
 */

#include "Watch.h"
#include "MKL25Z4.h"
#include "OLED_Screen_Driver.h"

#define NULL ((void *)0)

tenClockStates ClockStates = enIdle;
uint8_t u8Seg = 50;
uint8_t u8Min = 59;
uint8_t u8Hrs = 23;
uint8_t u8Day = 31;
uint8_t u8Week_Day = 1;
uint8_t u8Month = 1;
uint8_t u8Year = 20;
uint8_t u8LastDayMonth[13] ={29, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30 , 31};
char u8Time [] = {"  :  "};
char u8Date [] = {"DD/MMM/AA"};

void vfnIdle (void);

void Clock_vfnDisplayMenu (void);

void Clock_vfnSet_Seg (void);

void Clock_vfnSet_Min (void);

void Clock_vfnSet_Hrs (void);

void Clock_vfnSet_Day (void);

void Clock_vfnSetDay (void);

void Clock_vfnSetMonth (void);

void Clock_vfnSet_Year(void);

void Clock_vfnTimerIcon (void);

void Clock_vfnAlarmIcon (void);



void (*Clock_ptrfn[]) (void) = {vfnIdle, Clock_vfnSet_Seg, Clock_vfnSet_Min, Clock_vfnSet_Hrs,
		Clock_vfnSet_Day, Clock_vfnSetMonth, Clock_vfnSet_Year};
void vfnIdle (void)
{

}
void Clock_vfnInit(void)
{
	SSD1306_Begin();

	SSD1306_ClearDisplay();

	SSD1306_Display();

	Clock_vfnDisplayMenu();
	Clock_vfnSet_Year();
	Clock_vfnSetMonth();
	Clock_vfnSet_Day();
	Clock_vfnSet_Hrs();
	Clock_vfnSet_Min();
	Clock_vfnSet_Seg();
}

void Clock_vfnMasterClock (void)
{
	for(; ClockStates != 0; ClockStates--)
	{
		Clock_ptrfn[ClockStates]();
	}

	ClockStates = enIdle;

	Clock_vfnTimerIcon();

	Clock_vfnAlarmIcon();

}


void Clock_vfnClock(void){
	u8Seg++;
	ClockStates = enSet_Seg;
	if(u8Seg == OneMin){
		u8Seg = 0;
		u8Min++;
		ClockStates = enSet_Min;
		if(u8Min == OneHour){
			u8Min = 0;
			u8Hrs++;
			ClockStates = enSet_Hrs;
			if(u8Hrs == FullHour){
				u8Hrs = 0;
				u8Day++;
				u8Week_Day++;
				if(u8Week_Day == 7){
					u8Week_Day = 0;
				}
				ClockStates = enSet_Day;
				if(u8Day > u8LastDayMonth[u8Month] && !(u8Year%4==0  && u8Day == u8LastDayMonth[0] )){
					u8Day = 1;
					u8Month++;
					ClockStates = enSet_Month;
					if(u8Month > 12){
						u8Month = 0;
						u8Year++;
						if(u8Year > 99){
							u8Year = 0;
						}
						ClockStates = enSet_Year;
					}
				}
			}
		}
	}

}



void Clock_vfnDisplayMenu(void)
{
	SSD1306_DrawRect( 56, 0, 72, 11);

//	Clock_vfnSetDay();

	SSD1306_DrawFastHLine(0,12,128, 1);
}

void Clock_vfnSet_Seg(void)
{
	if(u8Seg%2== 0)
	{
		SSD1306_INVERSE (104, 24, 127, 31);
	}

	SSD1306_Display();

//	ClockStates = enIdle;

//	Clock_ptrfn = NULL;

}

void Clock_vfnSet_Min(void)
{
	  u8Time[4] = u8Min % 10 + 48;
	  u8Time[3] = u8Min / 10 + 48;

	  SSD1306_DrawText(35, 15, u8Time, 2);

	  ClockStates = enSet_Seg;

//	  Clock_ptrfn = Clock_vfnSet_Seg;

//	  Clock_ptrfn();
}

void Clock_vfnSet_Hrs(void)
{
	  u8Time[1] = u8Hrs % 10 + 48;
	  u8Time[0] = u8Hrs / 10 + 48;

	SSD1306_DrawText(105, 24, "p.m.", 1);

//	  ClockStates = enSet_Min;

//	  Clock_ptrfn = Clock_vfnSet_Min;

//	  Clock_ptrfn();
}

void Clock_vfnSet_Day (void)
{
	  u8Date[1] = u8Day % 10 + 48;
	  u8Date[0] = u8Day / 10 + 48;

	  Clock_vfnSetDay();

	  SSD1306_DrawText(0, 2, u8Date, 1);

//	  Clock_ptrfn = Clock_vfnSet_Hrs;

//	  Clock_ptrfn();
}


void Clock_vfnSetDay(void)
{
	switch (u8Week_Day)
	{
	case 0:
		SSD1306_FillRect(57,1,70,9,0);
		SSD1306_DrawText(75, 2, "Sunday", 1);
		break;
	case 1:
		SSD1306_DrawText(60, 2, "M Tu W Th F", 1);
		SSD1306_INVERSE (57, 1, 69, 9);					//Monday
	break;
	case 2:
		SSD1306_INVERSE (57, 1, 69, 9);
		SSD1306_INVERSE (69, 1, 84, 9);					//Tuesday
	break;
	case 3:
		SSD1306_INVERSE (69, 1, 84, 9);
		SSD1306_INVERSE (84, 1, 100, 9);				//Wednesday
	break;
	case 4:
		SSD1306_INVERSE (88, 1, 100, 9);
		SSD1306_INVERSE (100, 1, 114, 9);				//Thursday
	break;
	case 5:
		SSD1306_INVERSE (100, 1, 114, 9);
		SSD1306_INVERSE (114, 1, 126, 9);				//Friday
	break;
	default:
		SSD1306_INVERSE (114, 1, 126, 9);
		SSD1306_DrawText(68, 2, "Saturday", 1);
	break;
	}
}

void Clock_vfnSetMonth(void)
{
	switch (u8Month){
	case 1:
		  u8Date[3] = 'J';
		  u8Date[4] = 'a';
		  u8Date[5] = 'n';
		  break;
	case 2:
		  u8Date[3] = 'F';
		  u8Date[4] = 'e';
		  u8Date[5] = 'b';
		  break;
	case 3:
		  u8Date[3] = 'M';
		  u8Date[4] = 'a';
		  u8Date[5] = 'r';
		  break;
	case 4:
		  u8Date[3] = 'A';
		  u8Date[4] = 'p';
		  u8Date[5] = 'r';
		  break;
	case 5:
		  u8Date[3] = 'M';
		  u8Date[4] = 'a';
		  u8Date[5] = 'y';
		  break;
	case 6:
		  u8Date[3] = 'J';
		  u8Date[4] = 'u';
		  u8Date[5] = 'n';
		  break;
	case 7:
		  u8Date[3] = 'J';
		  u8Date[4] = 'u';
		  u8Date[5] = 'l';
		  break;
	case 8:
		  u8Date[3] = 'A';
		  u8Date[4] = 'u';
		  u8Date[5] = 'g';
		  break;
	case 9:
		  u8Date[3] = 'S';
		  u8Date[4] = 'e';
		  u8Date[5] = 'p';
		  break;
	case 10:
		  u8Date[3] = 'O';
		  u8Date[4] = 'c';
		  u8Date[5] = 't';
		  break;
	case 11:
		  u8Date[3] = 'N';
		  u8Date[4] = 'o';
		  u8Date[5] = 'v';
		  break;
	case 12:
		  u8Date[3] = 'D';
		  u8Date[4] = 'e';
		  u8Date[5] = 'c';
		  break;
	default:
		  u8Date[3] = 'J';
		  u8Date[4] = 'a';
		  u8Date[5] = 'n';
		  break;
	}
}

void Clock_vfnSet_Year(void)
{

	  u8Date[8] = u8Year % 10 + 48;
	  u8Date[7] = u8Year / 10 + 48;

//	  Clock_ptrfn = Clock_vfnSetMonth;

//	  Clock_ptrfn();
}

void Clock_vfnTimerIcon(void)
{
	SSD1306_DrawTriangle( 18, 17, 24, 17, 21, 21);
	SSD1306_DrawTriangle( 18, 24, 24, 24, 21, 21);
}

void Clock_vfnAlarmIcon(void)
{
	SSD1306_DrawCircle(5, 21, 3);

	SSD1306_DrawPixel(2, 17, 1);
	SSD1306_DrawPixel(1, 18, 1);

	SSD1306_DrawPixel(8, 17, 1);
	SSD1306_DrawPixel(9, 18, 1);
}

