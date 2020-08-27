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
 *@fn void Clock_vInit(void)
 *@pre The component "COMPNAME" has to be initialized.
 *@brief Init the menu of the clock when it called for the first time. Create the "GUI"
 *@brief for the clock, asking for the TimeData to "COMPNAME".
 *@param void
 * @return void
 */
void Clock_vInit(void);

/**
 * @fn Clock_vSetTime(uint8 u8cLoc)
 * @brief the function receives the location of the char that needs to be change on the screen
 * this in order to avoid the refresh of all the string an loosing time calculating every char and
 * sending all the info. Display the time only, no the date just time.
 * @param u8cLoc location of the char in the time sting must be under the range of Clock_enTimeIndex enum.
 * @return void.
 */
void Clock_vSetTime(uint8 u8cLoc);

void Clock_vToggleSec(uint8 u8Toggle);

uint8 Clock_u8WriteDate(uint8 u8Id, uint8 u8Num, uint8* pu8Len);


/**
 * @fn uint8* Clock_pu8GetTimePointer(void)
 * @brief Get the direction of the strign to write the char.
 * @return Return the direction of the Time and Date string
 */
uint8* Clock_pu8GetTimePointer(void);

/**
 * @fn  Clock_vDispPags(uint8 u8TimeId, uint8 u8Len, uint8 u8Index)
 * @brief Indicate what numbers will change on the display must be a value in
 *  the TimeIndex enum or from the DateIndex enum.
 *  @param u8Index have to be and decreasing var to change the time.
 * u8Index = {Clock_enTimeIndex} or {Clock_enDateIndex}
 *  @param u8TimeId This var can go from [0,2) where 0 = Time and 1 = Date.
 */
void Clock_vDispPags(uint8 u8TimeId, uint8 u8Index);


#endif /* WATCH_DVR_WATCH_H_ */
