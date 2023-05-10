#ifndef __ADC_H
#define	__ADC_H


#include "stm32f10x.h"

//SPI2���Ŷ���
#define ADC_SPI2_PORT   GPIOB
#define ADC_CS_N       GPIO_Pin_12      //PB12
#define ADC_SCLK       GPIO_Pin_13      //PB13
#define ADC_SDI        GPIO_Pin_14      //PB14
#define ADC_SDO        GPIO_Pin_15      //PB15
#define ADC_GPIO0      GPIO_Pin_11      //PB11

//SYNC����ΪPA12
#define ADC_SYNC_ERROR_PORT  GPIOA
#define ADC_SYNC_N           GPIO_Pin_12  //PA12

//CS����ߵ͵�ƽ����
#define ADC_CS_N_HIGH        GPIO_SetBits (ADC_SPI2_PORT,ADC_CS_N)
#define ADC_CS_N_LOW         GPIO_ResetBits (ADC_SPI2_PORT,ADC_CS_N)

//SYNC����ߵ͵�ƽ����
#define ADC_SYNC_N_HIGH      GPIO_SetBits (ADC_SYNC_ERROR_PORT,ADC_SYNC_N)
#define ADC_SYNC_N_LOW       GPIO_ResetBits (ADC_SYNC_ERROR_PORT,ADC_SYNC_N)

#define ADC_CMD_WEN_N_LOW               (0<<7)
#define DAC_CMD_WEN_N_HIGH              (1<<7)
#define ADC_CMD_READ                    (1<<6)
#define ADC_CMD_WRITE                   (0<<6)

/*ͨѶ�ȴ���ʱʱ��*/
#define SPI2T_FLAG_TIMEOUT    ((uint32_t) 0x1000)
//ͨ����Ϣ���
#define ADC_SPI2_ERROR(fmt,arg...)          printf("<<-EEPROM-ERROR->> "fmt"\n",##arg)

//�Ĵ�����ַ�궨��
#define ADC_COMMS_REG_ADDR    0X00    //8bits
#define ADC_STATUS_REG_ADDR   0X00    //8bits
#define ADC_ADCMODE_REG_ADDR  0X01    //16bits
#define ADC_IFMODE_REG_ADDR   0X02    //16bits
#define ADC_REGCHECK_REG_ADDR 0X03    //24bits
#define ADC_DATA_REG_ADDR     0X04    //24bits
#define ADC_GPIOCON_REG_ADDR  0X06    //16bits
//ID�Ĵ�����ֻ���Ĵ���
#define ADC_ID_REG_ADDR       0X07    //16bits
//ͨ���Ĵ��� 16λ
#define ADC_CH0_REG_ADDR      0X10    //16bits
#define ADC_CH1_REG_ADDR      0X11    //16bits
#define ADC_CH2_REG_ADDR      0X12    //16bits
#define ADC_CH3_REG_ADDR      0X13    //16bits
//�������üĴ���  16λ
#define ADC_SETUPCON0_REG_ADDR    0X20      //16bits
#define ADC_SETUPCON1_REG_ADDR    0X21      //16bits
#define ADC_SETUPCON2_REG_ADDR    0X22      //16bits
#define ADC_SETUPCON3_REG_ADDR    0X23      //16bits
//�˲������üĴ���  16λ
#define ADC_FILTCON0_REG_ADDR     0X28      //16bits
#define ADC_FILTCON1_REG_ADDR     0X29      //16bits
#define ADC_FILTCON2_REG_ADDR     0X2A      //16bits
#define ADC_FILTCON3_REG_ADDR     0X2B      //16bits
//ʧ���Ĵ�����24λ
#define ADC_OFFSET0_REG_ADDR      0X30      //24bits
#define ADC_OFFSET1_REG_ADDR      0X31      //24bits
#define ADC_OFFSET2_REG_ADDR      0X32      //24bits
#define ADC_OFFSET3_REG_ADDR      0X33      //24bits
//����Ĵ�����24λ
#define ADC_GAIN0_REG_ADDR        0X38      //24bits
#define ADC_GAIN1_REG_ADDR        0X39      //24bits
#define ADC_GAIN2_REG_ADDR        0X3A      //24bits
#define ADC_GAIN3_REG_ADDR        0X3B      //24bits

//ͨ���Ĵ����Ŀ����ֶ��壬����ѡ��ǰ��Ч��ͨ������ͨ��ʹ����Щ�����Լ���ͨ��ʹ�ú�������������ADC
#define CH_EN               (1<<15)
#define CH_SETUP_SEL(x)     (x<<12)
#define CH_AINPOS(x)        (x<<5)
#define CH_AINNEG(x)        (x<<0)
//��Щλ������ͨ��ʹ�����������е���һ��������ADC��CH0_SETUP_SEL0
#define CH_SETUP0             0
#define CH_SETUP1             1
#define CH_SETUP2             2
#define CH_SETUP3             3
//ͨ����CH0_AINPOS0(x)��CH0_AINNEG0(x)
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

