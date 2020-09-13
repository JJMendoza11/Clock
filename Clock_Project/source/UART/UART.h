/*
 * UART.h
 *
 *  Created on: 15 may. 2020
 *      Author: lucky
 */

#ifndef UART_H_
#define UART_H_

#define UART0_IRQ     UART0_IRQHandler

extern void UART_vInit(void);
extern uint8 UART_u8SendData(char* pu8Data, uint8 u8Size);
void UART_DisableRx(void);
void UART_EnableRx(void);



#endif /* UART_H_ */
