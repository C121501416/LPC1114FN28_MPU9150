/********************************************************************************/
/*!
	@file			systick.c
	@author         Nemui Trinomius (http://nemuisan.blog.bai.ne.jp)
    @version        2.00
    @date           2011.03.28
	@brief          delay mSec-order routine using systick timer			@n
					delay uSec-order routine using __NOP();

    @section HISTORY
		2010.07.21	V1.00	Restart Here.
		2011.03.28	V2.00	C++ Ready.

    @section LICENSE
		BSD License. See Copyright.txt
*/
/********************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"

/* Defines -------------------------------------------------------------------*/

/* Constants -----------------------------------------------------------------*/

/* Variables -----------------------------------------------------------------*/

/* Grobals -------------------------------------------------------------------*/
__IO uint32_t TimingDelay;
__IO uint32_t SystemTimer_ms;

/* Prototypes ----------------------------------------------------------------*/

/* Functions -----------------------------------------------------------------*/

/**************************************************************************/
/**
  * @brief  SysTick Timer Initialization.
  * @param Interval: Set TimeBase of Systick.
  * @retval : None
  */
/**************************************************************************/
void SysTickInit(__IO uint32_t interval)
{
	SystemCoreClockUpdate();

	if (SysTick_Config(SystemCoreClock / interval)) { 	/* Setup SysTick Timer for 1 msec interrupts  */
		while (1);										/* Capture error */
	}

	SystemTimer_ms = 0;
}

/**************************************************************************/
/**
  * @brief  delay accuracy MilliSeconds.
  * @param mSec: Set Delay Time mS Order.
  * @retval : None
  */
/**************************************************************************/
void _delay_ms(__IO uint32_t mSec)
{ 
	TimingDelay = mSec;

	while(TimingDelay != 0);
}


/**************************************************************************/
/**
  * @brief  delay non-accuracy MicroSeconds.
  * @param mSec: Set Delay Time uS Order.
  * @retval : None
  */
/**************************************************************************/
void _delay_us(__IO uint32_t uSec)
{
	while(uSec--){ 
					__NOP();
					__NOP();
					__NOP();
					__NOP();
					__NOP();
					}
}


/**************************************************************************/
/**
  * @brief  Inserts a stupid delay time.
  * @param nCount: specifies the delay time length.
  * @retval : None
  */
/**************************************************************************/
void Delay(__IO uint32_t nCount)
{
	for(; nCount != 0; nCount--);
}


/**************************************************************************/
/**
  * @brief  This function handles SysTick Handler.
  * @param  : None
  * @retval : None
  */
/**************************************************************************/
void SysTick_Handler(void)
{
	/* every 1mSec */
	TimingDelay_Decrement();

	/* update system timer */
	SystemTimer_ms++;
}

/* End Of File ---------------------------------------------------------------*/