//�������üĴ�������������ADC�Ļ�׼��ѹԴѡ�����뻺������������뷽ʽ
//��λ��������0��ADC�������
#define SETUPCON_BI_UNIPOLAR_SINGLE    (0<<12)      //�����Ա������
#define SETUPCON_BI_UNIPOLAR_DOUBLE    (1<<12)      //˫���Ա��룬��ƫ�ƶ����Ʊ���
#define SETUPCON_REFBUF_POS            (1<<11)      //REF������
#define SETUPCON_REFBUF_NEG            (1<<10)
#define SETUPCON_AINBUF_POS            (1<<9)       //ģ�����뻺����
#define SETUPCON_AINBUF_NEG            (1<<8)
#define SETUPCON_BURNOUT_EN            (1<<7)       //�ɶ�λ
#define SETUPCON_REF_SEL0(x)           (x<<4)
//SETUPCON_REF_SEL0��������0 ADCת����׼��ѹԴ
#define PERREFVOLT                      0     //�ⲿ��׼��ѹ
#define INTERREFVOLT                    2     //�ڲ���׼��ѹ
#define AVDD1_AVSS                      3     //AVDD1-AVSS��������ϣ���֤������׼ֵ

//�˲������üĴ�������������ADC�������ʺ��˲���ѡ��
/*д�����Ĵ����Ḵλ�κ����ڽ��е�ADCת�������´������еĵ�һ��ͨ����ʼת��*/
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
//FILTCON_ENHFILT(x)��ö��
#define ENHFILT_SPS_47                   2
#define ENHFILT_SPS_62                   3
#define ENHFILT_SPS_86                   5
#define ENHFILT_SPS_92                   6
//FILTCON_ORDER(x)��ö��
#define ORDER_SINC5_SINC1                0    //�����˲����Ľ���Ϊsinc5+sinc1
#define ORDER_SINC3                      3    //�����˲����Ľ���Ϊsinc3

//ʧ���Ĵ�������������ADC��ϵͳ�е��κ�ʧ����24λ��Ϊ���õ�ʧ��У׼ϵ��,����У׼�ؾ�

//����Ĵ�������������ADC��ϵͳ�е��κ�������24λ��Ϊ���õ�����У׼ϵ��������У׼б��

//ADCģʽ�Ĵ�������������ADC�Ĺ���ģʽ����ʱ��ѡ��д��ADC�Ĵ����Ḵλ�˲�����RDYλ������ʼ�µ�ת����У׼
#define ADCMODE_REF_EN                  (1<<15)  
#define ADCMODE_HIDE_DELAY              (1<<14)
#define ADCMODE_SING_CYC                (1<<13)
#define ADCMODE_DELAY(x)                (x<<8)       
#define ADCMODE_MODE(x)                 (x<<4)
#define ADCMODE_CLOCKSEL(x)             (x<<2)
//����λ[10:8],DALAY����Щλ����ͨ���л�������һ���ɱ�̵��ӳ�ʱ��
//�Ա��ⲿ��·����ADC��ʼ����������ǰ�ȶ�����
//���ǵ�ͨ��ʱ��������ò��ŵ�
#define ADCMODE_DELAY_0                  0
#define ADCMODE_DELAY_32_Us              1
#define ADCMODE_DELAY_128_Us             2
#define ADCMODE_DELAY_320_Us             3
#define ADCMODE_DELAY_800_Us             4
#define ADCMODE_DELAY_1_6_ms             5
#define ADCMODE_DELAY_4_ms               6
#define ADCMODE_DELAY_8_ms               7
//����λ[6:4],MODE������ADC�Ĺ���ģʽ
#define ADCMDOE_MODE_CONTINU             0   //����ת��
#define ADCMODE_MODE_SINGLE              1   //����ת��
#define ADCMODE_MODE_STANDBY             2   //����ģʽ
#define ADCMODE_MODE_OFF                 3   //�ػ�ģʽ
#define ADCMODE_MODE_INTEROFF            4   //�ڲ�ʧ��У׼
#define ADCMODE_MODE_SYSOFF              6   //ϵͳʧ��У׼
#define ADCMODE_MODE_SYSGAIN             7   //ϵͳ����У׼
//����λ��ADCMODE_CLOCKSEL(x)������ADC��ʱ����Դ
#define ADCMODE_CLOCKSEL_INTER               0   //�ڲ�����
#define ADCMODE_CLOCKSEL_XTAL2_CLKIO_OUT     1   //XTAL2/CLKIO�����ϵ��ڲ��������
#define ADCMODE_CLOCKSEL_XTAL2_CLKIO_IN      2   //XTAL2/CLKIO�����ϵ��ⲿʱ������
#define ADCMODE_CLOCKSEL_EXTER_XTAL          3   //���������ϵ��ⲿ����������Ϊ��ʱ��

