/**
 * I2CDrive.h
 *
 *@file I2CDrive.h
 *@brief It contain the public function that can be called.
 *
 *  Created on: 10 ago. 2020
 *  @author: lucky
 */

#ifndef I2CDRIVE_H_
#define I2CDRIVE_H_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;


/**
 * @fn I2C_vDriverInit(void)
 * @brief This function initialize the I2C peripheral as master. The BW it set for 100m/s.
 * @param void
 * @return void
 */
void I2C_vDriverInit(void);
/**
 * @fn I2C_vfnSlaveInit(void)
 * @brief This function set the kl as slave.
 * @param void
 * @return void
 */
void I2C_vfnSlaveInit(void);

/**
 * @fn I2C_vfnSendData(uint8_t *u8Data, uint16_t SizeData)
 * @brief  The function receive a pointer to the data it has to send.
 * @param u8Data: Pointer to the message to send.
 * @param SizeData: Specifie the length of the buffer.
 * @return if the message was send correctly or not.
 */
uint8_t I2C_vfnSendData(uint8_t *u8Data, uint16_t SizeData);

#endif /* I2CDRIVE_H_ */
