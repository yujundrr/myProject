/************************************
 * �ļ���  ��dac.c
 * ����    ��ʹ��оƬAD5760������ģת��������λ��������ֵת��Ϊģ���ѹ�����Ŵ������Ŵ�������Ϊ1
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ�SPI_GPIO_SYNC_N      -->PA4
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
  * @brief  DAC��ʼ��������Ҫ����LDAC��RST��CLR��������
  * @retval : none
  */
void AD5760_Init(void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		/* ʹ��GPIOA��GPIOB��ʱ��*/
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
		
		/*DAC��DAC_CLR_N,PB10*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_CLR_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*DAC��DAC_LDAC_N,PB1*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_LDAC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOB, &GPIO_InitStructure); 
	
	/*DAC��DAC_RST_N,PA2*/
	GPIO_InitStructure.GPIO_Pin = DAC_GPIO_RST_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(GPIOA, &GPIO_InitStructure); 
	
	//�����������ø�
	DAC_GPIO_LDAC_N_HIGH;
	DAC_GPIO_CLR_N_HIGH;
	DAC_GPIO_RST_N_HIGH;
	Delay(500);
	DAC_SPI1_Init();
	
	}
	
/*****************************************************************************************/
	/**
  * @brief  ʹ�����
  * @param  state��Enables/disables the output.
 *                 Example: 1 - Enables output.
 *                          0 - Disables output.
  * @retval : none
  */
void AD5760_EnableOutput(unsigned char state)
{
	
	unsigned long oldControl = 0;  //��ǰ��״̬
  unsigned long newControl = 0;  //�µ�״̬
	
	oldControl = AD5760_GetRegisterValue(DAC_REG_ADDR_CTRL, 3);           //��ȡ��ǰ�Ŀ���״̬
	oldControl = oldControl & ~(DAC_CTRL_OPGND); // Clears OPGND bit.     //
  newControl = oldControl | DAC_CTRL_OPGND * (!state);
  AD5760_SetRegisterValue(DAC_REG_ADDR_CTRL, newControl, 3);
}	
	
/*****************************************************************************************/
	/**
  * @brief  ��һ���Ĵ���д����
  * @param  aregisterAddress:�Ĵ�����ַ
	* @param  registerValue���Ĵ�����ֵ
	  @param  bytesNumber��Ҫд������ݵ��ֽ���
  * @retval : none
  */
void AD5760_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber)
{
	  unsigned char writeCommand[4] = {0, 0, 0, 0};
    unsigned long spiWord         = 0;
		/*�������ǲ��õ���MSB���У����ȷ���˵��ֽ�*/
    unsigned char* dataPointer    = (unsigned char*)&spiWord;  //�ֱ�ָ��4���ֽڵĵ�ַ
    unsigned char bytesNr         = bytesNumber;               //�ֽ���
	 
		/*�ϳ�4���ֽڵ�һλ��*/
		spiWord=DAC_CMD_WRITE |
			      DAC_ADDR_REG(registerAddress) |
			      registerValue;
		/*��Ϊ�ҵĴ�������Intel��ΪС�˴洢���������ֽڴ洢��С��ַ����
		�����õ�SPI����ģʽΪ������У���Ҫ����ת���������С��˳�����
		֮��ֻҪ��������鴫��ȥ���ɽ��з���*/
		writeCommand[0] = 0x01;  //���ֵ��Ӱ�죬��Ϊֻ���������ֽڣ�ÿ���Ĵ����Ĵ�СΪ24Bits
    while(bytesNr != 0)
    {
        writeCommand[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    }   
		//����ȥ�������ֽ���������
	  SPI_Write(writeCommand, bytesNumber);
		
}

/***********************************************************************************************/
	/**
  * @brief  ��DAC�Ĵ���д���ݲ�ʹ���첽DAC���
  * @param  value:����DAC�Ĵ�����ֵ
  * @retval : none
  */
void AD5760_SetDacValue(unsigned long value)   //dac�õ��Ƕ����Ʋ������,
{

	AD5760_SetRegisterValue(DAC_REG_ADDR_DAC,value<<4,3);  //��ʱSYNCΪ1��
	
	DelayUs(5);
	
	DAC_GPIO_LDAC_N_LOW;   //�첽����
	
	DelayUs(5);
	
	DAC_GPIO_LDAC_N_HIGH;
	
}
/***********************************************************************************************/
	/**
  * @brief  �����ƼĴ���дֵ
  * @param  setupWord��24λֵ�����û�������ƼĴ���
            Example: AD5780_CTRL_BIN2SC | AD5780_CTRL_SDODIS - sets
 *                   the DAC register to use offset binary coding and 
 *                   disables SDO pin(tristated).   
  * @retval : none
  */
void AD5760_Setup_CTRLREG(unsigned long setupWord)   //���ƼĴ������Ƕ����Ʋ���
{
	AD5760_SetRegisterValue(DAC_REG_ADDR_CTRL,setupWord,3);
	//�ȴ�������Ч
	Delay(10);
}

/***********************************************************************************************/
	/**
  * @brief  ������Ĵ�������
  * @param  clrCode��24λֵ���Ĵ����ڵ�ֵ
            Example: None   
  * @retval : none
  */
void AD5760_Setup_CLRCODE(unsigned long clrCode)    //
{
	AD5760_SetRegisterValue(DAC_REG_ADDR_CLRCODE,clrCode,3);
	//�ȴ�������Ч
	Delay(10);
}

/************************************************************************************************/
	/**
  * @brief  ��������ƼĴ���дֵ,����һ��ֻ�ܸı�һλ
  * @param  instructionBit��������ƼĴ�����һ������λ
  * @retval : none
  */
void AD5760_SoftInstruction(unsigned char instructionBit)
{
	
	//��ֵд��������ƼĴ���
	AD5760_SetRegisterValue(DAC_CMD_WR_SOFT_CTRL,instructionBit,3);
	//�ȴ�������Ч
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
		//����ǿ��������ת��������ֻ�ѵͰ�λ����registerWord[1]
    registerWord[1] = (DAC_CMD_READ | DAC_ADDR_REG(registerAddress)) >> 16;
		
    dataRead=SPI_Read(registerWord, 3);  
   
    
    return(dataRead);
}

/****************************************���뼶����ʱ****************************************************/
// �����ʱ������1ms
void Delay(unsigned long time)
{
	unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12000;i++);
	}
}
				

/****************************************΢�뼶����ʱ****************************************************/
// �����ʱ����,1us
void DelayUs(unsigned long time)
{
	unsigned long i,j;
  
	for(j=0; j<time; j++)
	{
	   for(i=0;i<12;i++);
	}
}
														 

