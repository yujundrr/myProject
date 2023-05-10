/************************************
 * 文件名  ：adc.c
 * 描述    ：基于AD7172-2芯片,用的SPI2进行通信    
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 硬件连接： 
**********************************************************************************/
#include "adc.h"
#include "dac.h"



/**
  * @brief  ADC的SPI2的四个串行引脚设置,SYNC引脚应该怎么设置,暂时先把SYNC设置为普通推挽输出
  * @retval : none
  */
static void ADC_SPI2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    //GPIO_InitTypeDef是一个结构，包括GPIO_Pin、GPIO_Mode、GPIO_Speed三个枚举量
	/* 使能GPIOA和GPIOB的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	
	//CS,PB12
	GPIO_InitStructure.GPIO_Pin=ADC_CS_N;          
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //普通推挽输出，因为NSS使用的是软件控制
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADC_SPI2_PORT,&GPIO_InitStructure);
	
	/*SPI2 SCLK,PB13*/
	GPIO_InitStructure.GPIO_Pin = ADC_SCLK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure); 
	
	/*单片机的SPI2 SDI，ADC的SDO,PB14*/
	GPIO_InitStructure.GPIO_Pin = ADC_SDI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure); 
	
	/*单片机的SPI2 SDO，DAC的SDI,PB15*/
	GPIO_InitStructure.GPIO_Pin = ADC_SDO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure);
	
	/*ADC的SYNC引脚，PA12，这个引脚相当于是启动ADC转换的使能引脚*/
	GPIO_InitStructure.GPIO_Pin = ADC_SYNC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(ADC_SYNC_ERROR_PORT, &GPIO_InitStructure);
	
	//刚开始处于空闲状态
	ADC_CS_N_HIGH;
	//SYNC引脚为同步输入引脚，SYNC的上升沿使能ADC的转换
	ADC_SYNC_N_LOW;
	
}

/*配置SPI2的工作模式  */
static void ADC_SPI2_Mode_Config(void)
{
	//定义一个SPI的初始化结构体
	 SPI_InitTypeDef  SPI_InitStructure;
	
	//打开SPI2的时钟,SPI2在APB1时钟树上,APB1的时钟为36M
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	/*AD7172-2*/
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;   //SCLK时钟频率为36/8=4.5M
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //偶数边沿采样
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                          //CS为高时，SCLK为高电平
	SPI_InitStructure.SPI_CRCPolynomial=7;                             //CRC无效
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //8位数据
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //全双工
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //MSB先行
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //主模式
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //开始信号和停止信号由软件产生
	//初始化
	SPI_Init(SPI2,&SPI_InitStructure);
	//使能SPI
	SPI_Cmd(SPI2,ENABLE);
	
}

 void ADC_SPI2_Init(void)
 {
	 //只要调用这个函数就可以
	 ADC_SPI2_GPIO_Config();
	 ADC_SPI2_Mode_Config();
 }
 
 
 /******************************************************************/
 /**
  * @brief  SPI2等待事件超时的情况下会调用这个函数来处理，
            与下面发送一个字节的函数一起作用，可以得知在发送一个字节时是否错误，且根据errorCode能够得知
  * @param  errorCode:错误代码。可以用来定位是哪个环节出错
  * @retval 返回0，表示SPI读取失败
  */
 static u8 SPI2_TIMEOUT_UserCallback(uint8_t errorCode)
{
	//使用串口printf输出错误信息，方便调试
	ADC_SPI2_ERROR("SPI等待超时！errorCode=%d",errorCode);
	return 0;
}

/************************************************************************
  * @brief  ADC的通信寄存器对ADC全部寄存器映射的访问，通信寄存器是只写寄存器,读写控制
  * 通信寄存器的八位：7：WEN_N(要与ADC开始通信，此位必须为低电平);6：R/W;5-0:Address
  * @param  RWCtrldata:写入通信寄存器的8位数据
  * @retval : 返回错误信息
  */
