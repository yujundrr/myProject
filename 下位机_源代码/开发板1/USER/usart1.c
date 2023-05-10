/************************************
 * �ļ���  ��usart1.c
 * ����    ����printf�����ض���USART1�������Ϳ�����printf��������Ƭ��������
 *           ��ӡ��PC�ϵĳ����ն˻򴮿ڵ������֡�         
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�------------------------
 *          | PA9����Ƭ����Tx�� - USART1(Tx)      |
 *          | PA10 ����Ƭ����Rx��- USART1(Rx)      |
 *           ------------------------
 * ��汾  ��ST3.0.0  *

**********************************************************************************/

#include "usart1.h"
#include <stdarg.h>
#include "stm32f10x.h"
#include "dac.h"


u8 USART1_RX[1024];
u16 USART1_RX_STA=0x00;


//Ƕ�������жϿ���������Ҫ��Ϊ�������жϵ�ͨ���Լ����ȼ�
void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	//Ƕ�������жϿ�������ѡ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
	/*����USART1Ϊ�ж�Դ*/
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
  /* �������ȼ�Ϊ1*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ�Ϊ1 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
	
}
//USART��GPIO�Լ�NVIC��ʹ�ܺ�����
void USART1_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* ʹ�� USART1 ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE); 

	/* USART1 ʹ��IO�˿����� */    
	/* PA9��TX,PA10��RX   */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOA, &GPIO_InitStructure);    
  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);   //��ʼ��GPIOA
	
	/*USART_SEL�������ã�ѡ�����������Ǵ����ߵ���*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	/*BT_RST��������*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	USART_SEL_OFF;                                  //ʹ������ͨ��
//  USART_SEL_ON;
	  
	/* USART1 ����ģʽ���� */
	USART_InitStructure.USART_BaudRate = 9600;	//���������ã�9600
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;	//����λ�����ã�8λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 	//ֹͣλ���ã�1λ
	USART_InitStructure.USART_Parity = USART_Parity_No ;  //�Ƿ���żУ�飺��
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;	//Ӳ��������ģʽ���ã�û��ʹ��
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//�����뷢�Ͷ�ʹ��
	USART_Init(USART1, &USART_InitStructure);  //��ʼ��USART1
	USART_Cmd(USART1, ENABLE);// USART1ʹ��
	
	NVIC_Configuration();
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);   //ʹ�ܽ����ж�
}

 /* ����  ���ض���c�⺯��printf��USART1*/ 
int fputc(int ch, FILE *f)
{
/* ��Printf���ݷ������� */
  USART_SendData(USART1, (unsigned char) ch);
  while (!(USART1->SR & USART_FLAG_TXE));
 
  return (ch);
}

////�ض���c�⺯��scanf�����ڣ��ض�����ʹ��scanf��getchar�Ⱥ���
////ʵ���ַ���ȡ����
//int fgetc(FILE *f)
//{
//	//���ﲻ��ȥ��ȡ������ɵ��жϱ�־����Ϊû�п�����������ж�
//	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE)==RESET);
//	
//	return (int)USART_ReceiveData(USART1);
//}

//���ڽ����жϷ�����
void USART1_IRQHandler(void)
{
	
	if (USART_GetITStatus(USART1,USART_IT_RXNE)!=RESET) 
		{
				USART1_RX[USART1_RX_STA++] = USART_ReceiveData(USART1);
		}
}

//������ת��,Ǳ�ڵ�Σ�����븺����ʱ����ܻ����
float Get_Float_value(void)
{
 float TMP;

	while(1)
	{

		if(USART1_RX_STA)   //���ȷʵ�յ������Դ��ڴ���������
		{
			sscanf(USART1_RX,"%f",&TMP);  //TMP�洢��ת������floatֵ
			memset(USART1_RX,0x00,1024);   //����
			USART1_RX_STA=0;               //����
			break;
		}
		Delay(10);
	}
	return TMP;
}
//����ת�������ظ�����
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



