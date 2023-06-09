#ifndef __USART1_H
#define	__USART1_H

#include "stm32f10x.h"
#include <stdio.h>

extern u8 USART1_RX[1024];
extern u16 USART1_RX_STA;


#define USART_SEL_OFF   GPIO_ResetBits(GPIOA,GPIO_Pin_11)    //单片机与蓝牙通信，0
#define USART_SEL_ON    GPIO_SetBits(GPIOA,GPIO_Pin_11)      //单片机与串口线通信�,1
#define BT_RST_ON       GPIO_SetBits(GPIOA,GPIO_Pin_8)          //HC-06的解复位，1：解复位
#define BT_RST_OFF      GPIO_ResetBits(GPIOA,GPIO_Pin_8)          //HC-06的解复位，1：解复位

void USART1_Config(void);
void NVIC_Configuration(void);
float Get_Float_value(void);
int Get_Integer_value(void);
int fputc(int ch, FILE *f);
//int fgetc(FILE *f);


#endif /* __USART1_H */
