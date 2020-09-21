/*
 * Comm_def.h
 *
 *  Created on: 15 may. 2020
 *      Author: lucky
 */

#ifndef COMM_DEF_H_
#define COMM_DEF_H_

#include "UART.h"


#define Comm_nMIN_BUS_LENGHT												  (2U)
#define Comm_nBUS_LENGHT								(5 + Comm_nMIN_BUS_LENGHT)
#define Comm_nNew_Msg									(2U)
#define Comm_s_fnDISABLE									{UART_DisableRx}
#define Comm_s_fnENABLE										{UART_EnableRx}


#endif /* COMM_DEF_H_ */
