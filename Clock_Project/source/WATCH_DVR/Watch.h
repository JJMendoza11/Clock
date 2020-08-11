/**
 * I2CDrive.h
 *
 *@file Watch.h
 *@brief Contain the public function that can be called.
 *
 *  Created on: 10 ago. 2020
 *  @author: lucky
 */

#ifndef WATCH_DVR_WATCH_H_
#define WATCH_DVR_WATCH_H_

/**
 * @struct Watch_stStrData
 * @brief Contain all the information necessary to be displayed on the clock screen
 */
typedef struct
{
	uint8 sTime[6];					/**< Time Contain the time in hr:sec. This var is a string */
	uint8 sDate[10];				/**< Contain the actual date with the format: "DD.MMM.YY". This var is a string */
	uint8 u8WeekDay;				/**< Contain the actual day from [0,6]. Where 0 = Sunday and 6 = Saturday. */
}Watch_stStrData;


/**
 * @enum Clock_enWeekDays
 * @brief Contain the day of the week starting by Sunday.
 */
typedef enum
{
	Clock_enSunday = 0,				/**< Equal to 0.					*/
	Clock_enMonday,					/**< Equal to 1.					*/
	Clock_enTuesday,				/**< Equal to 2.					*/
	Clock_enWednesday,				/**< Equal to 3.					*/
	Clock_enThursday,				/**< Equal to 4.					*/
	Clock_enFriday,					/**< Equal to 5.					*/
	Clock_enSaturday,				/**< Equal to 6.					*/
	Clock_enTotalDay				/**< Equal to 7 (Days of the week).	*/
}Clock_enWeekDays;

/**
 *@fn void Clock_vInit(void)
 *@pre The component "COMPNAME" has to be initialized.
 *@brief Init the menu of the clock when it called for the first time. Create the "GUI"
 *@brief for the clock, asking for the TimeData to "COMPNAME".
 *@param void
 * @return void
 */
void Clock_vInit(void);



#endif /* WATCH_DVR_WATCH_H_ */
