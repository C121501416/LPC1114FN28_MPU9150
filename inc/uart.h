/*
 * Project:		LPC1114_RS485Slave
 * File:		uart.h
 * Author:		Terukazu Sato
 *
 */

#ifndef __UART_H__
#define __UART_H__

#include "lpc11xx.h"
#include "lpc11xx_uart.h"
#include "lpc11xx_syscon.h"
#include "lpc11xx_gpio.h"

/* Function prototypes ------------------------------------------------------ */
void SCI_Init(int baud);
void SCI_PutChar(uint8_t c);
uint8_t SCI_GetChar();
int SCI_ScanInt();

#endif /* __UART_H__ */
