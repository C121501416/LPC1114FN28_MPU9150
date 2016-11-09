/********************************************************************************/
/*!
	@file			systick.h
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
#ifndef __SYSTICK_H
#define __SYSTICK_H 0x0200

#ifdef __cplusplus
 extern "C" {
#endif

/* General Inclusion */
#include "LPC11xx.h"                        /* LPC11xx definitions */

/* Systick Interval milliSecond order */
#define INTERVAL	1000

/* Inline Static Function */
extern __IO uint32_t TimingDelay;
extern __IO uint32_t SystemTimer_ms;
static inline void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

void _delay_ms(__IO uint32_t mSec);
void _delay_us(__IO uint32_t uSec);
void SysTickInit(__IO uint32_t interval);

#ifdef __cplusplus
}
#endif

#endif	/* __SYSTICK_H */
