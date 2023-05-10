#include "stm32f10x_tim.h"
#include "timer.h"


/*�ж�����Ϊ1ms����ʱ��û��Ӳ��I/O�ڣ�����Ҫ�����������
  ����Ҫ�Զ�ʱ������������ã�ʱ�ӡ��Լ���ؼĴ�����ֵ
*/
void TIM4_Int_Init(u16 arr,u16 psc)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
    TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period=arr;	//�Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) 
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= psc;	//ʱ��Ԥ��Ƶ�� 72M/72   
    //TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;//����ʱ�ӷָ�:TDTS = Tck_tim Ԥ��ƵΪ1 	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ 
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
    //TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);  //�����������ж�
	
	  TIM_ARRPreloadConfig(TIM4, ENABLE);  //ʹ�ܼ���������
	
	   /* ���ø�������Դֻ�ڼ��������������ʱ�����ж� */
    TIM_UpdateRequestConfig(TIM4,TIM_UpdateSource_Global);   //������������
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);    //������±�־
	
    //TIM_Cmd(TIM4, ENABLE);	// ������ʱ��   
}

/* TIM2�ж����ȼ����� */
void TIM4_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);    													
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	  ////ʹ��TIM3�ж����ڵ��ⲿ�ж�ͨ��
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void delay_timer_ms(__IO uint32_t nTime)  //���徲̬����nTime����֤nTime�����µ�
{     
	  TIM_Cmd(TIM4, DISABLE);   //�رն�ʱ��
    TIM4->CNT = 0;  //���������
    TIM_Cmd(TIM4, ENABLE);  //ʹ�ܶ�ʱ��3    

    for( ; nTime > 0 ; nTime--)
    {
//		 TIM4->CNT = 0;  //�������
     while(TIM_GetFlagStatus(TIM4, TIM_FLAG_Update) == RESET);  //�ж϶�ʱ���ļ������Ƿ������
     TIM_ClearITPendingBit(TIM4,TIM_IT_Update);//���TIMx�����жϱ�־ 
    }

    //TIM_Cmd(TIM4, DISABLE);
}




