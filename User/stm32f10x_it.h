#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

#include "stm32f10x.h"
#include <stdio.h>
#include "encoder.h"
#include "contact.h"
#include "string.h"
#include "delay.h"
	 
#define USART_REC_LEN  			200  	//定义最大接收字节数 200

void USART1_IRQHandler(void);//串口中断函数
void TIM5_IRQHandler(void);//速度计算定时器中断函数
void TIM1_UP_IRQHandler(void);//里程计发布定时器中断函数
    
#ifdef __cplusplus
}
#endif

#endif /* __STM32F10x_IT_H */

