/************************************
 * �ļ���  ��adc.c
 * ����    ������AD7172-2оƬ,�õ�SPI2����ͨ��    
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * Ӳ�����ӣ� 
**********************************************************************************/
#include "adc.h"
#include "dac.h"



/**
  * @brief  ADC��SPI2���ĸ�������������,SYNC����Ӧ����ô����,��ʱ�Ȱ�SYNC����Ϊ��ͨ�������
  * @retval : none
  */
static void ADC_SPI2_GPIO_Config(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;    //GPIO_InitTypeDef��һ���ṹ������GPIO_Pin��GPIO_Mode��GPIO_Speed����ö����
	/* ʹ��GPIOA��GPIOB��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB, ENABLE); 
	
	//CS,PB12
	GPIO_InitStructure.GPIO_Pin=ADC_CS_N;          
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP; //��ͨ�����������ΪNSSʹ�õ����������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(ADC_SPI2_PORT,&GPIO_InitStructure);
	
	/*SPI2 SCLK,PB13*/
	GPIO_InitStructure.GPIO_Pin = ADC_SCLK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure); 
	
	/*��Ƭ����SPI2 SDI��ADC��SDO,PB14*/
	GPIO_InitStructure.GPIO_Pin = ADC_SDI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure); 
	
	/*��Ƭ����SPI2 SDO��DAC��SDI,PB15*/
	GPIO_InitStructure.GPIO_Pin = ADC_SDO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(ADC_SPI2_PORT, &GPIO_InitStructure);
	
	/*ADC��SYNC���ţ�PA12����������൱��������ADCת����ʹ������*/
	GPIO_InitStructure.GPIO_Pin = ADC_SYNC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(ADC_SYNC_ERROR_PORT, &GPIO_InitStructure);
	
	//�տ�ʼ���ڿ���״̬
	ADC_CS_N_HIGH;
	//SYNC����Ϊͬ���������ţ�SYNC��������ʹ��ADC��ת��
	ADC_SYNC_N_LOW;
	
}

/*����SPI2�Ĺ���ģʽ  */
static void ADC_SPI2_Mode_Config(void)
{
	//����һ��SPI�ĳ�ʼ���ṹ��
	 SPI_InitTypeDef  SPI_InitStructure;
	
	//��SPI2��ʱ��,SPI2��APB1ʱ������,APB1��ʱ��Ϊ36M
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,ENABLE);
	/*AD7172-2*/
	SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_8;   //SCLKʱ��Ƶ��Ϊ36/8=4.5M
	SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //ż�����ز���
	SPI_InitStructure.SPI_CPOL=SPI_CPOL_High;                          //CSΪ��ʱ��SCLKΪ�ߵ�ƽ
	SPI_InitStructure.SPI_CRCPolynomial=7;                             //CRC��Ч
	SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //8λ����
	SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //ȫ˫��
	SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //MSB����
	SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //��ģʽ
	SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //��ʼ�źź�ֹͣ�ź����������
	//��ʼ��
	SPI_Init(SPI2,&SPI_InitStructure);
	//ʹ��SPI
	SPI_Cmd(SPI2,ENABLE);
	
}

 void ADC_SPI2_Init(void)
 {
	 //ֻҪ������������Ϳ���
	 ADC_SPI2_GPIO_Config();
	 ADC_SPI2_Mode_Config();
 }
 
 
 /******************************************************************/
 /**
  * @brief  SPI2�ȴ��¼���ʱ������»�����������������
            �����淢��һ���ֽڵĺ���һ�����ã����Ե�֪�ڷ���һ���ֽ�ʱ�Ƿ�����Ҹ���errorCode�ܹ���֪
  * @param  errorCode:������롣����������λ���ĸ����ڳ���
  * @retval ����0����ʾSPI��ȡʧ��
  */
 static u8 SPI2_TIMEOUT_UserCallback(uint8_t errorCode)
{
	//ʹ�ô���printf���������Ϣ���������
	ADC_SPI2_ERROR("SPI�ȴ���ʱ��errorCode=%d",errorCode);
	return 0;
}

/************************************************************************
  * @brief  ADC��ͨ�żĴ�����ADCȫ���Ĵ���ӳ��ķ��ʣ�ͨ�żĴ�����ֻд�Ĵ���,��д����
  * ͨ�żĴ����İ�λ��7��WEN_N(Ҫ��ADC��ʼͨ�ţ���λ����Ϊ�͵�ƽ);6��R/W;5-0:Address
  * @param  RWCtrldata:д��ͨ�żĴ�����8λ����
  * @retval : ���ش�����Ϣ
  */
