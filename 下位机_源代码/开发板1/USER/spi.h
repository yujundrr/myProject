#ifndef _SPI1_H
#define _SPI1_H

#include "stm32f10x.h"

/*SPI的四个引脚定义*/
#define SPI1_GPIO_PORT    GPIOA
#define SPI1_GPIO_SYNC_N  GPIO_Pin_4   //PA4
#define SPI1_GPIO_CLK     GPIO_Pin_5   //PA5
#define SPI1_GPIO_SDI     GPIO_Pin_6   //单片机的SDI<——>DAC的SDO,PA6
#define SPI1_GPIO_SDO     GPIO_Pin_7   //单片机的SDO<——>DAC的SDI,PA7

/*SYNC产生高低电平，即产生开始和停止信号*/
#define DAC_SPI1_GPIO_SYNC_N_HIGH    GPIO_SetBits (SPI1_GPIO_PORT,SPI1_GPIO_SYNC_N)
#define DAC_SPI1_GPIO_SYNC_N_LOW     GPIO_ResetBits (SPI1_GPIO_PORT,SPI1_GPIO_SYNC_N)

/*通讯等待超时时间*/
#define SPIT_FLAG_TIMEOUT    ((uint32_t) 0x1000)
#define SPIT_LONG_TIMEOUT    ((uint32_t)(10*I2CT_FLAG_TIMEOUT))

//通信信息输出
#define DAC_SPI_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)

//SPI的初始化函数
void DAC_SPI1_Init(void);
/*SPI写函数，正常发送返回1，异常返回0并打印错误信息*/
u8 SPI_Write(unsigned char *data,unsigned char bytesNumber);
unsigned long SPI_Read(unsigned char *data,unsigned char bytesNumber);

#endif /*_SPI1_H*/


