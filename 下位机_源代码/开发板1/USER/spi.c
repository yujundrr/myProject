/************************************
 * 文件名  ：spi.c
 * 描述    ：ad5760的通讯配置底层函数       
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 硬件连接: SPI_GPIO_SYNC_N      -->PA4
             SPI_GPIO_CLK         -->PA5
             SPI_GPIO_SDI         -->PA6
             SPI_GPIO_SDO         -->PA7
						 DAC_GPIO_CLR_N       -->PB10
             DAC_GPIO_LDAC_N      -->PB1
             DAC_GPIO_RST_N       -->PA2

**********************************************************************************/
#include "spi.h"
#include "dac.h"

/*配置SPI端口，这四个为SPI的引脚*/
 static void DAC_SPI1_GPIO_Config(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* 使能GPIOA和GPIOB的时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	 
	 /*SPI引脚配置,PA4*/
	 /*单片机的SPI NSS引脚定义，DAC的SYNC引脚*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SYNC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通推挽输出，因为NSS使用的是软件控制
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	 
	 /*SPI SCLK,PA5*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_CLK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	 
	 /*单片机的SPI SDI，DAC的SDO,PA6*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SDI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	
	/*单片机的SPI SDO，DAC的SDI,PA7*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SDO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //复用推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 	
	
	//停止信号，刚开始的时候处于空闲状态
	DAC_SPI1_GPIO_SYNC_N_HIGH;
	
 }
 
 
/*配置SPI的工作模式  */
static void DAC_SPI1_Mode_Config(void)
 {
	 
	 //定义一个SPI的初始化结构体
	 SPI_InitTypeDef  SPI_InitStructure;
	 
	  //打开SPI1的时钟
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	 
	 //设置结构体的具体设置
	 SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;  //16分配,sclk的频率为72/16=4.5MHz，因为DAC的最高为35MHz
	 SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //主模式
	 SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //CPHA=1
	 SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;                           //CPOL=0 
	 SPI_InitStructure.SPI_CRCPolynomial=7;                             //由于没有使能SPI的CRC功能，所以这里的值无效          
	 SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //数据帧大小为8位
	 SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //全双工同时接收和发送
	 SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //MSB先行
	 SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //开始信号和停止信号由软件产生
	 
	 //初始化
	 SPI_Init(SPI1,&SPI_InitStructure);
	 
	 //使能SPI
	 SPI_Cmd(SPI1,ENABLE);
	 
 }
 
 void DAC_SPI1_Init(void)
 {
	 //只要调用这个函数就可以
	 DAC_SPI1_GPIO_Config();
	 DAC_SPI1_Mode_Config();
 }
 
 /******************************************************************/
 /**
  * @brief  SPI等待事件超时的情况下会调用这个函数来处理，
            与下面发送一个字节的函数一起作用，可以得知在发送一个字节时是否错误，且根据errorCode能够得知
  * @param  errorCode:错误代码。可以用来定位是哪个环节出错
  * @retval 返回0，表示SPI读取失败
  */
static u8 SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	//使用串口printf输出错误信息，方便调试
	DAC_SPI_ERROR("SPI等待超时！errorCode=%d",errorCode);
	return 0;
}
 /**
  * @brief  使用SPI发送数据
  * @param  data:要发送的数据
  * @param  bytesNumber:要发送的数据字节大小
  * @retval : 返回错误信息
  */
u8 SPI_Write(unsigned char *data,unsigned char bytesNumber)
{
	//定义通讯超时变量
	int i=0;
	unsigned char recieveData[4]={0};    //收到的数据
	uint32_t SPITimeout;
	
	//产生开始信号
	DAC_SPI1_GPIO_SYNC_N_LOW;
	
	//发送数据
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPIT_FLAG_TIMEOUT;
		//当SPI_I2S_FLAG_TXE=1时表示发送缓冲器为空，可以写入数据
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/*发送数据*/
	SPI_I2S_SendData(SPI1,data[i]);

	
	//接收数据
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_RXNE=1时代表接收缓冲器不为空，即是有数据传来
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(1);
	}
	/*接收数据*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI1);
  }
	
	Delay(1);
	//产生停止信号，停止发送
  DAC_SPI1_GPIO_SYNC_N_HIGH;
	
	return 1;
}
 /**
  * @brief  从SPI读取数据
  * @param  data数组:这个数组的第一个数应该定义是读还是写
  * @param  bytesNumber:要接收的数据字节大小
  * @retval : 返回读取的数据
  */
/*! Reads data from SPI. */
unsigned long SPI_Read(unsigned char *data,
                        unsigned char bytesNumber)
{
	int i=0;
	uint32_t SPITimeout;      //超时变量
	unsigned char recieveData[4]={0};    //收到的数据
	unsigned long dataRead=0;           //将其转换为一个32位的数据
	unsigned char readback[4]={0,8,0,0};      //NOP
	
	SPI_Write(data,bytesNumber);
	
	Delay(1);
	
	//产生开始信号
	DAC_SPI1_GPIO_SYNC_N_LOW;
	//同时发生同时接收
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPIT_FLAG_TIMEOUT;
		//当SPI_I2S_FLAG_TXE=1时表示发送缓冲器为空，可以写入数据
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/*发送数据*/
	SPI_I2S_SendData(SPI1,readback[i]);

		
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//当SPI_I2S_FLAG_RXNE=1时代表接收缓冲器不为空，即是有数据传来
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(1);
	}
	/*接收数据*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI1);
	
	}
		
	Delay(1);
  //产生停止信号，停止发送
  DAC_SPI1_GPIO_SYNC_N_HIGH;
	
	for(i = 1;i <= bytesNumber;i ++) 
    {
        dataRead = (dataRead<<8) + recieveData[i];   //registerWord[0]是存储的高字节
    }
	/*成功发送返回1*/
	return dataRead;
	
}


												 
 

