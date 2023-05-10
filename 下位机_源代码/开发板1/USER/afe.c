/************************************
 * �ļ���  ��afe.c
 * ����    ������ģ��SPI����ADA4350�Ŀ�������·�����Ӷ�ѡ��ͬ������         
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�һ��7�����ţ�ģ��SPI
             AFE_SCLK     PB9
						 AFE_SDO      PB8
						 AFE_SDI      PB7
						 AFE_CS_N     PB6
						 AFE_LATCH_N  PB5
						 AFE_EN       PB0
						 AFE_MODE     PA3
**********************************************************************************/

#include "afe.h"
#include "dac.h"
#include "adc.h"
#include "usart1.h"

/**
  * @brief  AFE�˿����ã�һ��7������
  * @retval : none
  */
void ADA4350_SIM_SPI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* ʹ��GPIOA��GPIOB��ʱ�ӣ���ΪGPIOA��GPIOB����APB2ʱ������*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin=AFE_SCLK;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_SIM_SDO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_SIM_SDI;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //��Ϊ�õ���ģ��SPI,Ӧ������Ϊ�������룬
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_CS_N;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_LATCH_N;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_EN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_MODE;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������Ϊ�õ���ģ��SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_MODE_PORT,&GPIO_InitStructure);
	
	AFE_SPI_MODE;      //����Ϊģ��SPI����ģʽ
	AFE_SPI_LATCH_N_HIGH; //��Ϊ�ߵ�ƽ
	AFE_SPI_CS_N_HIGH; //��ʼʱ����Ϊ����״̬
	AFE_SPI_SCLK_LOW;  //����״̬ʱ��SCLKΪ�͵�ƽ
}

/*****************************************************************************************/
	/**
  * @brief  ���ͺͽ���һ���ֽڣ�ͬʱ����ͬʱ����
  * @param  feedback��д��Ĵ�����24λ����
 *                 
 *                         
  * @retval : ����ͬʱ���յ���ֵ
  */
u32 AFE_SIM_SPI_Write(u32 feedback)
{
	int i=0;
	u32 readdata=0;
	u8 temp1=0;
	u8 temp2=0;
	
	AFE_SPI_CS_N_LOW;   //д���ڿ�ʼ
	
	for(i=23;i>=0;i--)
	{
		AFE_SPI_SCLK_HIGH;
		
		DelayUs(1);
		temp1=bit_from24(feedback,i);
		if(temp1==1)   //Ҫ���͵�����Ϊ1
		{
			AFE_SPI_SD0_HIGH;   //��SD0���1
		}
		else
			AFE_SPI_SDO_LOW;   //SDO���0
		
		temp2=GPIO_ReadInputDataBit(AFE_SPI_PORT,AFE_SIM_SDI);
		readdata=((temp2<<i)&(1<<i))|readdata;    //��ȡͬʱ������������
		DelayUs(1);
		
		AFE_SPI_SCLK_LOW;
	}
	
	AFE_SPI_CS_N_HIGH;   //д���ڽ���	
	
	return readdata;
	
}

/*****************************************************************************************/
	/**
  * @brief  ȡ��24λ�е�����һλ
  * @param  data:Ҫ��ȡ����
 *          i��Ҫ��ȡ�ڼ�λ       
 *                         
  * @retval : ���ظ�λ��ֵ��ҪôΪ1��ҪôΪ0
  */
u8 bit_from24(u32 data,int i)
{
	return (data>>i)&1;     
}
/*****************************************************************************************/
	/**
  * @brief  ���պ�����ֱ�ӵ��÷��ͺ�������
  * @param  none
 *                 
 *                         
  * @retval : ����ͬʱ���յ���ֵ
  */
u32 AFE_SIM_SPI_Read(void)
{
	return (AFE_SIM_SPI_Write(0x00));
	
}

/*****************************************************************************************/
	/**
  * @brief  ��Ĵ���д����
  * @param  none
 *                 
 *                         
  * @retval : ����ͬʱ���յ���ֵ
  */
void AFE_WriteToReg(u32 feedback)
{
	
	AFE_SIM_SPI_Write(feedback);
	DelayUs(1);
	AFE_SPI_LATCH_N_LOW; //��Ϊ�͵�ƽ
	DelayUs(1);
	AFE_SPI_LATCH_N_HIGH; //��Ϊ�ߵ�ƽ
}
/*****************************************************************************************/
	/**
  * @brief  ��Ĵ���������
  * @param  none
 *                 
 *                         
  * @retval : ����ͬʱ���յ���ֵ
  */
u32 AFE_ReadFromReg(void)
{
	u32 readdata=0;
	AFE_SIM_SPI_Write(READCOMMAND);  //�ȷ��Ͷ�����
	DelayUs(1);
	readdata=AFE_SIM_SPI_Read();     //��������������
	return readdata;
	
}
//����AFE�����棬gain���˹������ֵ
/*֮�����У׼����Ҫ���迹ģ�ͣ����ǲ���ÿ��һ�����̾ͻ�һ�ΰ��ӣ�����Ӧ�����迹ģ�Ͱ��ϰ�ÿһ�����̶���ã�����ֵд��*/
void AFE_gain_cal(unsigned long *cal_value)
{

		cal_value[0]=AD7172_OFFSET_calibration();   //cal_value[0]��ʧ��ϵ��������ʧ��У׼�󷵻ظ�ϵ��
		cal_value[1]=AD7172_GAIN_calibration();     //cal_value[1]������ϵ��
		AD7172_OFFSETReg(ADC_OFFSET0_REG_ADDR,cal_value[0]);  //Ӧ���Ƚ���ʧ�����
		AD7172_GAINReg(ADC_GAIN0_REG_ADDR,cal_value[1]);
	
}
//����AFE�����棬gain���˹������ֵ
/*֮�����У׼����Ҫ���迹ģ�ͣ����ǲ���ÿ��һ�����̾ͻ�һ�ΰ��ӣ�����Ӧ�����迹ģ�Ͱ��ϰ�ÿһ�����̶���ã�����ֵд��*/
void AFE_gain_nocal(unsigned long *cal_value)
{
	AD7172_OFFSETReg(ADC_OFFSET0_REG_ADDR,cal_value[0]);  //Ӧ���Ƚ���ʧ�����
	AD7172_GAINReg(ADC_GAIN0_REG_ADDR,cal_value[1]);
}

//������������
int AFE_Choosegain(int gain)
{
	
	int flag=1;   //��ʾ����������ȷ����Ϊ0��ʾ�������ô���
	if(gain==1000)
	{
		AFE_WriteToReg(GAIN_1K);             //��������
	}
	//����570
	else if(gain==5000)
	{
		AFE_WriteToReg(GAIN_5K);             //��������
	}
	else if(gain==10000)
	{
		AFE_WriteToReg(GAIN_10K);             //��������

	}
	else if(gain==50000)
	{
		AFE_WriteToReg(GAIN_50K);             //��������
	}
	else if(gain==100000)
	{
		AFE_WriteToReg(GAIN_100K);             //��������
	}
	else if(gain==1000000)
	{
		AFE_WriteToReg(GAIN_1M);             //��������
	}
	else
	{
		
		flag=0;
		
	}
	
	return flag;
}