//�ӿ�ģʽ�Ĵ�������Ҫ�������ø��ִ��нӿ�ѡ��
#define IFMODE_ALT_SYNC                  (1<<12)   //ʹ��SYNC/ERROR���ŵĲ�ͬ��Ϊ��Ĭ��SYNCΪͬ�����룬��λΪ1ʱ�ɽ�SYNC������һ�ֹ���
#define IFMODE_IOSTRENGTH                (1<<11)   //����DOUT/RDY���ŵ�����ǿ��
#define IFMODE_DOUT_RESET                (1<<8)    //ʹDOUT/RDY�����ӳ���CS��Ϊ�ߵ�ƽ�Żָ�ΪRDY�ź�
#define IFMODE_CONTREAD                  (1<<7)    //ʹ��������ȡADC���ݼĴ�����ADC��������Ϊ����ת��ģʽʱ����ʹ�ܸ�λ
#define IFMODE_DATA_STAT                 (1<<6)    //ʹ״̬�Ĵ��������ڶ�ȡʱ���ӵ����ݼĴ����ϣ�ʹ��ͨ����״̬��Ϣ������һͬ����
#define IFMODE_REG_CHECK                 (1<<5)    //ʹ�ܼĴ��������Լ�飬���ô˼��ɼ����û��Ĵ���ֵ���κα仯
#define IFMODE_CRC_EN(x)                 (x<<2)    //ʹ�ܼĴ�����д��CRC������CRC�Ὣ���нӿڴ�����ֽ�����1
#define IFMODE_WL16_24                   0
#define IFMODE_WL16_16                   1         //��24λ��ADC���ݼĴ���ת����16λ��
//IFMODE_CRC_EN�Ŀ�����ѡ��
#define IFMODE_CRC_DISABLE               0
#define IFMODE_CRC_READ_XOR              1
#define IFMODE_CRC_RW_CRC                2

//���ݼĴ���������ADCת�������Ĭ��Ϊƫ�ƶ����Ʊ��룬����ͨ���������üĴ�����BI_UNIPOLARλ����Ϊ������
//��ȡ���ݼĴ����ὫRDYλ��RDY����������ߣ��ʵ�RDY������һ���½��ص�ʱ��˵�����µ���Ч���ݿ��Զ�ȡ
//���ݼĴ���λ24λ

//GPIO���üĴ���������ADC��ͨ������/�������
#define GPIOCON_NUX_IO                   (1<<12)     //����ADC�����ⲿ��·�����������ڲ�ͨ��˳��ͬ��ʹ��GPIO0/GPIO1
#define GPIOCON_SYNC_EN                  (1<<11)     //��ʹSYNC/ERROR��������ͬ�����룬����SYNC��������ʹ��ADCת��
#define GPIOCON_ERR_EN(x)                (x<<9)      //��Щλ��ʹSYNC/ERROR����������������/���
#define GPIOCON_ERR_DAT                  (1<<8)      //��λ�ɶ���д
#define GPIOCON_IP_EN1                   (1<<5)      //��λ��GPIO1��Ϊ����
#define GPIOCON_IP_EN0                   (1<<4)      //��λ��GPIO0��Ϊ����
#define GPIOCON_OP_EN1                   (1<<3)      //��λ��GPIO1��Ϊ���
#define GPIOCON_OP_EN0                   (1<<2)      //��λ��GPIO1��Ϊ���
#define GPIOCON_GP_DATA1                 (1<<1)
#define GPIOCON_GP_DATA0                 (1<<0)
//GPIOCON_ERR_EN�Ŀ�����ѡ��λ
#define GPIOCON_ERR_DISABLE               0
#define GPIOCON_ERR_INPUT                 1   //��������
#define GPIOCON_ERR_OD_OUT                2   //��©�������
#define GPIOCON_ERR_OUT                   3   //ͨ�����


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
//����ؼĴ����Ĳ���
unsigned long AD7172_ReadID(void);
void AD7172_SetAdcChannal(unsigned char CHregisterAddress,unsigned long setupsel,unsigned long ainpos,unsigned long ainneg);
void AD7172_SETUPCONReg(unsigned char SETUPCONregaddr,unsigned long setword);
void AD7172_FILTCONReg(unsigned long FiltConregaddr,unsigned long setword);
void AD7172_GAINReg(unsigned long GAINregaddr,unsigned long setword);
void AD7172_OFFSETReg(unsigned long OFFSETregaddr,unsigned long setword);
void AD7172_ADCMODEReg(unsigned long setword);
void AD7172_IFMODEReg(unsigned long setword);
unsigned long AD7172_ReadFromDATAReg_1(void);    //����ת��
unsigned long AD7172_ReadFromDATAReg_2(void);    //������ȡ
void AD7172_Reset(void);
unsigned long AD7172_OFFSET_calibration(void);
unsigned long AD7172_GAIN_calibration(void);



#endif /* __ADC_H */

