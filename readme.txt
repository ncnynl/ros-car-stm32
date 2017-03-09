
其中 积分计算有所变化


加入速度差值变化


#define SPEED_SAMPLING_FREQ (u16)(10000/(SPEED_SAMPLING_TIME+1))  //1000hz


脉冲的计算放在了中断里面

完整版的简易避障程序，但是电机转速计算还是需要改进，用测脉冲的方式计算转速。


