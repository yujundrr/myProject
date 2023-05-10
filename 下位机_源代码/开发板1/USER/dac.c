/************************************
 * 文件名  ：dac.c
 * 描述    ：使用芯片AD5760进行数模转换，将上位机传来的值转换为模拟电压传给放大器，放大器增益为1
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 硬件连接：SPI_GPIO_SYNC_N      -->PA4
             SPI_GPIO_CLK         -->PA5
             SPI_GPIO_SDI         -->PA6
             SPI_GPIO_SDO         -->PA7
						 DAC_GPIO_CLR_N       -->PB10
             DAC_GPIO_LDAC_N      -->PB1
             DAC_GPIO_RST_N       -->PA2
**********************************************************************************/

#include "dac.h"
#include "spi.h"

/**
  * @brief  DAC初始化函，主要配置LDAC、RST、CLR三个引脚
  * @retval : none
  */
void AD5760_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* 使能GPIOA和GPIOB的时钟*/
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
		
		/*DAC的DAC_CLR_N,PB10*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_CLR_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*DAC的DAC_LDAC_N,PB1*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_LDAC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*DAC的DAC_RST_N,PA2*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_RST_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //普通推挽输出
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//输入端口不用设置输出频率
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//将三个引脚置高
	DAC_GPIO_LDAC_N_HIGH;
	DAC_GPIO_CLR_N_HIGH;
	DAC_GPIO_RST_N_HIGH;
	Delay(500);
	DAC_SPI1_Init();
	
	}
	
/*****************************************************************************************/
	/**
  * @brief  使能输出
  * @param  state：Enables/disables the output.
 *                 Example: 1 - Enables output.
 *                          0 - Disables output.
  * @retval : none
  */
void AD5760_EnableOutput(unsigned char state)
{
	
	unsigned long oldControl = 0;  //以前的状态
  unsigned long newControl = 0;  //新的状态
	
	oldControl = AD5760_GetRegisterValue(DAC_REG_ADDR_CTRL, 3);           //读取以前的控制状态
	oldControl = oldControl & ~(DAC_CTRL_OPGND); // Clears OPGND bit.     //
  newControl = oldControl | DAC_CTRL_OPGND * (!state);
  AD5760_SetRegisterValue(DAC_REG_ADDR_CTRL, newControl, 3);
}	
	
/*****************************************************************************************/
	/**
  * @brief  向一个寄存器写数据
  * @param  aregisterAddress:寄存器地址
	* @param  registerValue：寄存器的值
	  @param  bytesNumber：要写入的数据的字节数
  * @retval : none
  */
void AD5760_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber)
{
	  unsigned char writeCommand[4] = {0, 0, 0, 0};
    unsigned long spiWord         = 0;
		/*由于我们采用的是MSB先行，即先发大端的字节*/
    unsigned char* dataPointer    = (unsigned char*)&spiWord;  //分别指向4个字节的地址
    unsigned char bytesNr         = bytesNumber;               //字节数
	 
		/*合成4个字节的一位数*/
		spiWord=DAC_CMD_WRITE |
			      DAC_ADDR_REG(registerAddress) |
			      registerValue;
		/*因为我的处理器是Intel，为小端存储，即将低字节存储在小地址里面
		而设置的SPI工作模式为大端先行，故要进行转换，将大端小端顺序调换
		之后只要将这个数组传过去即可进行发送*/
		writeCommand[0] = 0x01;  //这个值不影响，因为只发后三个字节，每个寄存器的大小为24Bits
    while(bytesNr != 0)
    {
        writeCommand[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    }   
		//发过去，按照字节数来发。
	  SPI_Write(writeCommand, bytesNumber);
		
}

/***********************************************************************************************/
	/**
  * @brief  向DAC寄存器写数据并使能异步DAC输出
  * @param  value:赋给DAC寄存器的值
  * @retval : none
  */
void AD5760_SetDacValue(unsigned long value)   //dac用的是二进制补码编码,
{

	AD5760_SetRegisterValue(DAC_REG_ADDR_DAC,value<<4,3);  //这时SYNC为1，
	
	DelayUs(5);
	
	DAC_GPIO_LDAC_N_LOW;   //异步更新
	
	DelayUs(5);
	
	DAC_GPIO_LDAC_N_HIGH;
	
}
/***********************************************************************************************/
	/**
  * @brief  给控制寄存器写值
  * @param  setupWord：24位值，设置或清除控制寄存器
            Example: AD5780_CTRL_BIN2SC | AD5780_CTRL_SDODIS - sets
 *                   the DAC register to use offset binary coding and 
 *                   disables SDO pin(tristated).   
  * @retval : none
  */
void AD5760_Setup_CTRLREG(unsigned long setupWord)   //控制寄存器不是二进制补码
{
	AD5760_SetRegisterValue(DAC_REG_ADDR_CTRL,setupWord,3);
	//等待设置生效
	Delay(10);
}

/***********************************************************************************************/
	/**
  * @brief  给清零寄存器设置
  * @param  clrCode：24位值，寄存器内的值
            Example: None   
  * @retval : none
  */
void AD5760_Setup_CLRCODE(unsigned long clrCode)    //
{
	AD5760_SetRegisterValue(DAC_REG_ADDR_CLRCODE,clrCode,3);
	//等待设置生效
	Delay(10);
}

/************************************************************************************************/
	/**
  * @brief  给软件控制寄存器写值,调用一次只能改变一位
  * @param  instructionBit：软件控制寄存器的一个控制位
  * @retval : none
  */
void AD5760_SoftInstruction(unsigned char instructionBit)
{
	
	//将值写入软件控制寄存器
	AD5760_SetRegisterValue(DAC_CMD_WR_SOFT_CTRL,instructionBit,3);
	//等待设置生效
	Delay(10);
}


/*********************************************************************************************/
/**
 * @brief Reads the value of a register.
 *
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 *
 * @return dataRead - Value of the register.
*/
unsigned long AD5760_GetRegisterValue(unsigned char registerAddress,
                                       unsigned char bytesNumber)
{
	  unsigned char registerWord[4] = {0, 0, 0, 0}; 
    unsigned long dataRead        = 0x0;
		
		registerWord[0] = 0x01;
		//由于强制性类型转换，所以只把低八位传给registerWord[1]
    registerWord[1] = (DAC_CMD_READ | DAC_ADDR_REG(registerAddress)) >> 16;
		
    dataRead=SPI_Read(registerWord, 3);  
   
    
    return(dataRead);
}

/****************************************毫秒级别延时****************************************************/
// 软件延时函数，1ms
void Delay(unsigned long time)
{
	unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}
				

/****************************************微秒级别延时****************************************************/
// 软件延时函数,1us
void DelayUs(unsigned long time)
{
	unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12;i++);
	}
}
														 

