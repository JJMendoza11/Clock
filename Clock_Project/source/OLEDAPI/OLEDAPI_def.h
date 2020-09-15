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
	OLEDAPI_enSunday = 0,				/**< Equal to 0.					*/
	OLEDAPI_enMonday,					/**< Equal to 1.					*/
	OLEDAPI_enTuesday,					/**< Equal to 2.					*/
	OLEDAPI_enWednesday,				/**< Equal to 3.					*/
	OLEDAPI_enThursday,					/**< Equal to 4.					*/
	OLEDAPI_enFriday,					/**< Equal to 5.					*/
	OLEDAPI_enSaturday,					/**< Equal to 6.					*/
	OLEDAPI_enTotalDay					/**< Equal to 7 (Days of the week).	*/
}Clock_enWeekDays;

typedef enum
{
	OLEDAPI_enFullHr = 0,
	OLEDAPI_enHr,
	OLEDAPI_enColon,
	OLEDAPI_enFullMin,
	OLEDAPI_enMin,
	OLEDAPI_enTimeDigit
}Clock_enTimeIndex;

typedef enum
{
	OLEDAPI_enTimeString = 0,
	OLEDAPI_enDateString,
	OLEDAPI_enCurrDay,
	OLEDAPI_enSunString,
	OLEDAPI_enSatString,
	OLEDAPI_enTotalStrings
}OLEDAPI_enStrings;


#define Nd4Digits			(0U)
#define OneDigit 			(1U)
#define TwoDigit			(2U)

#define TwoDigitUnit						(2U)
#define LastDigitLoc						(4U)

#endif /* OLEDAPI_OLEDAPI_DEF_H_ */