u8 SPI2_ReadWrite_CTRL(unsigned char RWCtrldata)
{

	unsigned char recieveData;    //�յ���8bits����
	uint32_t SPITimeout;          //��ʱ����         
	
	//��ʼͬʱ���պ�ͬʱ����
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_TXE=1ʱ��ʾ���ͻ�����Ϊ�գ�����д������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(2);
	}
	/*��������*/
	SPI_I2S_SendData(SPI2,RWCtrldata);

	//��������
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_RXNE=1ʱ������ջ�������Ϊ�գ����������ݴ���
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(3);
	}
	/*��������*/
	recieveData=SPI_I2S_ReceiveData(SPI2);

	return 1;
}

 /*********************************************************
  * @brief  ʹ��SPI2�������ݣ�SPIд����
  * @param  data:Ҫ���͵����ݣ���װ�õ��������飬���鳤��Ϊ4
  * @param  bytesNumber:Ҫ���͵������ֽڴ�С,��ADC�ڿ�����1��2��3
  * @retval : ���ش�����Ϣ
  */
u8 SPI2_Write(unsigned char *data,unsigned char bytesNumber)
{
	int i=0;
	unsigned char recieveData[9]={0};    //�յ�������
	uint32_t SPITimeout;                 //��ʱ����

	//����Ҫд����ֽڸ�������д��
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPI2T_FLAG_TIMEOUT;
		//��SPI_I2S_FLAG_TXE=1ʱ��ʾ���ͻ�����Ϊ�գ�����д������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(4);
	}
	/*��������*/
	SPI_I2S_SendData(SPI2,data[i]);

	//��������
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_RXNE=1ʱ������ջ�������Ϊ�գ����������ݴ���
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(5);
	}
	/*��������*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI2);
  }
	
//	DelayUs(1);
//	//����ֹͣ�ź�
//	ADC_CS_N_HIGH;
	return 1;
}

 /*********************************************************
  * @brief  ��SPI2��ȡ����,���ݳ��ȿ���Ϊ1��2��3
  * @param  bytesNumber:Ҫ���յ������ֽڴ�С
  * @retval : ���ض�ȡ������
  */
/*! Reads data from SPI. */
unsigned long SPI2_Read(unsigned char bytesNumber)
{
	int i=0;
	uint32_t SPITimeout;      //��ʱ����
	unsigned char recieveData[4]={0};    //�յ�������
	unsigned long dataRead=0;           //����ת��Ϊһ��32λ������
	unsigned char readback[4]={0,0,0,0};      //NOP
	

	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPI2T_FLAG_TIMEOUT;
		//��SPI_I2S_FLAG_TXE=1ʱ��ʾ���ͻ�����Ϊ�գ�����д������
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(6);
	}
	/*��������*/
	SPI_I2S_SendData(SPI2,readback[i]);

	
	//��������
	SPITimeout=SPI2T_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_RXNE=1ʱ������ջ�������Ϊ�գ����������ݴ���
	while(SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI2_TIMEOUT_UserCallback(7);
	}
	/*��������*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI2);
  }
	
	for(i = 1;i <= bytesNumber;i ++) 
    {
        dataRead = (dataRead<<8) + recieveData[i];   //registerWord[0]�Ǵ洢�ĸ��ֽ�
    }
	/*�ɹ����ͷ���1*/
	return dataRead;
}

/*****************************************************************************************/
	/**
  * @brief  ��ͨ�żĴ���д8λָ�ͨ�żĴ����ĵ�ַΪ0X00����ʾ����һ���Ĵ���������д
  * @param  RW:��дλ;1:����0:д
  * @param  registerAddress:Ҫ���ĸ��Ĵ������в���
  * @retval : none
  */
void AD7172_Set_COMMSREG_Value(unsigned char RW,unsigned char registerAddress)
{
	unsigned char comdata=0;   //Ҫд��COMMS�Ĵ�����ֵ
	
	comdata=ADC_CMD_WEN_N_LOW|RW|registerAddress;
	
	//д��
	SPI2_ReadWrite_CTRL(comdata);	
}

