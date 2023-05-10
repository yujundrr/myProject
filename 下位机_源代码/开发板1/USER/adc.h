#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

//SPI2引脚定义
#define ADC_SPI2_PORT   GPIOB
#define ADC_CS_N       GPIO_Pin_12      //PB12
#define ADC_SCLK       GPIO_Pin_13      //PB13
#define ADC_SDI        GPIO_Pin_14      //PB14
#define ADC_SDO        GPIO_Pin_15      //PB15
#define ADC_GPIO0      GPIO_Pin_11      //PB11

//SYNC引脚为PA12
#define ADC_SYNC_ERROR_PORT  GPIOA
#define ADC_SYNC_N           GPIO_Pin_12  //PA12

//CS输出高低电平控制
#define ADC_CS_N_HIGH        GPIO_SetBits (ADC_SPI2_PORT,ADC_CS_N)
#define ADC_CS_N_LOW         GPIO_ResetBits (ADC_SPI2_PORT,ADC_CS_N)

//SYNC输出高低电平控制
#define ADC_SYNC_N_HIGH      GPIO_SetBits (ADC_SYNC_ERROR_PORT,ADC_SYNC_N)
#define ADC_SYNC_N_LOW       GPIO_ResetBits (ADC_SYNC_ERROR_PORT,ADC_SYNC_N)

#define ADC_CMD_WEN_N_LOW               (0<<7)
#define DAC_CMD_WEN_N_HIGH              (1<<7)
#define ADC_CMD_READ                    (1<<6)
#define ADC_CMD_WRITE                   (0<<6)

/*通讯等待超时时间*/
#define SPI2T_FLAG_TIMEOUT    ((uint32_t) 0x1000)
//通信信息输出
#define ADC_SPI2_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)

//寄存器地址宏定义
#define ADC_COMMS_REG_ADDR    0X00    //8bits
#define ADC_STATUS_REG_ADDR   0X00    //8bits
#define ADC_ADCMODE_REG_ADDR  0X01    //16bits
#define ADC_IFMODE_REG_ADDR   0X02    //16bits
#define ADC_REGCHECK_REG_ADDR 0X03    //24bits
#define ADC_DATA_REG_ADDR     0X04    //24bits
#define ADC_GPIOCON_REG_ADDR  0X06    //16bits
//ID寄存器，只读寄存器
#define ADC_ID_REG_ADDR       0X07    //16bits
//通道寄存器 16位
#define ADC_CH0_REG_ADDR      0X10    //16bits
#define ADC_CH1_REG_ADDR      0X11    //16bits
#define ADC_CH2_REG_ADDR      0X12    //16bits
#define ADC_CH3_REG_ADDR      0X13    //16bits
//设置配置寄存器  16位
#define ADC_SETUPCON0_REG_ADDR    0X20      //16bits
#define ADC_SETUPCON1_REG_ADDR    0X21      //16bits
#define ADC_SETUPCON2_REG_ADDR    0X22      //16bits
#define ADC_SETUPCON3_REG_ADDR    0X23      //16bits
//滤波器配置寄存器  16位
#define ADC_FILTCON0_REG_ADDR     0X28      //16bits
#define ADC_FILTCON1_REG_ADDR     0X29      //16bits
#define ADC_FILTCON2_REG_ADDR     0X2A      //16bits
#define ADC_FILTCON3_REG_ADDR     0X2B      //16bits
//失调寄存器，24位
#define ADC_OFFSET0_REG_ADDR      0X30      //24bits
#define ADC_OFFSET1_REG_ADDR      0X31      //24bits
#define ADC_OFFSET2_REG_ADDR      0X32      //24bits
#define ADC_OFFSET3_REG_ADDR      0X33      //24bits
//增益寄存器，24位
#define ADC_GAIN0_REG_ADDR        0X38      //24bits
#define ADC_GAIN1_REG_ADDR        0X39      //24bits
#define ADC_GAIN2_REG_ADDR        0X3A      //24bits
#define ADC_GAIN3_REG_ADDR        0X3B      //24bits

//通道寄存器的控制字定义，用于选择当前有效的通道、各通道使用哪些输入以及该通道使用何种设置来配置ADC
#define CH_EN               (1<<15)
#define CH_SETUP_SEL(x)     (x<<12)
#define CH_AINPOS(x)        (x<<5)
#define CH_AINNEG(x)        (x<<0)
//这些位决定该通道使用四种设置中的哪一种来配置ADC，CH0_SETUP_SEL0
#define CH_SETUP0             0
#define CH_SETUP1             1
#define CH_SETUP2             2
#define CH_SETUP3             3
//通道，CH0_AINPOS0(x)，CH0_AINNEG0(x)
#define AIN0                   0
#define AIN1                   1
#define AIN2                   2
#define AIN3                   3
#define AIN4                   4
#define TEMP_SENSORPOS         17
#define TEMP_SENSORNEG         18
#define AVDD1_AVSS_5_POS       19
#define AVDD1_AVSS_5_NEG       20
#define REF_POS                21
#define REF_NEG                22

