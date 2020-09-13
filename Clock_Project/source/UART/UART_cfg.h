/*
 * UART_cfg.h
 *
 *  Created on: 15 may. 2020
 *      Author: lucky
 */

#ifndef UART_CFG_H_
#define UART_CFG_H_

#define UART_s_INIT_CLK				(uint32*)(0x40048004)
#define UART_s_CLK_ADDRESS			(uint32*)(0x40048034)
#define UART_s_ENABLE_CLK			(uint32)(1<<10)
#define UART_s_INIT_DEFAULT_CLK		(uint32)(1<<26)



#endif /* UART_CFG_H_ */
