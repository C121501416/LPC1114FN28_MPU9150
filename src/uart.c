/*
 * Project:		LPC1114_RS485Slave
 * File:		uart.c
 * Author:		Terukazu Sato
 *
 */


#include "uart.h"


void SCI_Init(int baud)
{
	UART_PinsInit();
	GPIO_SetDir(PORT1, GPIO_Pin_6, 0);
	GPIO_SetDir(PORT1, GPIO_Pin_7, 1);

	UART_CFG_Type UART_InitStruct;
	UART_ConfigStructInit(&UART_InitStruct);
	UART_InitStruct.Baud_rate = baud;
	UART_Init(LPC_UART, &UART_InitStruct);
	UART_TxCmd(LPC_UART, ENABLE);
}

void SCI_PutChar(uint8_t c)
{
	while(!(LPC_UART->LSR & UART_LSR_TEMT));
	LPC_UART->THR = c;
}
uint8_t SCI_GetChar()
{
	while(!(LPC_UART->LSR & UART_LSR_RDR));
	return LPC_UART->RBR;
}
/*
static int pow(int base, int n)
{
	int i, p = 1;
	if(p == 0) return 1;
	for (i = 1; i <= n; ++i) p = p * base;
	return p;
}
int SCI_ScanInt()
{
	int i, j, value = 0;
	char line[17];
	for(i=0;i<16;i++) {
		line[i] = SCI_GetChar();
		SCI_PutChar(line[i]);
		if(line[i] == 0x0a || line[i] == 0x0d) break;
	}
	line[i+1] = '\0';
	if (line[0]=='-') {
		for(j=1;j<i;j++) {
			value += (line[j]-0x30)*pow(10,i-j-1);
		}
		value = -value;
	} else {
		for(j=0;j<i;j++) {
			value += (line[j]-0x30)*pow(10,i-j-1);
		}
	}
	return value;
}
*/

