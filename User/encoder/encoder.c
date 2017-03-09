#include "encoder.h"

/****************************************************************************************************************/

s32 hSpeed_Buffer2[SPEED_BUFFER_SIZE]={0}, hSpeed_Buffer1[SPEED_BUFFER_SIZE]={0};//左右轮速度缓存数组
static unsigned int hRot_Speed2;//电机A平均转速缓存
static unsigned int hRot_Speed1;//电机B平均转速缓存
unsigned int Speed2=0; //电机A平均转速 r/min，PID调节
unsigned int Speed1=0; //电机B平均转速 r/min，PID调节

static volatile u16 hEncoder_Timer_Overflow1;//电机B编码数采集 
static volatile u16 hEncoder_Timer_Overflow2;//电机A编码数采集

//float A_REMP_PLUS;//电机APID调节后的PWM值缓存
float pulse = 0;//电机A PID调节后的PWM值缓存
float pulse1 = 0;//电机B PID调节后的PWM值缓存

int span;//采集回来的左右轮速度差值

static bool bIs_First_Measurement2 = true;//电机A以清除速度缓存数组标志位
static bool bIs_First_Measurement1 = true;//电机B以清除速度缓存数组标志位

struct PID Control_left  ={0.01,0.1,0.75,0,0,0,0,0,0};//左轮PID参数，适于新电机4096
struct PID Control_right ={0.01,0.1,0.75,0,0,0,0,0,0};//右轮PID参数，适于新电机4096

/****************************************************************************************************************/

s32 hPrevious_angle2, hPrevious_angle1;

/****************************************************************************************************************/

void ENC_Init2(void)//电机A码盘采集定时器，TIM4编码器模式
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;    
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_DeInit(ENCODER2_TIMER);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;  
    TIM_TimeBaseStructure.TIM_Period = (4*ENCODER2_PPR)-1;

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(ENCODER2_TIMER, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(ENCODER2_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;
    TIM_ICInit(ENCODER2_TIMER, &TIM_ICInitStructure);

    TIM_ClearFlag(ENCODER2_TIMER, TIM_FLAG_Update);
    TIM_ITConfig(ENCODER2_TIMER, TIM_IT_Update, ENABLE);

    TIM_Cmd(ENCODER2_TIMER, ENABLE); 
}

void ENC_Init1(void)//电机B码盘采集定时器，TIM3编码器模式
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    TIM_DeInit(ENCODER1_TIMER);
    TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);

    TIM_TimeBaseStructure.TIM_Prescaler = 0;
    TIM_TimeBaseStructure.TIM_Period = (4*ENCODER1_PPR)-1;  

    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;   
    TIM_TimeBaseInit(ENCODER1_TIMER, &TIM_TimeBaseStructure);

    TIM_EncoderInterfaceConfig(ENCODER1_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
    TIM_ICStructInit(&TIM_ICInitStructure);
    TIM_ICInitStructure.TIM_ICFilter = ICx_FILTER;
    TIM_ICInit(ENCODER1_TIMER, &TIM_ICInitStructure);

    TIM_ClearFlag(ENCODER1_TIMER, TIM_FLAG_Update);
    TIM_ITConfig(ENCODER1_TIMER, TIM_IT_Update, ENABLE);

    TIM_Cmd(ENCODER1_TIMER, ENABLE); 
}

/****************************************************************************************************************/