u8 SPI2_ReadWrite_CTRL(unsigned char RWCtrldata)
{

	unsigned char recieveData;    //收到的8bits数据
	uint32_t SPITimeout;          //超时变量         
	
	//开始同时接收和同时发送
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_TXE=1时表示发送缓冲器为空，可以写入数据
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(2);
	}
	/*发送数据*/
	SPI_I2S_SendData(SPI2,RWCtrldata);

	//接收数据
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_RXNE=1时代表接收缓冲器不为空，即是有数据传来
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(3);
	}
	/*接收数据*/
	recieveData=SPI_I2S_ReceiveData(SPI2);

	return 1;
}

 /*********************************************************
  * @brief  使用SPI2发送数据，SPI写函数
  * @param  data:要发送的数据，封装好的数据数组，数组长度为4
  * @param  bytesNumber:要发送的数据字节大小,在ADC内可以是1，2，3
  * @retval : 返回错误信息
  */
u8 SPI2_Write(unsigned char *data,unsigned char bytesNumber)
{
	int i=0;
	unsigned char recieveData[9]={0};    //收到的数据
	uint32_t SPITimeout;                 //超时变量

	//按照要写入的字节个数进行写入
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPI2T_FLAG_TIMEOUT;
		//当SPI_I2S_FLAG_TXE=1时表示发送缓冲器为空，可以写入数据
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(4);
	}
	/*发送数据*/
	SPI_I2S_SendData(SPI2,data[i]);

	//接收数据
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_RXNE=1时代表接收缓冲器不为空，即是有数据传来
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(5);
	}
	/*接收数据*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI2);
  }
	
//	DelayUs(1);
//	//产生停止信号
//	ADC_CS_N_HIGH;
	return 1;
}

 /*********************************************************
  * @brief  从SPI2读取数据,数据长度可以为1，2，3
  * @param  bytesNumber:要接收的数据字节大小
  * @retval : 返回读取的数据
  */
/*! Reads data from SPI. */
unsigned long SPI2_Read(unsigned char bytesNumber)
{
	int i=0;
	uint32_t SPITimeout;      //超时变量
	unsigned char recieveData[4]={0};    //收到的数据
	unsigned long dataRead=0;           //将其转换为一个32位的数据
	unsigned char readback[4]={0,0,0,0};      //NOP
	

	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPI2T_FLAG_TIMEOUT;
		//当SPI_I2S_FLAG_TXE=1时表示发送缓冲器为空，可以写入数据
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(6);
	}
	/*发送数据*/
	SPI_I2S_SendData(SPI2,readback[i]);

	
	//接收数据
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_RXNE=1时代表接收缓冲器不为空，即是有数据传来
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(7);
	}
	/*接收数据*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI2);
  }
	
	for(i = 1;i <= bytesNumber;i ++) 
    {
        dataRead = (dataRead<<8) + recieveData[i];   //registerWord[0]是存储的高字节
    }
	/*成功发送返回1*/
	return dataRead;
}

/*****************************************************************************************/
	/**
  * @brief  向通信寄存器写8位指令，通信寄存器的地址为0X00，表示对下一个寄存器读或者写
  * @param  RW:读写位;1:读；0:写
  * @param  registerAddress:要对哪个寄存器进行操作
  * @retval : none
  */
void AD7172_Set_COMMSREG_Value(unsigned char RW,unsigned char registerAddress)
{
	unsigned char comdata=0;   //要写入COMMS寄存器的值
	
	comdata=ADC_CMD_WEN_N_LOW|RW|registerAddress;
	
	//写入
	SPI2_ReadWrite_CTRL(comdata);	
}

/*****************************************************************************************/
	/**
  * @brief  向某一个寄存器进行写操作
  * @param  registerValue:需要在各个寄存器配置函数中调用这个函数，故这个值是一件设置好了的
  * @param  registerAddress:要对哪个寄存器进行操作
  * @param  bytesNumber：要写入的字节数，由具体寄存器决定
  * @retval : none
  */
