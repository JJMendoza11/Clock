/*
 * Copyright 2016-2018 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
/**
 * @file    I2CDriver.c
 * @brief   Application entry point.
 */
#include "MKL25Z4.h"
/* TODO: insert other include files here. */

/* TODO: insert other definitions and declarations here. */

/*
 * @brief   Application entry point.
 */


void I2C_vDriverInit(void)
{

	/*PinOut Configuration*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	/*Enable the SCL pin*/
	PORTB->PCR[2] = 0x207;
	/*Enable the SDA pin*/
	PORTB->PCR[3] = 0x207;

	/*Enable I2C protocol*/
	SIM->SCGC4 |= SIM_SCGC4_I2C0_MASK;

	I2C0->F = 0x45;

	I2C0->FLT = 0;

	I2C0->C1 |= I2C_C1_IICEN_MASK;

}

void I2C_vfnSlaveInit(void){

	/*PinOut Configuration*/
	SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;
	/*Enable the SCL pin*/
	PORTB->PCR[2] = 0x207;
	/*Enable the SDA pin*/
	PORTB->PCR[3] = 0x207;

	SIM->SCGC4 |= SIM_SCGC4_I2C1_MASK;

	I2C1->A1 = 0x3C;

	I2C1->C1 |= I2C_C1_IICEN_MASK;

	I2C1->F = 0x45;

}


uint8_t I2C_vfnSendData (uint8_t *u8Data, uint16_t SizeData)
{
	uint8_t u8Status = 0;

	 /* Clear all status before transfer. */
	I2C0->S |= I2C_S_ARBL_MASK;
	I2C0->S |= I2C_S_IICIF_MASK;
	I2C0->FLT |= I2C_FLT_STOPF_MASK;

    /* Wait until ready to complete. */
    while (!(I2C0->S & I2C_S_TCF_MASK))
    {
    }

    /* For normal transfer, send start. */
    /* Return an error if the bus is already in use. */
    if (I2C0->S & I2C_S_BUSY_MASK)
    {
    	u8Status = 1;

    	return u8Status;
    }else
    {
        /* Send the START signal. */
        I2C0->C1 |= I2C_C1_MST_MASK | I2C_C1_TX_MASK;

        /*Send the Slave address. */
        I2C0->D = (*u8Data++) << 1 | 0;
    }

    while (!(I2C0->S & I2C_S_IICIF_MASK))
     {
     }

    /* Check if there's transfer error. */
	if(I2C0->S & I2C_S_ARBL_MASK){
		u8Status = 1;
		I2C0->S |= I2C_S_ARBL_MASK;
	}

	if(I2C0->S & I2C_S_RXAK_MASK){
		u8Status = 1;
		I2C0->S |= I2C_S_RXAK_MASK;
	    /* Issue the STOP command on the bus. */
	    I2C0->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
	}

    if (!u8Status)
    {
    	SizeData--;

		/* Wait until the data register is ready for transmit. */
		while (!(I2C0->S & I2C_S_TCF_MASK))
		{
		}

		I2C0->S = I2C_S_IICIF_MASK;

	    /* Setup the I2C peripheral to transmit data. */
	    I2C0->C1 |= I2C_C1_TX_MASK;

		while(SizeData--)
		{
			I2C0->D = *u8Data++;

			/* Wait until data transfer complete. */
			while (!(I2C0->S & I2C_S_IICIF_MASK))
			{
			}

			 /* Clear the IICIF flag. */
			 I2C0->S |= I2C_S_IICIF_MASK;

			 /* Check if arbitration lost or no acknowledgement (NAK), return failure status. */
			 if (I2C0->S & I2C_S_ARBL_MASK)
			 {
				 I2C0->S |= I2C_S_ARBL_MASK;
				 u8Status = 1;
				 return u8Status;
			 }

			 if ((I2C0->S & I2C_S_RXAK_MASK) && SizeData)
			 {
				 I2C0->S |= I2C_S_RXAK_MASK;
				 u8Status = 1;
			 }

			 if (u8Status)
			 {
				 /* Breaking out of the send loop. */
				 break;
			 }
		}

		/* Clear the IICIF flag. */
		I2C0->S |= I2C_S_IICIF_MASK;
		/* Issue the STOP command on the bus. */
		I2C0->C1 &= ~(I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_TXAK_MASK);
    }


	while ((I2C0->S & I2C_S_BUSY_MASK)){
	}

    return u8Status;
}