/*****************************************************************************************/
	/**
  * @brief  ��ĳһ���Ĵ�������д����
  * @param  registerValue:��Ҫ�ڸ����Ĵ������ú����е�����������������ֵ��һ�����ú��˵�
  * @param  registerAddress:Ҫ���ĸ��Ĵ������в���
  * @param  bytesNumber��Ҫд����ֽ������ɾ���Ĵ�������
  * @retval : none
  */
void AD7172_SetRegisterValue(unsigned char registerAddress,
                             unsigned long registerValue,
                             unsigned char bytesNumber)
{
	unsigned char writeCommand[4] = {0, 0, 0, 0};      //Ҫд�������
	unsigned long spiWord         = 0;
	unsigned char* dataPointer    = (unsigned char*)&spiWord;  //�ֱ�ָ��4���ֽڵĵ�ַ
	unsigned char bytesNr         = bytesNumber;               //���滹Ҫ�õ������Զ�洢
	
	spiWord=registerValue;    //���ݿ����ֽ���ƴ��������
	
	
	writeCommand[0] = 0x01;  //���ֵ��Ӱ�죬��Ϊֻ���������ֽڣ�ÿ���Ĵ����Ĵ�С���Ϊ24Bits
	while(bytesNr != 0)
    {
        writeCommand[bytesNr] = *dataPointer;
        dataPointer ++;
        bytesNr --;
    } 
		//������ʼ�ź�
	ADC_CS_N_LOW;
	//�ȷ���д����
	AD7172_Set_COMMSREG_Value(ADC_CMD_WRITE,registerAddress);	
	SPI2_Write(writeCommand,bytesNumber);
	DelayUs(1);
	//����ֹͣ�ź�
	ADC_CS_N_HIGH;
}

/*********************************************************************************************/
/**
 * @brief ��һ���Ĵ�����ȡ���ݣ�����Ĵ�������Ϊ8��16��24bits
 * @param registerAddress - Address of the register.
 * @param bytesNumber - Number of bytes that will be read.
 *
 * @return dataRead - Value of the register.
*/
unsigned long AD7172_GetRegisterValue(unsigned char registerAddress,
                                       unsigned char bytesNumber)
{
	unsigned long dataRead        = 0x00;
	
	//������ʼ�ź�
	ADC_CS_N_LOW;
	AD7172_Set_COMMSREG_Value(ADC_CMD_READ,registerAddress);
	dataRead=SPI2_Read(bytesNumber);
	DelayUs(1);
	//����ֹͣ�ź�
	ADC_CS_N_HIGH;
	
	return dataRead;
}
/*********************************************************************************************/
/**
 * @brief ��һ���Ĵ�����ȡ���ݣ�����Ĵ�������Ϊ8��16��24bits
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
 * @brief ����ͨ���Ĵ���0/1/2/3,16bits
 *
 * @param CHregisterAddress - Address of the Channal register.����ΪCH0/CH1/CH2/CH3�ĵ�ַ
          setupsel - ��һ������ģʽ
 *        ainpos - AIN+��ʲô
 *        ainneg - AIN-��ʲô
 * @return NONE
*/
void AD7172_SetAdcChannal(unsigned char CHregisterAddress,unsigned long setupsel,unsigned long ainpos,unsigned long ainneg)
{
	unsigned long spiword=0;
	unsigned char bytesNr=2;  //�Ĵ����ֽ���
	
	spiword=CH_EN|CH_SETUP_SEL(setupsel)|CH_AINPOS(ainpos)|CH_AINNEG(ainneg);  //Ҫд��CH0�Ĵ����ڵ�ֵ
	
	AD7172_SetRegisterValue(CHregisterAddress,spiword,bytesNr);  //д��Ĵ���
	
}