void AD7172_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber)
{
	unsigned char writeCommand[4] = {0, 0, 0, 0};      //要写入的数据
	unsigned long spiWord         = 0;
	unsigned char* dataPointer    = (unsigned char*)&spiWord;  //分别指向4个字节的地址
	unsigned char bytesNr         = bytesNumber;               //后面还要用到，所以多存储
	
	spiWord=registerValue;    //根据控制字进行拼接来设置
	
	
	writeCommand[0] = 0x01;  //这个值不影响，因为只发后三个字节，每个寄存器的大小最大为24Bits
	while(bytesNr != 0)
    {
        writeCommand[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    } 
		//产生开始信号
	ADC_CS_N_LOW;
	//先发送写控制
	AD7172_Set_COMMSREG_Value(ADC_CMD_WRITE,registerAddress);	
	SPI2_Write(writeCommand,bytesNumber);
	DelayUs(1);
	//产生停止信号
	ADC_CS_N_HIGH;
}

/*********************************************************************************************/
/**
 * @brief 从一个寄存器读取数据，这个寄存器可以为8，16，24bits
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 *
 * @return dataRead - Value of the register.
*/
unsigned long AD7172_GetRegisterValue(unsigned char registerAddress,
                                       unsigned char bytesNumber)
{
	unsigned long dataRead        = 0x00;
	
	//产生开始信号
	ADC_CS_N_LOW;
	AD7172_Set_COMMSREG_Value(ADC_CMD_READ,registerAddress);
	dataRead=SPI2_Read(bytesNumber);
	DelayUs(1);
	//产生停止信号
	ADC_CS_N_HIGH;
	
	return dataRead;
}
/*********************************************************************************************/
/**
 * @brief 从一个寄存器读取数据，这个寄存器可以为8，16，24bits
 *
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 *
 * @return dataRead - Value of the register.
*/
unsigned long AD7172_ReadID(void)
{
	unsigned long ID=0;
	ID=AD7172_GetRegisterValue(ADC_ID_REG_ADDR,2);
	DelayUs(1);
	
	return ID;
}
/*********************************************************************************************/
/**
 * @brief 配置通道寄存器0/1/2/3,16bits
 *
 * @param CHregisterAddress - Address of the Channal register.可以为CH0/CH1/CH2/CH3的地址
          setupsel - 哪一种设置模式
 *        ainpos - AIN+是什么
 *        ainneg - AIN-是什么
 * @return NONE
*/
void AD7172_SetAdcChannal(unsigned char CHregisterAddress,unsigned long setupsel,unsigned long ainpos,unsigned long ainneg)
{
	unsigned long spiword=0;
	unsigned char bytesNr=2;  //寄存器字节数
	
	spiword=CH_EN|CH_SETUP_SEL(setupsel)|CH_AINPOS(ainpos)|CH_AINNEG(ainneg);  //要写入CH0寄存器内的值
	
	AD7172_SetRegisterValue(CHregisterAddress,spiword,bytesNr);  //写入寄存器
	
}

/*********************************************************************************************/
/**
 * @brief 配置设置寄存器0/1/2/3，16bits
 *
 * @param SETUPCONregaddr - 配置寄存器地址
          setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_SETUPCONReg(unsigned char SETUPCONregaddr,unsigned long setword)
{
	unsigned long spiword=0;
	unsigned char bytesNr=2;
	
	spiword=setword;
	AD7172_SetRegisterValue(SETUPCONregaddr,spiword,bytesNr);
}
/*********************************************************************************************/
/**
 * @brief 滤波器配置寄存器0/1/2/3，16bits
 * @param FiltConregaddr - 滤波器配置寄存器的地址
          setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_FILTCONReg(unsigned long FiltConregaddr,unsigned long setword)
{
	unsigned long spiword=0;
	unsigned char bytesNr=2;
	
	spiword=setword;
	AD7172_SetRegisterValue(FiltConregaddr,spiword,bytesNr);
}

/*********************************************************************************************/
/**
 * @brief 失调寄存器0/1/2/3,24bits
 * @param GAINregaddr - 寄存器地址
          setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_OFFSETReg(unsigned long OFFSETregaddr,unsigned long setword)
{
	unsigned long spiword=0;
	unsigned char bytesNr=3;
	
	spiword=setword;
	AD7172_SetRegisterValue(OFFSETregaddr,spiword,bytesNr);
}

/*********************************************************************************************/
/**
 * @brief 增益寄存器0/1/2/3,24bits
 * @param GAINregaddr - 寄存器地址
          setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_GAINReg(unsigned long GAINregaddr,unsigned long setword)
{
	unsigned long spiword=0;
	unsigned char bytesNr=3;
	
	spiword=setword;
	AD7172_SetRegisterValue(GAINregaddr,spiword,bytesNr);
}
/*********************************************************************************************/
/**
 * @brief ADC模式寄存器,16bits
 * @param GAINregaddr - 寄存器地址
          setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_ADCMODEReg(unsigned long setword)
{
	AD7172_SetRegisterValue(ADC_ADCMODE_REG_ADDR,setword,2);
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief 接口模式寄存器设置,16bits,
          接口模式寄存器中的几个控制位会影响ADC数据寄存器的位数
 * @param setword:寄存器的设置值，可以是几个控制字或的值
 * @return NONE
*/
void AD7172_IFMODEReg(unsigned long setword)
{
	AD7172_SetRegisterValue(ADC_IFMODE_REG_ADDR,setword,2);
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief 连续转换模式
          从ADC数据寄存器中读取数据，DATA寄存器默认为24bits，但是接口模式寄存器的几个位会影响其字节数
          当IFMODE_DATA_STAT=1,DATA的长度为32bits，当IFMODE_WL16=时，DATA，DATA寄存器的长度被限制为16bits
          先读取接口寄存器内的这两位，从而判断数据寄存器的长度，其转换公式是什么
 * @param 
 * @return dataread
*/
unsigned long AD7172_ReadFromDATAReg_1(void)
{
	unsigned long ifmodevalue=0;
	unsigned char bytesNr=3;   //原本数据寄存器的长度为3个字节
	unsigned long dataread=0;
	
	ifmodevalue=AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2);  //读取接口模式寄存器的值
	
	if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==0))  //DATA_STAT=0,WL16=0
		bytesNr=3;
	else if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==1))  //DATA_STAT=0,WL16=1
		bytesNr=2;
	else if((((ifmodevalue>>6)&1)==1)&&((ifmodevalue&1)==0))  //DATA_STAT=1,WL16=0
		bytesNr=4;
	else //DATA_STAT=1,WL16=1
		bytesNr=3;
	//开启连续转换模式
	AD7172_IFMODEReg(IFMODE_DOUT_RESET);
	//(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1); //开启转换之后要去判断RDY是否为0，为0表示转换完成
	//产生开始信号，将ADC DATA的值读出来
	ADC_CS_N_LOW;
	AD7172_Set_COMMSREG_Value(ADC_CMD_READ,ADC_DATA_REG_ADDR);   //发送0X44
	dataread=SPI2_Read(bytesNr);                                 //读取DATA寄存器的值
	//产生停止信号
	ADC_CS_N_HIGH;
	//开始转换ADC
	
	return dataread;
}

