/*************************************
 * �ļ���  ��main.c
 * ����    ��ʵ�ֲ���������ѭ������������         
 * ʵ��ƽ̨��    STM32������ ����STM32F103C8T6
 * ��������  ��1.scanf���������ã�����ӳ�亯�������⣬���
               2.DAC��ת����ʽ,������ʱ����ôת�������
							 3.��д����̫��,�´���ʾ�����ٿ���������ɣ���Ҳ��Ϊʲôɨ�ٴﲻ��100��ԭ��
							 4.ADC�ļĴ�������,��ز��������
							 5.ADC��������Ѿ�ת������绯ѧƽ̨һ���ķ�ʽ�ˣ�ֱ�ӿ��Խ������绯ѧƽ̨���бȽϣ����
							 6.���ö�ʱ����ʵ��ɨ�����ʣ���ô��ODR��ϵ�������ϸ����ʱ�䣬��ôͨ��ɨ���������������е�ʱ�䣿 ��ɣ�����100�ﲻ��
**********************************************************************************/

#include "stm32f10x.h"
#include "usart1.h"
#include "adc.h"   
#include "spi.h"
#include "dac.h"
#include "afe.h"
#include "timer.h"

int main(void)
{ 
	/*���¼�������������������*/
  float MinVolt,MaxVolt;//���ڴ�����õĵ�ѹ��Χ����ѹ���޺͵�ѹ����
  int ScanSpeed=0;      //���ڴ��ɨ�����ʣ�
	double pointcount=0;  //�ܹ��ĵ���
	float digitalVolt=0;  //���ֵ�ѹ
	int sample=0;         //��������
	int quit_time=0;      //��Ϣʱ��
	
	/*�Զ������*/
	float time=0;         //ִ��ʱ��
	//long a;
	unsigned long b;
	//unsigned long ID;
	unsigned short D;   //16bits����D�����з��ŵľͿ���ֱ����D=digitalVolt*32768/10;����ת�����������ٷ�digitalVolt������
	long DATA;
	float VIN[10]={0};
	float vin=0;
	int i=0;
	int flag=0;   //�����ж�ѭ�������ķ���,0:����1����С
	int count=0;
	unsigned int gain=0;
	int init=0;
	unsigned long calibration_value[2]={0X800000,0X555770};
	double current=0;
	int cal_flag=0;
	int scanspeed_flag=0;
	int current_accur_measure_flag=0;
	int t;
	int  choose;
	
	/*������ϲ�����6������6����ϲ���*/
//	double a1=0.8615,b1=-0.001;
//	double a2=0.87,b2=0.000228;
//	double a3=0.8673,b3=-0.0001035;
//	double a4=0.8630,b4=-0.000015;
//	double a5=0.8610,b5=0.0001;
//	double a6=-12.25,b6=6.451e-09+3.476e-08;
//		double a1=-1287,b1=1.043e-06;
	double a1=0.8575 ,b1= -7.624e-04;
	double a2= 0.861,b2= -1.933e-04;
	double a3=0.8574,b3=-8.561e-05 ;
	double a4=0.8588,b4=-1.453e-05;
	double a5=0.859 ,b5=-8.656e-06;
	double a6=-12.48,b6=5.575e-07;
	

  /* ����ϵͳʱ��Ϊ72M */      
  SystemInit();	      //��ʼ��ʱ��Ϊ72M
	
  /* ���ô��ڼ�����ͨ��ģ��*/
  USART1_Config();   //
	Delay(1);
	BT_RST_ON;     	   //�⸴λ����������
	
	/*DAC��ʼ�����������*/
	AD5760_Init();                       //DAC��ʼ��
	AD5760_Setup_CTRLREG(DAC_CTRL_RBUF); //����RBUF=1���ϵ��ڲ��Ŵ���������Ϊ1
	AD5760_Setup_CLRCODE(0);             //��������Ĵ���������Ϊ0����ʾ��ʼ��ʱ�����ѹΪ0
	DAC_GPIO_CLR_N_LOW;                  //��CLR=0��������CLR�Ĵ����ڵ�ֵ��ΪDAC�����Ӳ������
	AD5760_EnableOutput(ENABLE);         //ʹ���������OPGND=0��ʹDAC�������
	Delay(1);
	
	/*AFE��ʼ��*/
	ADA4350_SIM_SPI_Init();              //AFE��ʼ��
	Delay(1);
	
	/*ADC�������*/
	ADC_SPI2_Init();                     //ADC��ʼ��
	AD7172_Reset();                      //��λ�ɹ�
	Delay(1);
	AD7172_SetAdcChannal(ADC_CH0_REG_ADDR,CH_SETUP0,AIN0,AIN2);   //����ADC��ͨ��
  AD7172_SETUPCONReg(ADC_SETUPCON0_REG_ADDR,SETUPCON_AINBUF_POS|SETUPCON_AINBUF_NEG|SETUPCON_BI_UNIPOLAR_DOUBLE|SETUPCON_REF_SEL0(0));   //����ADC���������
  
	/*��ʱ����ʼ����*/
	TIM4_Int_Init(999,71);
	TIM4_NVIC_Configuration();
	
	
		
  
	while(1)
	{	
		//�տ�ʼDACʼ�ձ������Ϊ0
		AD5760_Setup_CLRCODE(0);             //��������Ĵ���������Ϊ0����ʾ��ʼ��ʱ�����ѹΪ0
		DAC_GPIO_CLR_N_LOW;                  //��CLR=0��������CLR�Ĵ����ڵ�ֵ��ΪDAC�����Ӳ������
		
    //printf("\r\n�Ƿ���ѭ������������\r\n");
	  while(Get_Integer_value()!=1);
	  printf("\r\nStart measuring!\r\n");

		
    printf("\r\nChoose the measurement method:\r\n");
    //current_accur_measure_flag=Get_Integer_value();
	  current_accur_measure_flag=0;
		
	
  //��ô�õ���������������ϵ���õ�����
	//��������ѭ��������ʱ��Ҫ������Щ����
	//else
  //{
		printf("You have turned on cyclic voltammetry measurements, the measurement starts right away, please wait.\r\n");
	
	
	  printf("\r\nPlease enter the starting voltage:\r\n");
    //MinVolt=Get_Float_value();
	  MinVolt=-0.2;   /****************************20190707��****************************/
	  while(MinVolt>5||MinVolt<-5)
	  {
			printf("�Բ���������ĵ�ѹ���޳�����Χ\r\n");
		  printf("\n�����������ѹ����(��λV)��(���𳬳�-5V��+5V�ķ�Χ)\r\n");
		  MinVolt=Get_Float_value();
	  }
	  printf("Set up successfully! The lower voltage limit you enter is:%.3fV\r\n",MinVolt);
		
	
	  printf("\r\nPlease enter the termination voltage:\r\n");
    //MaxVolt=Get_Float_value();
	  MaxVolt=1.0;  /****************************20190707��****************************/
	  while(MaxVolt>5||MaxVolt<-5)
		{
			printf("�Բ���������ĵ�ѹ���޳�����Χ\r\n");
		  printf("\n�����������ѹ����(��λV)��(���𳬳�-5V��+5V�ķ�Χ)\r\n");
		  MaxVolt=Get_Float_value();
	  }
	  printf("Set up successfully! The voltage limit you enter is:%.3fV\r\n",MaxVolt);
		 
	
	  printf("\r\nPlease enter the scan rate:\r\n");
//		ScanSpeed=Get_Integer_value();
    ScanSpeed=100;

    scanspeed_flag=0;   //Ĭ��Ϊ1
	  while(scanspeed_flag==0)
    {
			if(ScanSpeed==1)
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0576);    //���ODR��������ôӰ��ADC��ת�����ʵ�
					scanspeed_flag=1;
	      }
	    else if(ScanSpeed==5)
	      {
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0574);
          scanspeed_flag=1;		
	      }		//����ADC���˲�������������ODR��
	    else if(ScanSpeed==10)
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0573); 
					scanspeed_flag=1;
	      }
	    else if(ScanSpeed==20)
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0571);  
					scanspeed_flag=1;
	      }
	    else if(ScanSpeed==50)	
	      {
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0570); 
					scanspeed_flag=1;
	      }
	    else if(ScanSpeed==100)	
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X056E); 
					scanspeed_flag=1;
	      }
			else if(ScanSpeed==200)	
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X056D); 
		      scanspeed_flag=1;
	      }
	    else if(ScanSpeed==500)	
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X056B); 
					scanspeed_flag=1;
	      }
			else if(ScanSpeed==1000)	
	      {
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X056A);
		      scanspeed_flag=1;
	      }
	    else 
	      {
					scanspeed_flag=0;
		      printf("�������ɨ��������������1/5/10/20/50/100/200/500/1000����ѡ��\r\n");
		      printf("����������ɨ�����ʣ�(!!!ע�ⵥλ��mV/s)\r\n");
		      ScanSpeed=Get_Integer_value();
	      }
    }
	  printf("The scan rate you entered is:%dmV/s\r\n",ScanSpeed);
	
	
	  printf("\r\nPlease enter the scan direction:\r\n");
    //init=Get_Integer_value();
    init=1;
	  if(init==1)
	  {
			printf("The scan direction you entered is positive.\r\n");
		  digitalVolt=MinVolt;   //����Сֵ��ʼ����,����ɨ��
		  flag=0;   //�տ�ʼ������ķ���
	  }
	  else
	  {
		  printf("The scan direction you entered is reverse.\r\n");
		  digitalVolt=MaxVolt;   //�����ֵ��ʼ��С������ɨ��
		  flag=1;   //�տ�ʼ�Ǽ�С�ķ���
	  }
	
    //gain=Get_Integer_value();
		gain = 10000;
  	if(gain==1)
  		gain=1000;
  	else if(gain==2)
  		gain=5000;
  	else if(gain==3)
  		gain=10000;
  	else if(gain==4)
  		gain=50000;
  	else if(gain==5)
  		gain=100000;
  	else if(gain==6)
  		gain=1000000;

	  cal_flag=Get_Integer_value();
	//����ı����Ժ�Ҫ����һ��У׼
	  if(cal_flag==1)
	  {
			AFE_gain_cal(calibration_value);   //��ÿ���������У׼��ÿ��У׼֮ǰ��Ҫʹ�ñ�׼�迹ģ��
	    printf("You have turned on calibration\r\n");
	  }
		else
	  {
			AFE_gain_nocal(calibration_value);      //ʹ��У׼���ʧ��������ϵ���������ã�ֻҪ���治�ı�
		  printf("You have not turned on calibration\r\n");
	  }
	

	  printf("\r\nPlease enter the number of sampling points:\r\n");
    //sample=Get_Integer_value();
	  sample=1;
	  while(sample>20||sample<1)
	  {
			printf("�Բ���������Ĳ�������������Χ\r\n");
		  printf("\n�������������������(���𳬳�1��10�ķ�Χ)\r\n");
		  sample=Get_Integer_value();
	  }
    printf("The number of sampling points you enter is:%d\r\n",sample);
	
	
	  printf("\r\nPlease enter a rest time:\r\n");
    //quit_time=Get_Integer_value();
	  quit_time=2;
	  while(quit_time>50||quit_time<1)
			{
				printf("�Բ���������ľ�Ϣʱ�䳬����Χ\r\n");
		    printf("\n���������뾲Ϣʱ��(��λs)�����𳬳�1��10�ķ�Χ)\r\n");
		    quit_time=Get_Integer_value();
	    }
	  printf("The quiet time you enter is:%ds\r\n",quit_time);
	
