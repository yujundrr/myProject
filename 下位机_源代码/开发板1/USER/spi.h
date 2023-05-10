#ifndef _SPI1_H
#define _SPI1_H

#include "stm32f10x.h"

/*SPI���ĸ����Ŷ���*/
#define SPI1_GPIO_PORT    GPIOA
#define SPI1_GPIO_SYNC_N  GPIO_Pin_4   //PA4
#define SPI1_GPIO_CLK     GPIO_Pin_5   //PA5
#define SPI1_GPIO_SDI     GPIO_Pin_6   //��Ƭ����SDI<����>DAC��SDO,PA6
#define SPI1_GPIO_SDO     GPIO_Pin_7   //��Ƭ����SDO<����>DAC��SDI,PA7

/*SYNC�����ߵ͵�ƽ����������ʼ��ֹͣ�ź�*/
#define DAC_SPI1_GPIO_SYNC_N_HIGH    GPIO_SetBits (SPI1_GPIO_PORT,SPI1_GPIO_SYNC_N)
#define DAC_SPI1_GPIO_SYNC_N_LOW     GPIO_ResetBits (SPI1_GPIO_PORT,SPI1_GPIO_SYNC_N)

/*ͨѶ�ȴ���ʱʱ��*/
#define SPIT_FLAG_TIMEOUT    ((uint32_t) 0x1000)
#define SPIT_LONG_TIMEOUT    ((uint32_t)(10*I2CT_FLAG_TIMEOUT))

//ͨ����Ϣ���
#define DAC_SPI_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)

//SPI�ĳ�ʼ������
void DAC_SPI1_Init(void);
/*SPIд�������������ͷ���1���쳣����0����ӡ������Ϣ*/
u8 SPI_Write(unsigned char *data,unsigned char bytesNumber);
unsigned long SPI_Read(unsigned char *data,unsigned char bytesNumber);

#endif /*_SPI1_H*/


