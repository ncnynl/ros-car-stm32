/*********************************************************************************************************
*
* File                : systick.c
* Hardware Environment: 
* Build Environment   : RealView MDK-ARM  Version: 4.32
* Version             : V1.0
* By                  : mao
* Help Web			  : http://fantin.taobao.com/
*
*
*********************************************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "systick.h"
#include "core_cm3.h"

/* Private variables ---------------------------------------------------------*/	 
static __IO uint32_t TimingDelay;

void Delay_Init(void)
{
  if (SysTick_Config(SystemCoreClock / 1000))
  { 
    /* Capture error */ 
    while (1);
  }
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
void Delay(__IO uint32_t nTime)
{ 
  TimingDelay = nTime;

  while(TimingDelay != 0);
}

/**
  * @brief  Decrements the TimingDelay variable.
  * @param  None
  * @retval None
  */
void TimingDelay_Decrement(void)
{
  if (TimingDelay != 0x00)
  { 
    TimingDelay--;
  }
}

 /******************************************************************************
* Function Name  : delay
* Description    : 延时约1us
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//void delay_us(u16 time)
//{
//	time=time*8;  
//	while(time--);
//}

/*******************************************************************************
* Function Name  : Delay_ms   毫秒级延时函数
* Description    : config MCU LCD_DB pins AS OutPut
* Input          : None
* Output         : None
* Return         : None.
*******************************************************************************/
//void delay_ms(u32 nCount)
//{
//  for (; nCount != 0; nCount--)
//  {
//	 delay_us(1000);
//  }
//}



/*********************************************************************************************************
      END FILE
*********************************************************************************************************/
