/*********************************************************************************************/
/**
 * @brief �������üĴ���0/1/2/3��16bits
 *
 * @param SETUPCONregaddr - ���üĴ�����ַ
          setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
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
 * @brief �˲������üĴ���0/1/2/3��16bits
 * @param FiltConregaddr - �˲������üĴ����ĵ�ַ
          setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
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
 * @brief ʧ���Ĵ���0/1/2/3,24bits
 * @param GAINregaddr - �Ĵ�����ַ
          setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
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
 * @brief ����Ĵ���0/1/2/3,24bits
 * @param GAINregaddr - �Ĵ�����ַ
          setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
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
 * @brief ADCģʽ�Ĵ���,16bits
 * @param GAINregaddr - �Ĵ�����ַ
          setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
 * @return NONE
*/
void AD7172_ADCMODEReg(unsigned long setword)
{
	AD7172_SetRegisterValue(ADC_ADCMODE_REG_ADDR,setword,2);
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief �ӿ�ģʽ�Ĵ�������,16bits,
          �ӿ�ģʽ�Ĵ����еļ�������λ��Ӱ��ADC���ݼĴ�����λ��
 * @param setword:�Ĵ���������ֵ�������Ǽ��������ֻ��ֵ
 * @return NONE
*/
void AD7172_IFMODEReg(unsigned long setword)
{
	AD7172_SetRegisterValue(ADC_IFMODE_REG_ADDR,setword,2);
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief ����ת��ģʽ
          ��ADC���ݼĴ����ж�ȡ���ݣ�DATA�Ĵ���Ĭ��Ϊ24bits�����ǽӿ�ģʽ�Ĵ����ļ���λ��Ӱ�����ֽ���
          ��IFMODE_DATA_STAT=1,DATA�ĳ���Ϊ32bits����IFMODE_WL16=ʱ��DATA��DATA�Ĵ����ĳ��ȱ�����Ϊ16bits
          �ȶ�ȡ�ӿڼĴ����ڵ�����λ���Ӷ��ж����ݼĴ����ĳ��ȣ���ת����ʽ��ʲô
 * @param 
 * @return dataread
*/
unsigned long AD7172_ReadFromDATAReg_1(void)
{
	unsigned long ifmodevalue=0;
	unsigned char bytesNr=3;   //ԭ�����ݼĴ����ĳ���Ϊ3���ֽ�
	unsigned long dataread=0;
	
	ifmodevalue=AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2);  //��ȡ�ӿ�ģʽ�Ĵ�����ֵ
	
	if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==0))  //DATA_STAT=0,WL16=0
		bytesNr=3;
	else if((((ifmodevalue>>6)&1)==0)&&((ifmodevalue&1)==1))  //DATA_STAT=0,WL16=1
		bytesNr=2;
	else if((((ifmodevalue>>6)&1)==1)&&((ifmodevalue&1)==0))  //DATA_STAT=1,WL16=0
		bytesNr=4;
	else //DATA_STAT=1,WL16=1
		bytesNr=3;
	//��������ת��ģʽ
	AD7172_IFMODEReg(IFMODE_DOUT_RESET);
	//(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1); //����ת��֮��Ҫȥ�ж�RDY�Ƿ�Ϊ0��Ϊ0��ʾת�����
	//������ʼ�źţ���ADC DATA��ֵ������
	ADC_CS_N_LOW;
	AD7172_Set_COMMSREG_Value(ADC_CMD_READ,ADC_DATA_REG_ADDR);   //����0X44
	dataread=SPI2_Read(bytesNr);                                 //��ȡDATA�Ĵ�����ֵ
	//����ֹͣ�ź�
	ADC_CS_N_HIGH;
	//��ʼת��ADC
	
	return dataread;
}

///*********************************************************************************************/
///**
// * @brief ������ȡģʽ
//          ��ADC���ݼĴ����ж�ȡ���ݣ�DATA�Ĵ���Ĭ��Ϊ24bits�����ǽӿ�ģʽ�Ĵ����ļ���λ��Ӱ�����ֽ���
//          ��IFMODE_DATA_STAT=1,DATA�ĳ���Ϊ32bits����IFMODE_WL16=ʱ��DATA��DATA�Ĵ����ĳ��ȱ�����Ϊ16bits
//          �ȶ�ȡ�ӿڼĴ����ڵ�����λ���Ӷ��ж����ݼĴ����ĳ��ȣ���ת����ʽ��ʲô
// * @param 
// * @return dataread
//*/
//unsigned long AD7172_ReadFromDATAReg_2(void)
//{
//	unsigned long ifmodevalue=0; //����DATA���ݳ��ȵ�һ��������
//	unsigned char bytesNr=3;   //ԭ�����ݼĴ����ĳ���Ϊ3���ֽ�
//	unsigned long dataread=0;  //��DATA�Ĵ����ڶ�ȡ������
//	
//	ifmodevalue=AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2);  //��ȡ�ӿ�ģʽ�Ĵ�����ֵ
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
//	//����������ȡģʽ
//	//AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
//	AD7172_IFMODEReg(IFMODE_DOUT_RESET|IFMODE_CONTREAD);    //ʹRDY�ӳ��������CS��Ϊ�ߵ�ƽ֮������RDY�źţ���ʹ��������ȡģʽ
//	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1); //����ת��֮��Ҫȥ�ж�RDY�Ƿ�Ϊ0��Ϊ0��ʾת�����
//	
//}

