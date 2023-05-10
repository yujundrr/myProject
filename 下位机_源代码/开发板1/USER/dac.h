#ifndef __DAC_H
#define	__DAC_H


#include "stm32f10x.h"
#include "spi.h"

//GPIO
/*DAC�������������ֿ��ƿڵĶ���*/
#define DAC_GPIO_LDAC_N           GPIO_Pin_1    //PB1
#define DAC_GPIO_CLR_N            GPIO_Pin_10   //PB10
#define DAC_GPIO_RST_N            GPIO_Pin_2    //PA2

/*LDAC�����ߵ͵�ƽ*/
#define DAC_GPIO_LDAC_N_HIGH         GPIO_SetBits (GPIOB,DAC_GPIO_LDAC_N)
#define DAC_GPIO_LDAC_N_LOW          GPIO_ResetBits (GPIOB,DAC_GPIO_LDAC_N)
/*CLR�����ߵ͵�ƽ*/
#define DAC_GPIO_CLR_N_HIGH          GPIO_SetBits (GPIOB,DAC_GPIO_CLR_N)
#define DAC_GPIO_CLR_N_LOW           GPIO_ResetBits (GPIOB,DAC_GPIO_CLR_N)
/*RST�����ߵ͵�ƽ*/
#define DAC_GPIO_RST_N_HIGH          GPIO_SetBits (GPIOA,DAC_GPIO_RST_N)
#define DAC_GPIO_RST_N_LOW           GPIO_ResetBits (GPIOA,DAC_GPIO_RST_N)

//24λ�����е����ݣ���д���ơ��Ĵ�����ַ����Ч����
#define DAC_CMD_READ               (1<<23)
#define DAC_CMD_WRITE              (0<<23)
#define DAC_ADDR_REG(addr)         ((addr)<<20)

/*�Ĵ�����ַ����*/
#define DAC_REG_ADDR_NOOP           0
#define DAC_REG_ADDR_DAC            1
#define DAC_REG_ADDR_CTRL           2
#define DAC_REG_ADDR_CLRCODE        3
#define DAC_CMD_WR_SOFT_CTRL        4

/*���ƼĴ����Ŀ����ֶ���*/
#define DAC_CTRL_RBUF      (1<<1)
#define DAC_CTRL_OPGND     (1<<2)
#define DAC_CTRL_DACTRI    (1<<3)
#define DAC_CTRL_BIN2SC    (1<<4)
#define DAC_CTRL_SDODIS    (1<<5)

/*����ӵ�ǯ����*/
#define DAC_CTRL_PWRDN_6K      0       //���ͨ��6K����ǯλ���أ�������״̬
#define DAC_CTRL_PWRDN_3STATE  1       //DAC������̬

/*������ƼĴ����Ŀ����ֶ���*/
#define DAC_SWCTRL_LDAC    (1<<0)
#define DAC_SWCTRL_CLR     (1<<1)
#define DAC_SWCTRL_RESET   (1<<2)


//���ڽ��յ��κ���
#define Dummy_Byte   0xFF

void AD5760_Init(void);                                                  //��ʼ��
void AD5760_EnableOutput(unsigned char state);                           //ʹ�����                          
void AD5760_SetRegisterValue(unsigned char registerAddress,              //���üĴ���ֵ
                             unsigned long registerValue,
                             unsigned char bytesNumber);																																					
void AD5760_SetDacValue(unsigned long value);                             //DAC�Ĵ�������
void Delay(unsigned long time);                                           //��ʱ����
void DelayUs(unsigned long time);
void AD5760_SoftInstruction(unsigned char instructionBit);                //������ƼĴ�������
void AD5760_Setup_CTRLREG(unsigned long setupWord);                       //���ƼĴ�������
void AD5760_Setup_CLRCODE(unsigned long clrCode);                         //�������Ĵ�������
unsigned long AD5760_GetRegisterValue(unsigned char registerAddress,      //��ȡ�Ĵ�����ֵ
                                      unsigned char bytesNumber);



#endif /* __DAC_H */