//设置配置寄存器，用于配置ADC的基准电压源选择、输入缓冲器和输出编码方式
//此位设置设置0的ADC输出编码
#define SETUPCON_BI_UNIPOLAR_SINGLE    (0<<12)      //单极性编码输出
#define SETUPCON_BI_UNIPOLAR_DOUBLE    (1<<12)      //双极性编码，即偏移二进制编码
#define SETUPCON_REFBUF_POS            (1<<11)      //REF缓冲器
#define SETUPCON_REFBUF_NEG            (1<<10)
#define SETUPCON_AINBUF_POS            (1<<9)       //模拟输入缓冲器
#define SETUPCON_AINBUF_NEG            (1<<8)
#define SETUPCON_BURNOUT_EN            (1<<7)       //可读位
#define SETUPCON_REF_SEL0(x)           (x<<4)
//SETUPCON_REF_SEL0用于设置0 ADC转换基准电压源
#define PERREFVOLT                      0     //外部基准电压
#define INTERREFVOLT                    2     //内部基准电压
#define AVDD1_AVSS                      3     //AVDD1-AVSS，用于诊断，验证其他基准值

//滤波器配置寄存器，用于配置ADC数据速率和滤波器选项
/*写入此类寄存器会复位任何正在进行的ADC转换，重新从序列中的第一个通道开始转换*/
#define FILTCON_SINC3_MAP              (1<<15)
#define FILTCON_ENHFILTEN              (1<<11)
#define FILTCON_ENHFILT(x)             (x<<8)
#define FILTCON_ORDER(x)               (x<<5)   
#define FILTCON_ODR_31250              0
#define FILTCON_ODR_15625              6
#define FILTCON_ODR_10417              7
#define FILTCON_ODR_5208               8
#define FILTCON_ODR_2597               9
#define FILTCON_ODR_1007               10
#define FILTCON_ODR_503_8              11
#define FILTCON_ODR_381                12
#define FILTCON_ODR_200                13
#define FILTCON_ODR_100                14
#define FILTCON_ODR_59                 15
#define FILTCON_ODR_49                 16
#define FILTCON_ODR_20                 17
#define FILTCON_ODR_16                 18
#define FILTCON_ODR_10                 19
#define FILTCON_ODR_5                  20
#define FILTCON_ODR_2                  21
#define FILTCON_ODR_1                  22
//FILTCON_ENHFILT(x)的枚举
#define ENHFILT_SPS_47                   2
#define ENHFILT_SPS_62                   3
#define ENHFILT_SPS_86                   5
#define ENHFILT_SPS_92                   6
//FILTCON_ORDER(x)的枚举
#define ORDER_SINC5_SINC1                0    //数字滤波器的阶数为sinc5+sinc1
#define ORDER_SINC3                      3    //数字滤波器的阶数为sinc3

//失调寄存器，用来补偿ADC或系统中的任何失调误差，24位均为设置的失调校准系数,用于校准截距

//增益寄存器，用来补偿ADC或系统中的任何增益误差，24位均为设置的增益校准系数，用于校准斜率

//ADC模式寄存器，用来控制ADC的工作模式和主时钟选择，写入ADC寄存器会复位滤波器和RDY位，并开始新的转换或校准
#define ADCMODE_REF_EN                  (1<<15)  
#define ADCMODE_HIDE_DELAY              (1<<14)
#define ADCMODE_SING_CYC                (1<<13)
#define ADCMODE_DELAY(x)                (x<<8)       
#define ADCMODE_MODE(x)                 (x<<4)
#define ADCMODE_CLOCKSEL(x)             (x<<2)
//控制位[10:8],DALAY：这些位允许通道切换后增加一个可编程的延迟时间
//以便外部电路能在ADC开始处理其输入前稳定下来
//若是单通道时，这个是用不着的
#define ADCMODE_DELAY_0                  0
#define ADCMODE_DELAY_32_Us              1
#define ADCMODE_DELAY_128_Us             2
#define ADCMODE_DELAY_320_Us             3
#define ADCMODE_DELAY_800_Us             4
#define ADCMODE_DELAY_1_6_ms             5
#define ADCMODE_DELAY_4_ms               6
#define ADCMODE_DELAY_8_ms               7
//控制位[6:4],MODE，控制ADC的工作模式
#define ADCMDOE_MODE_CONTINU             0   //连续转换
#define ADCMODE_MODE_SINGLE              1   //单次转换
#define ADCMODE_MODE_STANDBY             2   //待机模式
#define ADCMODE_MODE_OFF                 3   //关机模式
#define ADCMODE_MODE_INTEROFF            4   //内部失调校准
#define ADCMODE_MODE_SYSOFF              6   //系统失调校准
#define ADCMODE_MODE_SYSGAIN             7   //系统增益校准
//控制位，ADCMODE_CLOCKSEL(x)，控制ADC的时钟来源
#define ADCMODE_CLOCKSEL_INTER               0   //内部振荡器
#define ADCMODE_CLOCKSEL_XTAL2_CLKIO_OUT     1   //XTAL2/CLKIO引脚上的内部振荡器输出
#define ADCMODE_CLOCKSEL_XTAL2_CLKIO_IN      2   //XTAL2/CLKIO引脚上的外部时钟输入
#define ADCMODE_CLOCKSEL_EXTER_XTAL          3   //两个引脚上的外部晶振输入作为主时钟

