#ifndef TIME_TEST_H
#define TIME_TEST_H

#include "stm32f10x.h"
 

void TIM4_NVIC_Configuration(void);
void TIM4_Int_Init(u16 arr,u16 psc);
void delay_timer_ms(__IO uint32_t nTime);

#endif	/* TIME_TEST_H */