///*********************************************************************************************/
///**
// * @brief 连续读取模式
//          从ADC数据寄存器中读取数据，DATA寄存器默认为24bits，但是接口模式寄存器的几个位会影响其字节数
//          当IFMODE_DATA_STAT=1,DATA的长度为32bits，当IFMODE_WL16=时，DATA，DATA寄存器的长度被限制为16bits
//          先读取接口寄存器内的这两位，从而判断数据寄存器的长度，其转换公式是什么
// * @param 
// * @return dataread
//*/
//unsigned long AD7172_ReadFromDATAReg_2(void)
//{
//	unsigned long ifmodevalue=0; //控制DATA数据长度的一个控制字
//	unsigned char bytesNr=3;   //原本数据寄存器的长度为3个字节
//	unsigned long dataread=0;  //从DATA寄存器内读取的数据
//	
//	ifmodevalue=AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2);  //读取接口模式寄存器的值
//	
//	if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==0))  //DATA_STAT=0,WL16=0
//		bytesNr=3;
//	else if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==1))  //DATA_STAT=0,WL16=1
//		bytesNr=2;
//	else if((((ifmodevalue>>6)&1)==1)&&((ifmodevalue&1)==0))  //DATA_STAT=1,WL16=0
//		bytesNr=4;
//	else //DATA_STAT=1,WL16=1
//		bytesNr=3;
//	
//	//开启连续读取模式
//	//AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
//	AD7172_IFMODEReg(IFMODE_DOUT_RESET|IFMODE_CONTREAD);    //使RDY延迟输出，在CS变为高电平之后才输出RDY信号，且使能连续读取模式
//	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1); //开启转换之后要去判断RDY是否为0，为0表示转换完成
//	
//}

