/*
 * I2CDrive.h
 *
 *  Created on: 17 nov. 2019
 *      Author: lucky
 */

#ifndef I2CDRIVE_H_
#define I2CDRIVE_H_

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;

void I2C_vDriverInit(void);
void I2C_vfnSlaveInit(void);
uint8_t I2C_vfnSendData(uint8_t *u8Data, uint16_t SizeData);

#endif /* I2CDRIVE_H_ */
