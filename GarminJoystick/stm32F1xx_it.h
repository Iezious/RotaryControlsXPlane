#pragma once

#ifndef __STM32F1xx_IT_H
#define __STM32F1xx_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f1xx_hal.h"


void SysTick_Handler(void);
void PendSV_Handler(void);
void USB_LP_CAN1_RX0_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);
	 
	 
#ifdef __cplusplus
}
#endif

#endif /* __STM32F1xx_IT_H */