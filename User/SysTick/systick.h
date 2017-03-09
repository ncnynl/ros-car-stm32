/*********************************************************************************************************
*
* File                : systick.h
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.32
* Version             : V1.0
* By                  : mao
* Help Web			  : http://fantin.taobao.com/
*
*
*********************************************************************************************************/

#ifndef __SYSTICK_H
#define __SYSTICK_H 

/* Includes ------------------------------------------------------------------*/	   
#include "stm32f10x.h"

//#define bool unsigned char
#define TRUE    1
#define FALSE   0
/* Private function prototypes -----------------------------------------------*/
void Delay_Init(void);
void Delay(__IO uint32_t nTime);
void TimingDelay_Decrement(void);
//void delay_us(u16 time);
//void delay_ms(u32 nCount);

#endif
/*********************************************************************************************************
      END FILE
*********************************************************************************************************/






























