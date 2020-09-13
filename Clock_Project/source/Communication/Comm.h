/*
 * Comm.h
 *
 *  Created on: 15 may. 2020
 *      Author: lucky
 */

#ifndef COMM_H_
#define COMM_H_

typedef struct
{
	uint8 u8Protocol;
	char* s8Data;
	uint8 u8BusLenght;
	uint8 u8xPos;
	uint8 u8yPos;
	uint8 u8LetterSize;
}Comm_stMsgProps;

void Comm_vInit(void);
uint8* Comm_u8GetPointer(void);
void Comm_vSetReadFlag(void);
void Comm_SaveNewChar(void);
uint8 Comm_vSendData(Comm_stMsgProps* pstMsgProps);
uint8 Comm_u8GetMsg(uint8* pu8Data);
void Comm_vDisableComm(uint8 u8Protocol);
void Comm_vEnableComm(uint8 u8Protocol);
uint8 Comm_u8GetStatus(void);
void Comm_vCleanFlag(uint8 u8ClnFlag);

#endif /* COMM_H_ */
