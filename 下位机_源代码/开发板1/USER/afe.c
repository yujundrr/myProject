/************************************
 * 文件名  ：afe.c
 * 描述    ：根据模拟SPI设置ADA4350的跨阻增益路径，从而选择不同的增益         
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 硬件连接：一共7个引脚，模拟SPI
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
  * @brief  AFE端口配置，一共7个引脚
  * @retval : none
  */
void ADA4350_SIM_SPI_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;
	/* 使能GPIOA和GPIOB的时钟，因为GPIOA和GPIOB都在APB2时钟树上*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	
	GPIO_InitStructure.GPIO_Pin=AFE_SCLK;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_SIM_SDO;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_SIM_SDI;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING; //因为用的是模拟SPI,应该配置为浮空输入，
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_CS_N;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_LATCH_N;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_EN;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_SPI_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=AFE_MODE;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为用的是模拟SPI
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(AFE_MODE_PORT,&GPIO_InitStructure);
	
	AFE_SPI_MODE;      //配置为模拟SPI工作模式
	AFE_SPI_LATCH_N_HIGH; //设为高电平
	AFE_SPI_CS_N_HIGH; //初始时设置为空闲状态
	AFE_SPI_SCLK_LOW;  //空闲状态时，SCLK为低电平
}

/*****************************************************************************************/
	/**
  * @brief  发送和接收一个字节，同时接收同时发送
  * @param  feedback：写入寄存器的24位数据
 *                 
 *                         
  * @retval : 返回同时接收到的值
  */
u32 AFE_SIM_SPI_Write(u32 feedback)
{
	int i=0;
	u32 readdata=0;
	u8 temp1=0;
	u8 temp2=0;
	
	AFE_SPI_CS_N_LOW;   //写周期开始
	
	for(i=23;i>=0;i--)
	{
		AFE_SPI_SCLK_HIGH;
		
		DelayUs(1);
		temp1=bit_from24(feedback,i);
		if(temp1==1)   //要发送的数据为1
		{
			AFE_SPI_SD0_HIGH;   //让SD0输出1
		}
		else
			AFE_SPI_SDO_LOW;   //SDO输出0
		
		temp2=GPIO_ReadInputDataBit(AFE_SPI_PORT,AFE_SIM_SDI);
		readdata=((temp2<<i)&(1<<i))|readdata;    //读取同时接收来的数据
		DelayUs(1);
		
		AFE_SPI_SCLK_LOW;
	}
	
	AFE_SPI_CS_N_HIGH;   //写周期结束	
	
	return readdata;
	
}

/*****************************************************************************************/
	/**
  * @brief  取出24位中的任意一位
  * @param  data:要提取的数
 *          i：要提取第几位       
 *                         
  * @retval : 返回该位的值，要么为1，要么为0
  */
u8 bit_from24(u32 data,int i)
{
	return (data>>i)&1;     
}
/*****************************************************************************************/
	/**
  * @brief  接收函数，直接调用发送函数即可
  * @param  none
 *                 
 *                         
  * @retval : 返回同时接收到的值
  */
u32 AFE_SIM_SPI_Read(void)
{
	return (AFE_SIM_SPI_Write(0x00));
	
}

/*****************************************************************************************/
	/**
  * @brief  向寄存器写数据
  * @param  none
 *                 
 *                         
  * @retval : 返回同时接收到的值
  */
void AFE_WriteToReg(u32 feedback)
{
	
	AFE_SIM_SPI_Write(feedback);
	DelayUs(1);
	AFE_SPI_LATCH_N_LOW; //设为低电平
	DelayUs(1);
	AFE_SPI_LATCH_N_HIGH; //设为高电平
}
/*****************************************************************************************/
	/**
  * @brief  向寄存器读数据
  * @param  none
 *                 
 *                         
  * @retval : 返回同时接收到的值
  */
u32 AFE_ReadFromReg(void)
{
	u32 readdata=0;
	AFE_SIM_SPI_Write(READCOMMAND);  //先发送读命令
	DelayUs(1);
	readdata=AFE_SIM_SPI_Read();     //接下来接收数据
	return readdata;
	
}
//设置AFE的增益，gain是人工输入的值
/*之后进行校准，需要用阻抗模型，但是不能每换一个量程就换一次板子，所以应该在阻抗模型板上把每一个量程都测好，并把值写好*/
void AFE_gain_cal(unsigned long *cal_value)
{

		cal_value[0]=AD7172_OFFSET_calibration();   //cal_value[0]是失调系数，经过失调校准后返回该系数
		cal_value[1]=AD7172_GAIN_calibration();     //cal_value[1]是增益系数
		AD7172_OFFSETReg(ADC_OFFSET0_REG_ADDR,cal_value[0]);  //应该先进行失调误差
		AD7172_GAINReg(ADC_GAIN0_REG_ADDR,cal_value[1]);
	
}
//设置AFE的增益，gain是人工输入的值
/*之后进行校准，需要用阻抗模型，但是不能每换一个量程就换一次板子，所以应该在阻抗模型板上把每一个量程都测好，并把值写好*/
void AFE_gain_nocal(unsigned long *cal_value)
{
	AD7172_OFFSETReg(ADC_OFFSET0_REG_ADDR,cal_value[0]);  //应该先进行失调误差
	AD7172_GAINReg(ADC_GAIN0_REG_ADDR,cal_value[1]);
}

//纯粹设置增益
int AFE_Choosegain(int gain)
{
	
	int flag=1;   //表示设置增益正确，若为0表示增益设置错误
	if(gain==1000)
	{
		AFE_WriteToReg(GAIN_1K);             //设置增益
	}
	//设置570
	else if(gain==5000)
	{
		AFE_WriteToReg(GAIN_5K);             //设置增益
	}
	else if(gain==10000)
	{
		AFE_WriteToReg(GAIN_10K);             //设置增益

	}
	else if(gain==50000)
	{
		AFE_WriteToReg(GAIN_50K);             //设置增益
	}
	else if(gain==100000)
	{
		AFE_WriteToReg(GAIN_100K);             //设置增益
	}
	else if(gain==1000000)
	{
		AFE_WriteToReg(GAIN_1M);             //设置增益
	}
	else
	{
		
		flag=0;
		
	}
	
	return flag;
}

