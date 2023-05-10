#include "stm32f10x_tim.h"
#include "timer.h"


/*中断周期为1ms，定时器没有硬件I/O口，不需要对其进行配置
  但是要对定时器本身进行配置，时钟、以及相关寄存器的值
*/
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period=arr;	//自动重装载寄存器周期的值(计数值) 
    /* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//时钟预分频数 72M/72   
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//设置时钟分割:TDTS = Tck_tim 预分频为1 	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    //TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //开启计数器中断
	
	  TIM_ARRPreloadConfig(TIM4, ENABLE);  //使能计数器计数
	
	   /* 设置更新请求源只在计数器上溢或上溢时产生中断 */
    TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Global);   //更新请求配置
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);    //清除更新标志
	
    //TIM_Cmd(TIM4, ENABLE);	// 开启定时器   
}

/* TIM2中断优先级配置 */
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  ////使能TIM3中断所在的外部中断通道
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void delay_timer_ms(__IO uint32_t nTime)  //定义静态变量nTime，保证nTime是最新的
{     
	  TIM_Cmd(TIM4, DISABLE);   //关闭定时器
    TIM4->CNT = 0;  //清零计数器
    TIM_Cmd(TIM4, ENABLE);  //使能定时器3    

    for( ; nTime > 0 ; nTime--)
    {
//		 TIM4->CNT = 0;  //清零计数
     while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);  //判断定时器的计数器是否有溢出
     TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//清除TIMx更新中断标志 
    }

    //TIM_Cmd(TIM4, DISABLE);
}