/*********************************************************************************************/
/**
 * @brief 在CS=0且SDO=1时提供64个SCLK，可以复位ADC和所有寄存器内容,ADC芯片复位函数
 * @param none
 * @return none
*/
void AD7172_Reset(void)
{
	unsigned char data[9]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	//产生开始信号
	ADC_CS_N_LOW;
	SPI2_Write(data,8);
	//产生停止信号
	ADC_CS_N_HIGH;
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief 校准6个增益通道的OFFSET寄存器内的值
 * @param none
 * @return none
*/
unsigned long AD7172_OFFSET_calibration(void)
{
	float volt;
	unsigned short DAC_data;
	int Offsetreg_value;
	//先进行失调校准，即让DAC输出0电平
  DAC_GPIO_CLR_N_HIGH;     //不使能清零代码寄存器的值有效，即让DAC寄存器内的值有效
	volt=0;
	if(volt<0)
			 DAC_data=volt*32768/10+0XFFFF;
	else
			 DAC_data=volt*32768/10;
	AD5760_SetDacValue(DAC_data);   //使DAC输出0电�
	//开启失调校准
	ADC_SYNC_N_HIGH;         //ADC开始转换
	//开启内部失调校准
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(4)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //校准完成后，状态REG的RDY变为低电平表示校准完成
	Delay(5);
	//转换完成之后开始校准
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(6)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //校准完成后，状态REG的RDY变为低电平表示校准完成
	Offsetreg_value=AD7172_GetRegisterValue(ADC_OFFSET0_REG_ADDR,3);      //读出经过校准后的OFFSETREG的值，并将这个值返回
	printf("OFFSETreg data = %X \r\n",Offsetreg_value);
	DAC_GPIO_CLR_N_LOW;     //使能清零代码寄存器的值
	ADC_SYNC_N_LOW;         //ADC关闭转换
	
	return Offsetreg_value;
	
}
/*********************************************************************************************/
/**
 * @brief 校准6个增益通道的GAIN寄存器内的值
 * @param none
 * @return none
*/
unsigned long AD7172_GAIN_calibration(void)
{
	float volt;
	unsigned short DAC_data;
	int Gainreg_value;

	//进行满量程校准
  DAC_GPIO_CLR_N_HIGH;     //不使能清零代码寄存器的值有效，即让DAC寄存器内的值有效
	volt=3;                  //DAC输出3V，ADC的输入端为2V，将2V作为满量程2.5V的输入，放大了1.25倍，故之后的值均要除以1.25
	if(volt<0)
			 DAC_data=volt*32768/10+0XFFFF;
	else
			 DAC_data=volt*32768/10;
	AD5760_SetDacValue(DAC_data);   //使DAC输出满量程电平
	//开启满量程校准
	ADC_SYNC_N_HIGH;         //ADC开始转换
	Delay(5);
	//转换完成之后开始系统失调校准
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(7)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //校准完成后，状态REG的RDY变为低电平表示校准完成
	Gainreg_value=AD7172_GetRegisterValue(ADC_GAIN0_REG_ADDR,3);      //读出经过校准后的OFFSETREG的值，并将这个值返回
	printf("GAINreg data = %X \r\n",Gainreg_value);
	DAC_GPIO_CLR_N_LOW;     //使能清零代码寄存器的值
	ADC_SYNC_N_LOW;         //ADC关闭转换
	
	return Gainreg_value;
	
}

