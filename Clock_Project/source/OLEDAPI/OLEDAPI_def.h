/*
 * Wathc_def.h
 *
 *  Created on: 21 ago. 2020
 *      Author: lucky
 */

#ifndef OLEDAPI_OLEDAPI_DEF_H_
#define OLEDAPI_OLEDAPI_DEF_H_


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

typedef enum
{
	Clock_enFullHr = 0,
	Clock_enHr,
	Clock_enColon,
	Clock_enFullMin,
	Clock_enMin,
	Clock_enTimeDigit
}Clock_enTimeIndex;

#define Nd4Digits			(0U)
#define OneDigit 			(1U)
#define TwoDigit			(2U)

#define TwoDigitUnit						(2U)
#define LastDigitLoc						(4U)

#endif /* OLEDAPI_OLEDAPI_DEF_H_ */
