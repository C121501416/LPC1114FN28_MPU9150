/*
 * Project:		LPC1114_RS485Slave
 * File:		rs485.h
 * Author:		Terukazu Sato
 *
 */

#ifndef __RS485_H__
#define __RS485_H__

#define BAUD		115200
#if (BAUD == 1000000)
#define DL			3
#define DIVADDVAL	0
#define MULVAL		1
#endif
/* Baud rate = PCLK/(16 * DL * (DIVADDVAL/MULVAL)) */

/* Function prototypes ------------------------------------------------------ */
void RS485_Init(uint8_t MyAddr);
void RS485_SendData(uint8_t data);
void RS485_SendAddr(uint8_t addr);
int RS485_RecvData(void);

#endif /* __RS485_H__ */