/*********************************************************************************************/
/**
 * @brief ��CS=0��SDO=1ʱ�ṩ64��SCLK�����Ը�λADC�����мĴ�������,ADCоƬ��λ����
 * @param none
 * @return none
*/
void AD7172_Reset(void)
{
	unsigned char data[9]={0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF,0XFF};
	//������ʼ�ź�
	ADC_CS_N_LOW;
	SPI2_Write(data,8);
	//����ֹͣ�ź�
	ADC_CS_N_HIGH;
	DelayUs(1);
}

/*********************************************************************************************/
/**
 * @brief У׼6������ͨ����OFFSET�Ĵ����ڵ�ֵ
 * @param none
 * @return none
*/
unsigned long AD7172_OFFSET_calibration(void)
{
	float volt;
	unsigned short DAC_data;
	int Offsetreg_value;
	//�Ƚ���ʧ��У׼������DAC���0��ƽ
  DAC_GPIO_CLR_N_HIGH;     //��ʹ���������Ĵ�����ֵ��Ч������DAC�Ĵ����ڵ�ֵ��Ч
	volt=0;
	if(volt<0)
			 DAC_data=volt*32768/10+0XFFFF;
	else
			 DAC_data=volt*32768/10;
	AD5760_SetDacValue(DAC_data);   //ʹDAC���0��1�7
	//����ʧ��У׼
	ADC_SYNC_N_HIGH;         //ADC��ʼת��
	//�����ڲ�ʧ��У׼
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(4)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //У׼��ɺ�״̬REG��RDY��Ϊ�͵�ƽ��ʾУ׼���
	Delay(5);
	//ת�����֮��ʼУ׼
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(6)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //У׼��ɺ�״̬REG��RDY��Ϊ�͵�ƽ��ʾУ׼���
	Offsetreg_value=AD7172_GetRegisterValue(ADC_OFFSET0_REG_ADDR,3);      //��������У׼���OFFSETREG��ֵ���������ֵ����
	printf("OFFSETreg data = %X \r\n",Offsetreg_value);
	DAC_GPIO_CLR_N_LOW;     //ʹ���������Ĵ�����ֵ
	ADC_SYNC_N_LOW;         //ADC�ر�ת��
	
	return Offsetreg_value;
	
}
/*********************************************************************************************/
/**
 * @brief У׼6������ͨ����GAIN�Ĵ����ڵ�ֵ
 * @param none
 * @return none
*/
unsigned long AD7172_GAIN_calibration(void)
{
	float volt;
	unsigned short DAC_data;
	int Gainreg_value;

	//����������У׼
  DAC_GPIO_CLR_N_HIGH;     //��ʹ���������Ĵ�����ֵ��Ч������DAC�Ĵ����ڵ�ֵ��Ч
	volt=3;                  //DAC���3V��ADC�������Ϊ2V����2V��Ϊ������2.5V�����룬�Ŵ���1.25������֮���ֵ��Ҫ����1.25
	if(volt<0)
			 DAC_data=volt*32768/10+0XFFFF;
	else
			 DAC_data=volt*32768/10;
	AD5760_SetDacValue(DAC_data);   //ʹDAC��������̵�ƽ
	//����������У׼
	ADC_SYNC_N_HIGH;         //ADC��ʼת��
	Delay(5);
	//ת�����֮��ʼϵͳʧ��У׼
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(7)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
	while(((AD7172_GetRegisterValue(ADC_STATUS_REG_ADDR,1)>>7)&1)==1);    //У׼��ɺ�״̬REG��RDY��Ϊ�͵�ƽ��ʾУ׼���
	Gainreg_value=AD7172_GetRegisterValue(ADC_GAIN0_REG_ADDR,3);      //��������У׼���OFFSETREG��ֵ���������ֵ����
	printf("GAINreg data = %X \r\n",Gainreg_value);
	DAC_GPIO_CLR_N_LOW;     //ʹ���������Ĵ�����ֵ
	ADC_SYNC_N_LOW;         //ADC�ر�ת��
	
	return Gainreg_value;
	
}