s16 ENC_Calc_Rot_Speed2(void)//计算电机A的编码数
{   
    s32 wDelta_angle;
    u16 hEnc_Timer_Overflow_sample_one;
    u16 hCurrent_angle_sample_one;
    s32 temp;
    s16 haux;

    if (!bIs_First_Measurement2)//电机A以清除速度缓存数组
    {  
        hEnc_Timer_Overflow_sample_one = hEncoder_Timer_Overflow2; 	
        hCurrent_angle_sample_one = ENCODER2_TIMER->CNT;
        hEncoder_Timer_Overflow2 = 0;
        haux = ENCODER2_TIMER->CNT;   

        if ( (ENCODER2_TIMER->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  
        {
            // encoder timer down-counting 反转的速度计算     
            wDelta_angle = (s32)((hEnc_Timer_Overflow_sample_one) * (4*ENCODER2_PPR) -(hCurrent_angle_sample_one - hPrevious_angle2));
        }
        else  
        {
            //encoder timer up-counting 正转的速度计算
            wDelta_angle = (s32)(hCurrent_angle_sample_one - hPrevious_angle2 + (hEnc_Timer_Overflow_sample_one) * (4*ENCODER2_PPR));
        }		
        temp=wDelta_angle;
    } 
    else
    {
        bIs_First_Measurement2 = false;//电机A以清除速度缓存数组标志位
        temp = 0;
        hEncoder_Timer_Overflow2 = 0;
        haux = ENCODER2_TIMER->CNT;       
    }
    hPrevious_angle2 = haux;  
    return((s16) temp);
}


s16 ENC_Calc_Rot_Speed1(void)//计算电机B的编码数
{   
    s32 wDelta_angle;
    u16 hEnc_Timer_Overflow_sample_one;
    u16 hCurrent_angle_sample_one;
    s32 temp;
    s16 haux;

    if (!bIs_First_Measurement1)//电机B以清除速度缓存数组
    {   
        hEnc_Timer_Overflow_sample_one = hEncoder_Timer_Overflow1; 	//得到采样时间内的编码数	
        hCurrent_angle_sample_one = ENCODER1_TIMER->CNT;
        hEncoder_Timer_Overflow1 = 0;//清除脉冲数累加
        haux = ENCODER1_TIMER->CNT;   

        if ( (ENCODER1_TIMER->CR1 & TIM_CounterMode_Down) == TIM_CounterMode_Down)  
        {
            // encoder timer down-counting 反转的速度计算
            wDelta_angle = (s32)((hEnc_Timer_Overflow_sample_one) * (4*ENCODER1_PPR) -(hCurrent_angle_sample_one - hPrevious_angle1));	
        }
        else  
        {
            //encoder timer up-counting 正转的速度计算
            wDelta_angle = (s32)(hCurrent_angle_sample_one - hPrevious_angle1 + (hEnc_Timer_Overflow_sample_one) * (4*ENCODER1_PPR));
        }
        temp=wDelta_angle;
    } 
    else
    {
        bIs_First_Measurement1 = false;//电机B以清除速度缓存数组标志位
        temp = 0;
        hEncoder_Timer_Overflow1 = 0;
        haux = ENCODER1_TIMER->CNT;       
    }
    hPrevious_angle1 = haux;  
    return((s16) temp);
}


/****************************************************************************************************************/

void ENC_Clear_Speed_Buffer(void)//速度存储器清零
{   
    u32 i;

    //清除左右轮速度缓存数组
    for (i=0;i<SPEED_BUFFER_SIZE;i++)
    {
        hSpeed_Buffer2[i] = 0;
        hSpeed_Buffer1[i] = 0;
    }
    
    bIs_First_Measurement2 = true;//电机A以清除速度缓存数组标志位
    bIs_First_Measurement1 = true;//电机B以清除速度缓存数组标志位
}

void ENC_Calc_Average_Speed(void)//计算三次电机的平均编码数
{   
    u32 i;
	signed long long wtemp3=0;
	signed long long wtemp4=0;

    //累加缓存次数内的速度值
	for (i=0;i<SPEED_BUFFER_SIZE;i++)
	{
		wtemp4 += hSpeed_Buffer2[i];
		wtemp3 += hSpeed_Buffer1[i];
	}
    
    //取平均，平均脉冲数单位为 个/s	
	wtemp3 /= (SPEED_BUFFER_SIZE);
	wtemp4 /= (SPEED_BUFFER_SIZE); //平均脉冲数 个/s	
    
    //将平均脉冲数单位转为 r/min
	wtemp3 = (wtemp3 * SPEED_SAMPLING_FREQ)*60/(4*ENCODER1_PPR);
	wtemp4 = (wtemp4 * SPEED_SAMPLING_FREQ)*60/(4*ENCODER2_PPR); 
		
	hRot_Speed2= ((s16)(wtemp4));//平均转速 r/min
	hRot_Speed1= ((s16)(wtemp3));//平均转速 r/min
	Speed2=hRot_Speed2;//平均转速 r/min
	Speed1=hRot_Speed1;//平均转速 r/min
}

/****************************************************************************************************************/

void Gain2(void)//设置电机A PID调节 PA2
{
	//static float pulse = 0;
    
	span=1*(Speed1-Speed2);//采集回来的左右轮速度差值
	pulse= pulse + PID_calculate(&Control_right,hRot_Speed2);//PID调节
    
    //pwm幅度抑制
	if(pulse > 3600) pulse = 3600;
	if(pulse < 0) pulse = 0;
    
	//A_REMP_PLUS=pulse;//电机APID调节后的PWM值缓存
}


void Gain1(void)//设置电机B PID调节 PA1
{
	//static float pulse1 = 0;
    
	span=1*(Speed2-Speed1);//采集回来的左右轮速度差值
	pulse1= pulse1 + PID_calculate(&Control_left,hRot_Speed1);//PID调节
    
    ////pwm 幅度抑制
	if(pulse1 > 3600) pulse1 = 3600;
	if(pulse1 < 0) pulse1 = 0;
	
	TIM2->CCR2 = pulse1;//电机B赋值PWM
	//TIM2->CCR3 = A_REMP_PLUS;//电机A赋值PWM
    TIM2->CCR3 = pulse;//电机A赋值PWM
}

/****************************************************************************************************************/

void ENC_Init(void)//电机处理初始化
{
    ENC_Init2();              //设置电机A TIM4编码器模式PB6 PB7 右电机
	ENC_Init1();              //设置电机B TIM3编码器模式PA6 PA7 左电机
    ENC_Clear_Speed_Buffer();//速度存储器清零
}

/****************************************************************************************************************/

void TIM4_IRQHandler (void)//执行TIM4(电机A编码器采集)计数中断
{   
    TIM_ClearFlag(ENCODER2_TIMER, TIM_FLAG_Update);
    if (hEncoder_Timer_Overflow2 != U16_MAX)//不超范围  
    {
        hEncoder_Timer_Overflow2++; //脉冲数累加
    }
}

void TIM3_IRQHandler (void)//执行TIM3(电机B编码器采集)计数中断
{  
    TIM_ClearFlag(ENCODER1_TIMER, TIM_FLAG_Update);
    if (hEncoder_Timer_Overflow1 != U16_MAX)//不超范围    
    {
        hEncoder_Timer_Overflow1++;	 //脉冲数累加
    }
}

