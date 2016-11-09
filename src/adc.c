/*
 * Project:		LPC1114_RS485Slave
 * File:		adc.c
 * Author:		Terukazu Sato
 *
 */

#include "lpc11xx_adc.h"
#include "lpc11xx_syscon.h"
#include "systick.h"

void ADC_InitNormalMode()
{
	SYSCON_PowerCon(SYSCON_ABLOCK_ADC, ENABLE);
	SYSCON_AHBPeriphClockCmd(SYSCON_AHBPeriph_ADC, ENABLE);
	LPC_ADC->CR = ADC_CR_CLKDIV(10);
}
int ADC_Read(int ch)
{
	int wait = 0xffff;
	if(ch>7) return 0;
	LPC_ADC->CR &= 0xffffff00;
	LPC_ADC->CR |= ADC_CR_START_NOW|ADC_CR_CH_SEL(ch);
	while(wait--) {
		if(LPC_ADC->GDR & ADC_GDR_DONE_FLAG) break;
		_delay_us(1);
	}
	LPC_ADC->CR &= ~ADC_CR_START_MASK;
	return ADC_GDR_RESULT(LPC_ADC->GDR);
}
