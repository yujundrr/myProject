#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"
#include "spi.h"

//GPIO
/*DAC的另外三个数字控制口的定义*/
#define DAC_GPIO_LDAC_N           GPIO_Pin_1    //PB1
#define DAC_GPIO_CLR_N            GPIO_Pin_10   //PB10
#define DAC_GPIO_RST_N            GPIO_Pin_2    //PA2

/*LDAC产生高低电平*/
#define DAC_GPIO_LDAC_N_HIGH         GPIO_SetBits (GPIOB,DAC_GPIO_LDAC_N)
#define DAC_GPIO_LDAC_N_LOW          GPIO_ResetBits (GPIOB,DAC_GPIO_LDAC_N)
/*CLR产生高低电平*/
#define DAC_GPIO_CLR_N_HIGH          GPIO_SetBits (GPIOB,DAC_GPIO_CLR_N)
#define DAC_GPIO_CLR_N_LOW           GPIO_ResetBits (GPIOB,DAC_GPIO_CLR_N)
/*RST产生高低电平*/
#define DAC_GPIO_RST_N_HIGH          GPIO_SetBits (GPIOA,DAC_GPIO_RST_N)
#define DAC_GPIO_RST_N_LOW           GPIO_ResetBits (GPIOA,DAC_GPIO_RST_N)

//24位数据中的内容：读写控制、寄存器地址、有效数据
#define DAC_CMD_READ               (1<<23)
#define DAC_CMD_WRITE              (0<<23)
#define DAC_ADDR_REG(addr)         ((addr)<<20)

/*寄存器地址定义*/
#define DAC_REG_ADDR_NOOP           0
#define DAC_REG_ADDR_DAC            1
#define DAC_REG_ADDR_CTRL           2
#define DAC_REG_ADDR_CLRCODE        3
#define DAC_CMD_WR_SOFT_CTRL        4

/*控制寄存器的控制字定义*/
#define DAC_CTRL_RBUF      (1<<1)
#define DAC_CTRL_OPGND     (1<<2)
#define DAC_CTRL_DACTRI    (1<<3)
#define DAC_CTRL_BIN2SC    (1<<4)
#define DAC_CTRL_SDODIS    (1<<5)

/*输出接地钳控制*/
#define DAC_CTRL_PWRDN_6K      0       //输出通过6K电阻钳位到地，不正常状态
#define DAC_CTRL_PWRDN_3STATE  1       //DAC处于三态

/*软件控制寄存器的控制字定义*/
#define DAC_SWCTRL_LDAC    (1<<0)
#define DAC_SWCTRL_CLR     (1<<1)
#define DAC_SWCTRL_RESET   (1<<2)


//用于接收的任何数
#define Dummy_Byte   0xFF

void AD5760_Init(void);                                                  //初始化
void AD5760_EnableOutput(unsigned char state);                           //使能输出                          
void AD5760_SetRegisterValue(unsigned char registerAddress,              //设置寄存器值
                             unsigned long registerValue,
                             unsigned char bytesNumber);																																					
void AD5760_SetDacValue(unsigned long value);                             //DAC寄存器设置
void Delay(unsigned long time);                                           //延时函数
void DelayUs(unsigned long time);
void AD5760_SoftInstruction(unsigned char instructionBit);                //软件控制寄存器设置
void AD5760_Setup_CTRLREG(unsigned long setupWord);                       //控制寄存器设置
void AD5760_Setup_CLRCODE(unsigned long clrCode);                         //清零代码寄存器设置
unsigned long AD5760_GetRegisterValue(unsigned char registerAddress,      //获取寄存器的值
                                      unsigned char bytesNumber);



#endif /* __DAC_H */

