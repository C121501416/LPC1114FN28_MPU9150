/*
 * Project:		LPC1114_RS485Slave
 * File:		rs485.c
 * Author:		Terukazu Sato
 *
 */

#include <lpc11xx.h>
#include "rs485.h"
#include "lpc11xx_syscon.h"
#include "lpc11xx_uart.h"
#include "lpc11xx_gpio.h"
#include "lpc11xx_iocon.h"
#include "systick.h"
#include "xa_s.h"

void RS485_Config(uint8_t MyAddr, uint32_t baud)
{
	uint32_t dl;

	NVIC_DisableIRQ(UART_IRQn);

	LPC_IOCON->PIO1_5 &= ~0x07;	// 下位3ビットを0にする
	LPC_IOCON->PIO1_5 |=  0x01;	// ~RTSに選択
	LPC_IOCON->PIO1_6 &= ~0x07;	// 下位3ビットを0にする
	LPC_IOCON->PIO1_6 |=  0x01; // RXDに選択
	LPC_IOCON->PIO1_7 &= ~0x07;	// 下位3ビットを0にする
	LPC_IOCON->PIO1_7 |=  0x01; // TXDに選択

	LPC_SYSCON->SYSAHBCLKCTRL |= (1 << 12);	// UARTにクロックを供給する
	LPC_SYSCON->UARTCLKDIV = 1;

	LPC_UART->LCR = UART_LCR_WLEN8 |
					UART_LCR_PARITY_EN |	// パリティを9ビット目として利用する
					UART_LCR_DLAB_EN;

	dl = SystemCoreClock/(LPC_SYSCON->UARTCLKDIV*16*baud);
	LPC_UART->DLM = dl / 256;
	LPC_UART->DLL = dl % 256;
	LPC_UART->LCR &= ~UART_LCR_DLAB_EN;
	LPC_UART->FDR = UART_FDR_DIVADDVAL(0) | UART_FDR_MULVAL(1);

	LPC_UART->FCR = UART_FCR_FIFO_EN |
					UART_FCR_RX_RS |
					UART_FCR_TX_RS |
					UART_FCR_TRG_LEV0;

	LPC_UART->ADRMATCH = MyAddr;
	LPC_UART->RS485CTRL = 	UART_RS485CTRL_NMM_EN|
							UART_RS485CTRL_RX_DIS|
							UART_RS485CTRL_AADEN|
							UART_RS485CTRL_DCTRL_EN|
							UART_RS485CTRL_OINV_1;
}

void RS485_Init(uint8_t MyAddr)
{
    IOCON_SetPinFunc(IOCON_PIO1_6, PIO1_6_FUN_RXD);	/* UART RXD - PIO1_6 */
    IOCON_SetPinFunc(IOCON_PIO1_7, PIO1_7_FUN_TXD);	/* UART TXD - PIO1_7 */
	IOCON_SetPinFunc(IOCON_PIO1_5, PIO1_5_FUN_RTS);	/* UART RTS - PIO1_5 */
	IOCON_SetPinMode(IOCON_PIO1_5, PIN_MODE_PullDown);
	GPIO_SetDir(PORT1, GPIO_Pin_6, 0);	// RX
	GPIO_SetDir(PORT1, GPIO_Pin_7, 1);	// TX
	GPIO_SetDir(PORT1, GPIO_Pin_5, 1);	// DE(RTS)

	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_UART, ENABLE);
	LPC_UART->TER = 0;
	LPC_SYSCON->UARTCLKDIV = 1;
	/* uart configure */
	LPC_UART->LCR = UART_LCR_WLEN8|
					UART_LCR_PARITY_EN|
					UART_LCR_DLAB_EN;

#if BAUD == 1000000
	/* Baudrate prescaler */
	LPC_UART->DLM = DL>>8;
	LPC_UART->DLL = (DL&0xff);
	/* Baudrate prescaler access disable */
	LPC_UART->LCR &= ~UART_LCR_DLAB_EN;
	/* Baudrate prescaler2 */
	LPC_UART->FDR = ((MULVAL&0x0f)<<4) | (DIVADDVAL&0x0f);
	/* FIFO enable and reset */
#else
	uart_set_divisors(LPC_UART, BAUD);
#endif
	LPC_UART->FCR = UART_FCR_FIFO_EN|UART_FCR_RX_RS|UART_FCR_TX_RS;
	/* RS485 my address */
	LPC_UART->ADRMATCH = MyAddr;
	/* DE(driver enable) signal delay value */
	LPC_UART->RS485DLY = 10;
	/* RS485 Configure */
	LPC_UART->RS485CTRL = 	UART_RS485CTRL_NMM_EN|
							UART_RS485CTRL_RX_DIS|
							UART_RS485CTRL_AADEN|
							UART_RS485CTRL_DCTRL_EN|
							UART_RS485CTRL_OINV_1;

	/* uart enable */
	LPC_UART->TER = UART_TER_TXEN;

}
void RS485_SendData(uint8_t data)
{
	LPC_UART->LCR |= UART_LCR_PARITY_F_0;
	while(!(LPC_UART->LSR & UART_LSR_TEMT));
	LPC_UART->THR = data;
}
void RS485_SendAddr(uint8_t addr)
{
	LPC_UART->LCR &= 0xffcf;
	LPC_UART->LCR |= UART_LCR_PARITY_F_1;
	while(!(LPC_UART->LSR & UART_LSR_TEMT));
	LPC_UART->THR = addr;
}

int RS485_RecvData()
{
	int cnt = RS485_TIMEOUT;
	do {
		if(LPC_UART->LSR & UART_LSR_RDR)
			break;
		_delay_us(1);
		cnt--;
	} while (cnt);
	if(cnt)	return LPC_UART->RBR & UART_RBR_MASKBIT;
	else return NO_RESPONSE;
}

