/*
 * Watch.h
 *
 *  Created on: 10 ago. 2020
 *      Author: lucky
 */

#ifndef WATCH_DVR_WATCH_H_
#define WATCH_DVR_WATCH_H_

#define OneSec 5
#define OneMin 60
#define OneHour 60
#define FullHour 24
#define ChronoSet		(1<<0)
#define AlarmSet		(1<<1)
#define TimerSet		(1<<2)
#define AlarmOn 		'A'
#define AlarmOff 		'O'
#define TimerOn			'T'
#define TimerOff		'D'

typedef enum{
	enIdle = 0,
	enSet_Seg,
	enSet_Min,
	enSet_Hrs,
	enSet_Day,
	enSet_Month,
	enSet_Year,
	enSet_Timer,
	enSet_Chrono,
	enConf
}tenClockStates;

void AddClock(void);

void Clock_vfnClock(void);

void Alarm_vfnClock(void);

void Timer_vfnClock(void);

void vfMin_Hour(void);

void Clock_vfnMasterClock (void);

void Clock_vfnInit(void);



#endif /* WATCH_DVR_WATCH_H_ */
