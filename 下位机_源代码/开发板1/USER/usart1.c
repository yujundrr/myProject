/************************************
 * 文件名  ：usart1.c
 * 描述    ：将printf函数重定向到USART1。这样就可以用printf函数将单片机的数据
 *           打印到PC上的超级终端或串口调试助手。         
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 硬件连接：------------------------
 *          | PA9（单片机的Tx） - USART1(Tx)      |
 *          | PA10 （单片机的Rx）- USART1(Rx)      |
 *           ------------------------
 * 库版本  ：ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "dac.h"


u8 USART1_RX[1024];
u16 USART1_RX_STA=0x00;


//嵌套向量中断控制器，主要是为了配置中断的通道以及优先级
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//嵌套向量中断控制器组选择
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
	/*配置USART1为中断源*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* 抢断优先级为1*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* 子优先级为1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
}
//USART和GPIO以及NVIC的使能和配置
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* 使能 USART1 时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 使用IO端口配置 */    
	/* PA9是TX,PA10是RX   */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //初始化GPIOA
	
	/*USART_SEL引脚配置，选择用蓝牙还是串口线调试*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	/*BT_RST引脚配置*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	USART_SEL_OFF;                                  //使用蓝牙通信
//  USART_SEL_ON;
	  
	/* USART1 工作模式配置 */
	USART_InitStructure.USART_BaudRate = 9600;	//波特率设置：9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//数据位数设置：8位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//停止位设置：1位
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //是否奇偶校验：无
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//硬件流控制模式设置：没有使能
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//接收与发送都使能
	USART_Init(USART1, &USART_InitStructure);  //初始化USART1
	USART_Cmd(USART1, ENABLE);// USART1使能
	
	NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   //使能接收中断
}

 /* 描述  ：重定向c库函数printf到USART1*/ 
int fputc(int ch, FILE *f)
{
/* 将Printf内容发往串口 */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

////重定向c库函数scanf到串口，重定向后可使用scanf、getchar等函数
////实现字符读取功能
//int fgetc(FILE *f)
//{
//	//这里不能去获取接收完成的中断标志，因为没有开启加收完成中断
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
//	
//	return (int)USART_ReceiveData(USART1);
//}

//串口接收中断服务函数
void USART1_IRQHandler(void)
{
	
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) 
		{
				USART1_RX[USART1_RX_STA++] = USART_ReceiveData(USART1);
		}
}

//浮点数转换,潜在的危机输入负数的时候可能会出错
float Get_Float_value(void)
{
 float TMP;

	while(1)
	{

		if(USART1_RX_STA)   //如果确实收到了来自串口传来的数据
		{
			sscanf(USART1_RX,"%f",&TMP);  //TMP存储了转换来的float值
			memset(USART1_RX,0x00,1024);   //重置
			USART1_RX_STA=0;               //重置
			break;
		}
		Delay(10);
	}
	return TMP;
}
//整数转换，返回该整数
int Get_Integer_value(void)
{
	int Integer;
	while(1)
	{
		if(USART1_RX_STA)
		{
			sscanf(USART1_RX,"%d",&Integer);
			memset(USART1_RX,0x00,1024);
			USART1_RX_STA=0;
			break;
		}
		Delay(10);
	}
	return Integer;
}



