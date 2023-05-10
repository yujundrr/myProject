/************************************
 * �ļ���  ��spi.c
 * ����    ��ad5760��ͨѶ���õײ㺯��       
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * Ӳ������: SPI_GPIO_SYNC_N      -->PA4
             SPI_GPIO_CLK         -->PA5
             SPI_GPIO_SDI         -->PA6
             SPI_GPIO_SDO         -->PA7
						 DAC_GPIO_CLR_N       -->PB10
             DAC_GPIO_LDAC_N      -->PB1
             DAC_GPIO_RST_N       -->PA2

**********************************************************************************/
#include "spi.h"
#include "dac.h"

/*����SPI�˿ڣ����ĸ�ΪSPI������*/
 static void DAC_SPI1_GPIO_Config(void)
 {
	 GPIO_InitTypeDef GPIO_InitStructure;
	 
	 /* ʹ��GPIOA��GPIOB��ʱ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); 
	 
	 /*SPI��������,PA4*/
	 /*��Ƭ����SPI NSS���Ŷ��壬DAC��SYNC����*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SYNC_N;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //��ͨ�����������ΪNSSʹ�õ����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	 
	 /*SPI SCLK,PA5*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_CLK;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	 
	 /*��Ƭ����SPI SDI��DAC��SDO,PA6*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SDI;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 
	
	/*��Ƭ����SPI SDO��DAC��SDI,PA7*/
	GPIO_InitStructure.GPIO_Pin = SPI1_GPIO_SDO;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; //�����������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;//����˿ڲ����������Ƶ��
  GPIO_Init(SPI1_GPIO_PORT, &GPIO_InitStructure); 	
	
	//ֹͣ�źţ��տ�ʼ��ʱ���ڿ���״̬
	DAC_SPI1_GPIO_SYNC_N_HIGH;
	
 }
 
 
/*����SPI�Ĺ���ģʽ  */
static void DAC_SPI1_Mode_Config(void)
 {
	 
	 //����һ��SPI�ĳ�ʼ���ṹ��
	 SPI_InitTypeDef  SPI_InitStructure;
	 
	  //��SPI1��ʱ��
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	 
	 //���ýṹ��ľ�������
	 SPI_InitStructure.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_16;  //16����,sclk��Ƶ��Ϊ72/16=4.5MHz����ΪDAC�����Ϊ35MHz
	 SPI_InitStructure.SPI_Mode=SPI_Mode_Master;                        //��ģʽ
	 SPI_InitStructure.SPI_CPHA=SPI_CPHA_2Edge;                         //CPHA=1
	 SPI_InitStructure.SPI_CPOL=SPI_CPOL_Low;                           //CPOL=0 
	 SPI_InitStructure.SPI_CRCPolynomial=7;                             //����û��ʹ��SPI��CRC���ܣ����������ֵ��Ч          
	 SPI_InitStructure.SPI_DataSize=SPI_DataSize_8b;                    //����֡��СΪ8λ
	 SPI_InitStructure.SPI_Direction=SPI_Direction_2Lines_FullDuplex;   //ȫ˫��ͬʱ���պͷ���
	 SPI_InitStructure.SPI_FirstBit=SPI_FirstBit_MSB;                   //MSB����
	 SPI_InitStructure.SPI_NSS=SPI_NSS_Soft;                            //��ʼ�źź�ֹͣ�ź����������
	 
	 //��ʼ��
	 SPI_Init(SPI1,&SPI_InitStructure);
	 
	 //ʹ��SPI
	 SPI_Cmd(SPI1,ENABLE);
	 
 }
 
 void DAC_SPI1_Init(void)
 {
	 //ֻҪ������������Ϳ���
	 DAC_SPI1_GPIO_Config();
	 DAC_SPI1_Mode_Config();
 }
 
 /******************************************************************/
 /**
  * @brief  SPI�ȴ��¼���ʱ������»�����������������
            �����淢��һ���ֽڵĺ���һ�����ã����Ե�֪�ڷ���һ���ֽ�ʱ�Ƿ�����Ҹ���errorCode�ܹ���֪
  * @param  errorCode:������롣����������λ���ĸ����ڳ���
  * @retval ����0����ʾSPI��ȡʧ��
  */