//}	
	
	
	
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//����ADC�Ĺ���ģʽ��ʱ��Դ
	AD7172_IFMODEReg(IFMODE_DOUT_RESET);                                  //ʹRDY�ӳ��������CS��Ϊ�ߵ�ƽ֮������RDY�ź�
	printf("\r\nCH0REG data = %X \r\n",AD7172_GetRegisterValue(ADC_CH0_REG_ADDR,2));		
	printf("SETUPCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_SETUPCON0_REG_ADDR,2));
	printf("FILTCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_FILTCON0_REG_ADDR,2));
	printf("ADCMODE data = %X \r\n", AD7172_GetRegisterValue(ADC_ADCMODE_REG_ADDR,2));
	printf("IFMODE data = %X \r\n", AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2));
	calibration_value[0]=AD7172_GetRegisterValue(ADC_OFFSET0_REG_ADDR,3);
	calibration_value[1]= AD7172_GetRegisterValue(ADC_GAIN0_REG_ADDR,3);
	printf("OFFSETreg data = %X \r\n",calibration_value[0]);
	printf("GAINreg data = %X \r\n\n",calibration_value[1]);
		 
	
	//��ȡ��ʼ��ѹ�Ķ�������
	if(digitalVolt<0)
			 D=digitalVolt*32768/10+0XFFFF;
	else
			 D=digitalVolt*32768/10;
	
	delay_timer_ms(100);
	//���ݴ�����ʼ��־
	//printf("START\r\n");

	
	DAC_GPIO_CLR_N_HIGH;     //��ʹ���������Ĵ�����ֵ��Ч������DAC�Ĵ����ڵ�ֵ��Ч
	AD5760_SetDacValue(D);   //ʹ����һ����ʼ��ѹ
//	Delay((quit_time+1.5)*1000);   //��Ϣʱ��
	delay_timer_ms(quit_time*1000);  //�ö�ʱ����ʱ����׼ȷ
	ADC_SYNC_N_HIGH;         //ADC��ʼת��
	
	//ѭ�������ĳ�ʼ��
  count=0;                 //ÿ�ν���һ��ɨ��ǰҪ��ʼ��Ϊ0����Ȼ�޷����У�count==2����һ��ѭ�������������
	for(i=0;i<sample;i++)    //����������Ϊ0���������⽫�ϴεĽ�������¶�Ӱ����εĽ��
	    VIN[i]=0;
	

  while (1)      //���յ���λ������Ϣ������ѭ��������
  {
		 //��������������ʼ�ݼ�
		 if(digitalVolt>MaxVolt)
		 {
			 flag=1;  //��С����
			 if(current_accur_measure_flag)
				 digitalVolt=MaxVolt-0.4;
			 else
			   digitalVolt=MaxVolt-0.001;
			 count++; 
		 }
		 //���ݼ���������ʼ����
		 if(digitalVolt<MinVolt)
		 {
			 flag=0;  //������
			 if(current_accur_measure_flag)
				 digitalVolt=MinVolt+0.4;
			 else 
			 digitalVolt=MinVolt+0.001;
			 count++;
		 }
		 //�����õĵ�ѹֵ����DAC����DAC�����Ӧ�ĵ�ѹ
		 if(count>=10)
		 {
			 //printf("END");
				 break;
		 }
		 else
		 {
			 if(digitalVolt<0)
				 D=digitalVolt*32768/10+0XFFFF;    //D��һ��16λ�����Ҳ���˫���ԣ�
		   else
			   D=digitalVolt*32768/10;

		   AD5760_SetDacValue(D);    //DAC���ģ����
			 if(count!=200)
		   printf("%.3f, ",digitalVolt);
		 }
		 
		 //�����ݼ����ı��ѹֵ
		 if(flag==0)   //������
		 {
			if(current_accur_measure_flag)  //������������
				digitalVolt=digitalVolt+0.4;
			else
			   digitalVolt=digitalVolt+0.001;//ѭ��������
		 }
		 else         //��С����
		 {
			 if(current_accur_measure_flag)   //�������Ȳ���
				 digitalVolt=digitalVolt-0.4;
			 else
				 digitalVolt=digitalVolt-0.001; //ѭ������������
		 }

		 
//	  printf("\nFILTCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_FILTCON0_REG_ADDR,2));

		 //�������õ�ɨ����������ʱ�Ĵ�С����ʱ50��20��10��׼ȷ��
		 t=1000*1.0/ScanSpeed;
		 t=t-20;
//		if(current_accur_measure_flag)
//			Delay(100);
//		else
			delay_timer_ms(t);
		
		//    ��ʼת��ADC
		vin=0;    
		current=0;
		for(i=0;i<sample;i++)
		{
			DATA=AD7172_ReadFromDATAReg_1();    //����ת��ģʽ�µĶ�ȡ����
		  //printf("\nADC data = %X \r\n",DATA);//�����������ѹ��������������
      //�������������ʵ�����缫ϵͳ�ĵ�ѹ������AFE��������ǵ��������缫ϵͳ�൱��һ����أ������Ļ�������������ֵ��绯ѧƽ̨����һģһ���ĲŶ�
		  //VIN[i]=-(DATA-0X800000)*2.500*0X400000/0X555770/8388608/0.75*14.99/10;  
		  //VIN[i]=-(((DATA-0X800000)*2.499*0X400000/Gainreg_value/8388608/0.75)+((Offsetreg_value-0X800000)*2.499/0.75/0X80000))*14.99/10; 
		  //VIN[i]=-((0XFFFFFF-DATA)*2.499/0X800000-2.499);
		  VIN[i]=((DATA-0X800000)*2.499/0X800000)*14.99/10*0.8;   //����ɾ����һ��*0.8,��ΪУ׼����0.8����2V������������У׼
		  vin=vin+VIN[i];
		}
		current=(vin*1.0/sample)/gain;  //����ɵ���
		
		//������ϣ���У׼�Ժ����������
		if(gain==1000)
	{		
    current=a1*current-b1/1000;
	}
	else if(gain==5000)
	{
		current=a2*current-b2/1000;	                                                                        
	}		
	else if(gain==10000)
	{
		current=a3*current-b3/1000;
	}
	else if(gain==50000)
	{
		current=a4*current-b4/1000;
	}
	else if(gain==100000)	
	{
		current=a5*current-b5/1000;
	}
	else if(gain==1000000)	
	{
		current=a6*current+b6/1000;
	}
		
	
	if(count!=10)                      //�������
	{
		printf("%.4e\r\n",current);    //20190701������\n�Ա����Ժ���vs2017�Ͻ������ݴ���
//		printf(" %.4e\r\n",vin);     //   \r\nֻ�Ǵ�����һ��
	}
			
//  if(current_accur_measure_flag)    //ֻ�в�����������ʱ��ִ������
//		while(Get_Integer_value()!=1);
//			
  }
}
}
	




