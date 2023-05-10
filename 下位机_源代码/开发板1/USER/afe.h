#ifndef __AFE_H
#define	__AFE_H


#include "stm32f10x.h"

//模拟SPI接口，在PB口
#define AFE_SPI_PORT  GPIOB
#define AFE_SCLK      GPIO_Pin_9
#define AFE_SIM_SDI   GPIO_Pin_8     //AFE的SDO<——>单片机的SDI
#define AFE_SIM_SDO   GPIO_Pin_7     //AFE的SDI<——>单片机的SDO
#define AFE_CS_N      GPIO_Pin_6
#define AFE_LATCH_N   GPIO_Pin_5
#define AFE_EN        GPIO_Pin_0    //PB0
//模式配置引脚，在PA口
#define AFE_MODE_PORT GPIOA
#define AFE_MODE      GPIO_Pin_3    //PA3

/*ADA4350工作模式配置*/
#define AFE_SPI_MODE          GPIO_SetBits (AFE_SPI_PORT,AFE_EN);GPIO_ResetBits (AFE_MODE_PORT,AFE_MODE)
#define AFE_PAR_MANU_MODE     GPIO_SetBits (AFE_SPI_PORT,AFE_EN);GPIO_SetBits (AFE_MODE_PORT,AFE_MODE)    

/*几个端口的高低电平输出控制*/
//时钟
#define AFE_SPI_SCLK_HIGH     GPIO_SetBits (AFE_SPI_PORT,AFE_SCLK)
#define AFE_SPI_SCLK_LOW      GPIO_ResetBits (AFE_SPI_PORT,AFE_SCLK)
//片选
#define AFE_SPI_CS_N_HIGH     GPIO_SetBits (AFE_SPI_PORT,AFE_CS_N)
#define AFE_SPI_CS_N_LOW      GPIO_ResetBits (AFE_SPI_PORT,AFE_CS_N)
//锁存
#define AFE_SPI_LATCH_N_HIGH  GPIO_SetBits (AFE_SPI_PORT,AFE_LATCH_N)
#define AFE_SPI_LATCH_N_LOW   GPIO_ResetBits (AFE_SPI_PORT,AFE_LATCH_N)
//SDO输出
#define AFE_SPI_SD0_HIGH      GPIO_SetBits (AFE_SPI_PORT,AFE_SIM_SDO)
#define AFE_SPI_SDO_LOW       GPIO_ResetBits (AFE_SPI_PORT,AFE_SIM_SDO)
//读命令
#define READCOMMAND     0X00800000

//跨阻增益选择,写入寄存器内的值
#define AFE_FB0       0X00000041
#define AFE_FB0_1pF   0X00002041
#define AFE_FB1       0X00000082
#define AFE_FB1_1pF   0X00002082
#define AFE_FB2       0X00000104
#define AFE_FB2_1pF   0X00002104
#define AFE_FB3       0X00000208
#define AFE_FB3_1pF   0X00002208
#define AFE_FB4       0X00000410
#define AFE_FB4_1pF   0X00002410
#define AFE_FB5       0X00000840
#define AFE_FB5_1pF   0X00002840

//增益表示，后面用的到
#define GAIN_1K       AFE_FB0_1pF
#define GAIN_5K       AFE_FB1_1pF
#define GAIN_10K      AFE_FB2_1pF
#define GAIN_50K      AFE_FB3_1pF
#define GAIN_100K     AFE_FB4_1pF
#define GAIN_1M       AFE_FB5_1pF

//函数声明
void ADA4350_SIM_SPI_Init(void);
u32 AFE_SIM_SPI_Write(u32 feedback);
u8 bit_from24(u32 data,int i);
u32 AFE_SIM_SPI_Read(void);
void AFE_WriteToReg(u32 feedback);
u32 AFE_ReadFromReg(void);
void AFE_gain_nocal(unsigned long *cal_value);
void AFE_gain_cal(unsigned long *cal_value);
int AFE_Choosegain(int gain);



#endif /* __AFE_H */