static u8 SPI_TIMEOUT_UserCallback(uint8_t errorCode)
{
	//ʹ�ô���printf���������Ϣ���������
	DAC_SPI_ERROR("SPI�ȴ���ʱ��errorCode=%d",errorCode);
	return 0;
}
 /**
  * @brief  ʹ��SPI��������
  * @param  data:Ҫ���͵�����
  * @param  bytesNumber:Ҫ���͵������ֽڴ�С
  * @retval : ���ش�����Ϣ
  */
u8 SPI_Write(unsigned char *data,unsigned char bytesNumber)
{
	//����ͨѶ��ʱ����
	int i=0;
	unsigned char recieveData[4]={0};    //�յ�������
	uint32_t SPITimeout;
	
	//������ʼ�ź�
	DAC_SPI1_GPIO_SYNC_N_LOW;
	
	//��������
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPIT_FLAG_TIMEOUT;
		//��SPI_I2S_FLAG_TXE=1ʱ��ʾ���ͻ�����Ϊ�գ�����д������
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/*��������*/
	SPI_I2S_SendData(SPI1,data[i]);

	
	//��������
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_RXNE=1ʱ������ջ�������Ϊ�գ����������ݴ���
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(1);
	}
	/*��������*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI1);
  }
	
	Delay(1);
	//����ֹͣ�źţ�ֹͣ����
  DAC_SPI1_GPIO_SYNC_N_HIGH;
	
	return 1;
}
 /**
  * @brief  ��SPI��ȡ����
  * @param  data����:�������ĵ�һ����Ӧ�ö����Ƕ�����д
  * @param  bytesNumber:Ҫ���յ������ֽڴ�С
  * @retval : ���ض�ȡ������
  */
/*! Reads data from SPI. */
unsigned long SPI_Read(unsigned char *data,
                        unsigned char bytesNumber)
{
	int i=0;
	uint32_t SPITimeout;      //��ʱ����
	unsigned char recieveData[4]={0};    //�յ�������
	unsigned long dataRead=0;           //����ת��Ϊһ��32λ������
	unsigned char readback[4]={0,8,0,0};      //NOP
	
	SPI_Write(data,bytesNumber);
	
	Delay(1);
	
	//������ʼ�ź�
	DAC_SPI1_GPIO_SYNC_N_LOW;
	//ͬʱ����ͬʱ����
	for(i=1;i<=bytesNumber;i++)
	{
		SPITimeout=SPIT_FLAG_TIMEOUT;
		//��SPI_I2S_FLAG_TXE=1ʱ��ʾ���ͻ�����Ϊ�գ�����д������
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(0);
	}
	
	/*��������*/
	SPI_I2S_SendData(SPI1,readback[i]);

		
	SPITimeout=SPIT_FLAG_TIMEOUT;
	//��SPI_I2S_FLAG_RXNE=1ʱ������ջ�������Ϊ�գ����������ݴ���
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET)
	{
		if((SPITimeout--)==0) return SPI_TIMEOUT_UserCallback(1);
	}
	/*��������*/
	recieveData[i]=SPI_I2S_ReceiveData(SPI1);
	
	}
		
	Delay(1);
  //����ֹͣ�źţ�ֹͣ����
  DAC_SPI1_GPIO_SYNC_N_HIGH;
	
	for(i = 1;i <= bytesNumber;i ++) 
    {
        dataRead = (dataRead<<8) + recieveData[i];   //registerWord[0]�Ǵ洢�ĸ��ֽ�
    }
	/*�ɹ����ͷ���1*/
	return dataRead;
	
}


												 
 