//接口模式寄存器，主要用来配置各种串行接口选项
#define IFMODE_ALT_SYNC                  (1<<12)   //使能SYNC/ERROR引脚的不同行为，默认SYNC为同步输入，此位为1时可将SYNC置于另一种功能
#define IFMODE_IOSTRENGTH                (1<<11)   //控制DOUT/RDY引脚的驱动强度
#define IFMODE_DOUT_RESET                (1<<8)    //使DOUT/RDY引脚延迟至CS变为高电平才恢复为RDY信号
#define IFMODE_CONTREAD                  (1<<7)    //使能连续读取ADC数据寄存器，ADC必须配置为连续转换模式时才能使能该位
#define IFMODE_DATA_STAT                 (1<<6)    //使状态寄存器可以在读取时附加到数据寄存器上，使得通道和状态信息与数据一同传输
#define IFMODE_REG_CHECK                 (1<<5)    //使能寄存器完整性检查，利用此检查可监视用户寄存器值的任何变化
#define IFMODE_CRC_EN(x)                 (x<<2)    //使能寄存器读写的CRC保护，CRC会将串行接口传输的字节数加1
#define IFMODE_WL16_24                   0
#define IFMODE_WL16_16                   1         //将24位的ADC数据寄存器转换成16位的
//IFMODE_CRC_EN的控制字选择
#define IFMODE_CRC_DISABLE               0
#define IFMODE_CRC_READ_XOR              1
#define IFMODE_CRC_RW_CRC                2

//数据寄存器，包含ADC转换结果，默认为偏移二进制编码，可以通过设置配置寄存器的BI_UNIPOLAR位更改为单极性
//读取数据寄存器会将RDY位和RDY引脚输出拉高，故当RDY引脚有一个下降沿的时候说明有新的有效数据可以读取
//数据寄存器位24位

//GPIO配置寄存器，控制ADC的通用输入/输出引脚
#define GPIOCON_NUX_IO                   (1<<12)     //允许ADC控制外部多路复用器，与内部通道顺序同步使用GPIO0/GPIO1
#define GPIOCON_SYNC_EN                  (1<<11)     //可使SYNC/ERROR引脚用作同步输入，这是SYNC的上升沿使能ADC转换
#define GPIOCON_ERR_EN(x)                (x<<9)      //这些位可使SYNC/ERROR引脚用作错误输入/输出
#define GPIOCON_ERR_DAT                  (1<<8)      //此位可读可写
#define GPIOCON_IP_EN1                   (1<<5)      //此位将GPIO1变为输入
#define GPIOCON_IP_EN0                   (1<<4)      //此位将GPIO0变为输入
#define GPIOCON_OP_EN1                   (1<<3)      //此位将GPIO1变为输出
#define GPIOCON_OP_EN0                   (1<<2)      //此位将GPIO1变为输出
#define GPIOCON_GP_DATA1                 (1<<1)
#define GPIOCON_GP_DATA0                 (1<<0)
//GPIOCON_ERR_EN的控制字选择位
#define GPIOCON_ERR_DISABLE               0
#define GPIOCON_ERR_INPUT                 1   //错误输入
#define GPIOCON_ERR_OD_OUT                2   //开漏错误输出
#define GPIOCON_ERR_OUT                   3   //通用输出


 void ADC_SPI2_Init(void);
 u8 SPI2_ReadWrite_CTRL(unsigned char RWCtrldata);
 u8 SPI2_Write(unsigned char *data,unsigned char bytesNumber);
 unsigned long SPI2_Read(unsigned char bytesNumber);
 void AD7172_Set_COMMSREG_Value(unsigned char RW,unsigned char registerAddress);
 void AD7172_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber);
unsigned long AD7172_GetRegisterValue(unsigned char registerAddress,
                                       unsigned char bytesNumber);
//对相关寄存器的操作
unsigned long AD7172_ReadID(void);
void AD7172_SetAdcChannal(unsigned char CHregisterAddress,unsigned long setupsel,unsigned long ainpos,unsigned long ainneg);
void AD7172_SETUPCONReg(unsigned char SETUPCONregaddr,unsigned long setword);
void AD7172_FILTCONReg(unsigned long FiltConregaddr,unsigned long setword);
void AD7172_GAINReg(unsigned long GAINregaddr,unsigned long setword);
void AD7172_OFFSETReg(unsigned long OFFSETregaddr,unsigned long setword);
void AD7172_ADCMODEReg(unsigned long setword);
void AD7172_IFMODEReg(unsigned long setword);
unsigned long AD7172_ReadFromDATAReg_1(void);    //连续转换
unsigned long AD7172_ReadFromDATAReg_2(void);    //连续读取
void AD7172_Reset(void);
unsigned long AD7172_OFFSET_calibration(void);
unsigned long AD7172_GAIN_calibration(void);



#endif /* __ADC_H */

