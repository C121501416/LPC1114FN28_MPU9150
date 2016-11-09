/*
 * **************************************************************************
 * 	@file		spi.c
 * 	@author		T.Sato
 * 	@version	1.00
 * 	@date		2014/1/11
 * **************************************************************************
 */
#include "lpc11xx.h"
#include "lpc11xx_ssp.h"
#include "lpc11xx_gpio.h"
#include "lpc11xx_syscon.h"
#include "spi.h"

void SPI_Init(int freq)
{
	GPIO_SetDir(PORT0, GPIO_Pin_8, 0);
	GPIO_SetDir(PORT0, GPIO_Pin_9, 1);
	GPIO_SetDir(PORT0, GPIO_Pin_10, 1);
	
	IOCON_SetPinMode(IOCON_PIO0_8, PIN_MODE_PullUp);
	IOCON_SetPinMode(IOCON_PIO0_9, PIN_MODE_PullUp);
	IOCON_SetPinMode(IOCON_PIO0_10, PIN_MODE_PullUp);

	SSP_CFG_Type SSP_InitStruct;
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_SSP0, ENABLE);
	SSP_DeInit(LPC_SSP0);
	SSP_SSP0PinsInit(SCK0_PIO0_10, DISABLE);
	SSP_ConfigStructInit(&SSP_InitStruct);
	SSP_Init(LPC_SSP0, &SSP_InitStruct);
	SSP_SetClock(LPC_SSP0, freq);

	SSP_Cmd(LPC_SSP0, ENABLE);
}

uint16_t SPI_Xmit(uint16_t sendData)
{
	uint16_t recvData;
	LPC_SSP0->DR = sendData;
	while(LPC_SSP0->SR & SSP_STAT_BUSY);
	recvData = LPC_SSP0->DR;
	return recvData;
}
