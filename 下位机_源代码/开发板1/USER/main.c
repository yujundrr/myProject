/*************************************
 * 文件名  ：main.c
 * 描述    ：实现参数测量和循环伏安法测量         
 * 实验平台：    STM32开发板 基于STM32F103C8T6
 * 调试问题  ：1.scanf函数不能用，串口映射函数有问题，完成
               2.DAC的转换公式,负数的时候怎么转换，完成
							 3.读写周期太长,下次用示波器再看看，待完成，这也是为什么扫速达不到100的原因
							 4.ADC的寄存器设置,相关参数，完成
							 5.ADC测量结果已经转换成与电化学平台一样的方式了，直接可以将结果与电化学平台进行比较，完成
							 6.利用定时器来实现扫描速率，怎么与ODR联系起来，严格控制时间，怎么通过扫描速率来控制运行的时间？ 完成，但是100达不到
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
	/*以下几个参数均由蓝牙传来*/
  float MinVolt,MaxVolt;//用于存放设置的电压范围，电压下限和电压上限
  int ScanSpeed=0;      //用于存放扫描速率；
	double pointcount=0;  //总共的点数
	float digitalVolt=0;  //数字电压
	int sample=0;         //采样点数
	int quit_time=0;      //静息时间
	
	/*自定义变量*/
	float time=0;         //执行时间
	//long a;
	unsigned long b;
	//unsigned long ID;
	unsigned short D;   //16bits，把D换成有符号的就可以直接用D=digitalVolt*32768/10;进行转换，而不用再分digitalVolt的正负
	long DATA;
	float VIN[10]={0};
	float vin=0;
	int i=0;
	int flag=0;   //用于判断循环伏安的方向,0:增大。1：减小
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
	
	/*数据拟合参数，6个增益6组拟合参数*/
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
	

  /* 配置系统时钟为72M */      
  SystemInit();	      //初始化时钟为72M
	
  /* 配置串口及蓝牙通信模块*/
  USART1_Config();   //
	Delay(1);
	BT_RST_ON;     	   //解复位，开启蓝牙
	
	/*DAC初始化及相关配置*/
	AD5760_Init();                       //DAC初始化
	AD5760_Setup_CTRLREG(DAC_CTRL_RBUF); //设置RBUF=1，断掉内部放大器，增益为1
	AD5760_Setup_CLRCODE(0);             //设置清零寄存器的数据为0，表示初始化时输出电压为0
	DAC_GPIO_CLR_N_LOW;                  //将CLR=0，代表让CLR寄存器内的值作为DAC输出，硬件控制
	AD5760_EnableOutput(ENABLE);         //使能输出，令OPGND=0，使DAC输出正常
	Delay(1);
	
	/*AFE初始化*/
	ADA4350_SIM_SPI_Init();              //AFE初始化
	Delay(1);
	
	/*ADC相关配置*/
	ADC_SPI2_Init();                     //ADC初始化
	AD7172_Reset();                      //复位成功
	Delay(1);
	AD7172_SetAdcChannal(ADC_CH0_REG_ADDR,CH_SETUP0,AIN0,AIN2);   //设置ADC的通道
  AD7172_SETUPCONReg(ADC_SETUPCON0_REG_ADDR,SETUPCON_AINBUF_POS|SETUPCON_AINBUF_NEG|SETUPCON_BI_UNIPOLAR_DOUBLE|SETUPCON_REF_SEL0(0));   //设置ADC的相关配置
  
	/*定时器初始化，*/
	TIM4_Int_Init(999,71);
	TIM4_NVIC_Configuration();
	
	
		
  
	while(1)
	{	
		//刚开始DAC始终保持输出为0
		AD5760_Setup_CLRCODE(0);             //设置清零寄存器的数据为0，表示初始化时输出电压为0
		DAC_GPIO_CLR_N_LOW;                  //将CLR=0，代表让CLR寄存器内的值作为DAC输出，硬件控制
		
    //printf("\r\n是否开启循环伏安法测量\r\n");
	  while(Get_Integer_value()!=1);
	  printf("\r\nStart measuring!\r\n");

		
    printf("\r\nChoose the measurement method:\r\n");
    //current_accur_measure_flag=Get_Integer_value();
	  current_accur_measure_flag=0;
		
	
  //怎么用电流测量那里的拟合系数用到这里
	//开启的是循环伏安法时才要设置这些参数
	//else
  //{
		printf("You have turned on cyclic voltammetry measurements, the measurement starts right away, please wait.\r\n");
	
	
	  printf("\r\nPlease enter the starting voltage:\r\n");
    //MinVolt=Get_Float_value();
	  MinVolt=-0.2;   /****************************20190707改****************************/
	  while(MinVolt>5||MinVolt<-5)
	  {
			printf("对不起，您输入的电压下限超出范围\r\n");
		  printf("\n请重新输入电压下限(单位V)：(请勿超出-5V到+5V的范围)\r\n");
		  MinVolt=Get_Float_value();
	  }
	  printf("Set up successfully! The lower voltage limit you enter is:%.3fV\r\n",MinVolt);
		
	
	  printf("\r\nPlease enter the termination voltage:\r\n");
    //MaxVolt=Get_Float_value();
	  MaxVolt=1.0;  /****************************20190707改****************************/
	  while(MaxVolt>5||MaxVolt<-5)
		{
			printf("对不起，您输入的电压上限超出范围\r\n");
		  printf("\n请重新输入电压上限(单位V)：(请勿超出-5V到+5V的范围)\r\n");
		  MaxVolt=Get_Float_value();
	  }
	  printf("Set up successfully! The voltage limit you enter is:%.3fV\r\n",MaxVolt);
		 
	
	  printf("\r\nPlease enter the scan rate:\r\n");
//		ScanSpeed=Get_Integer_value();
    ScanSpeed=100;

    scanspeed_flag=0;   //默认为1
	  while(scanspeed_flag==0)
    {
			if(ScanSpeed==1)
				{
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0576);    //这个ODR到底是怎么影响ADC的转换速率的
					scanspeed_flag=1;
	      }
	    else if(ScanSpeed==5)
	      {
					AD7172_FILTCONReg(ADC_FILTCON0_REG_ADDR,0X0574);
          scanspeed_flag=1;		
	      }		//设置ADC的滤波器参数，包括ODR等
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
		      printf("您输入的扫描速率有误，请在1/5/10/20/50/100/200/500/1000等中选择\r\n");
		      printf("请重新输入扫描速率：(!!!注意单位是mV/s)\r\n");
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
		  digitalVolt=MinVolt;   //从最小值开始增大,正向扫描
		  flag=0;   //刚开始是增大的方向
	  }
	  else
	  {
		  printf("The scan direction you entered is reverse.\r\n");
		  digitalVolt=MaxVolt;   //从最大值开始减小，反向扫描
		  flag=1;   //刚开始是减小的方向
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
	//增益改变了以后都要进行一次校准
	  if(cal_flag==1)
	  {
			AFE_gain_cal(calibration_value);   //对每个增益进行校准，每次校准之前需要使用标准阻抗模型
	    printf("You have turned on calibration\r\n");
	  }
		else
	  {
			AFE_gain_nocal(calibration_value);      //使用校准后的失调和增益系数进行设置，只要增益不改变
		  printf("You have not turned on calibration\r\n");
	  }
	

	  printf("\r\nPlease enter the number of sampling points:\r\n");
    //sample=Get_Integer_value();
	  sample=1;
	  while(sample>20||sample<1)
	  {
			printf("对不起，您输入的采样点数超出范围\r\n");
		  printf("\n请重新输入采样点数：(请勿超出1到10的范围)\r\n");
		  sample=Get_Integer_value();
	  }
    printf("The number of sampling points you enter is:%d\r\n",sample);
	
	
	  printf("\r\nPlease enter a rest time:\r\n");
    //quit_time=Get_Integer_value();
	  quit_time=2;
	  while(quit_time>50||quit_time<1)
			{
				printf("对不起，您输入的静息时间超出范围\r\n");
		    printf("\n请重新输入静息时间(单位s)：请勿超出1到10的范围)\r\n");
		    quit_time=Get_Integer_value();
	    }
	  printf("The quiet time you enter is:%ds\r\n",quit_time);
	
//}	
	
	
	
	AD7172_ADCMODEReg(ADCMODE_REF_EN|ADCMODE_MODE(0)|ADCMODE_CLOCKSEL(0));//设置ADC的工作模式及时钟源
	AD7172_IFMODEReg(IFMODE_DOUT_RESET);                                  //使RDY延迟输出，在CS变为高电平之后才输出RDY信号
	printf("\r\nCH0REG data = %X \r\n",AD7172_GetRegisterValue(ADC_CH0_REG_ADDR,2));		
	printf("SETUPCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_SETUPCON0_REG_ADDR,2));
	printf("FILTCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_FILTCON0_REG_ADDR,2));
	printf("ADCMODE data = %X \r\n", AD7172_GetRegisterValue(ADC_ADCMODE_REG_ADDR,2));
	printf("IFMODE data = %X \r\n", AD7172_GetRegisterValue(ADC_IFMODE_REG_ADDR,2));
	calibration_value[0]=AD7172_GetRegisterValue(ADC_OFFSET0_REG_ADDR,3);
	calibration_value[1]= AD7172_GetRegisterValue(ADC_GAIN0_REG_ADDR,3);
	printf("OFFSETreg data = %X \r\n",calibration_value[0]);
	printf("GAINreg data = %X \r\n\n",calibration_value[1]);
		 
	
	//获取初始电压的二进制数
	if(digitalVolt<0)
			 D=digitalVolt*32768/10+0XFFFF;
	else
			 D=digitalVolt*32768/10;
	
	delay_timer_ms(100);
	//数据传送起始标志
	//printf("START\r\n");

	
	DAC_GPIO_CLR_N_HIGH;     //不使能清零代码寄存器的值有效，即让DAC寄存器内的值有效
	AD5760_SetDacValue(D);   //使其有一个初始电压
//	Delay((quit_time+1.5)*1000);   //静息时间
	delay_timer_ms(quit_time*1000);  //用定时器定时更加准确
	ADC_SYNC_N_HIGH;         //ADC开始转换
	
	//循环变量的初始化
  count=0;                 //每次进行一次扫描前要初始化为0，不然无法进行，count==2代表一个循环伏安检测周期
	for(i=0;i<sample;i++)    //将数组设置为0，这样避免将上次的结果保留下而影响这次的结果
	    VIN[i]=0;
	

  while (1)      //当收到上位机的消息，开启循环伏安法
  {
		 //当递增结束，开始递减
		 if(digitalVolt>MaxVolt)
		 {
			 flag=1;  //减小方向
			 if(current_accur_measure_flag)
				 digitalVolt=MaxVolt-0.4;
			 else
			   digitalVolt=MaxVolt-0.001;
			 count++; 
		 }
		 //当递减结束，开始递增
		 if(digitalVolt<MinVolt)
		 {
			 flag=0;  //增大方向
			 if(current_accur_measure_flag)
				 digitalVolt=MinVolt+0.4;
			 else 
			 digitalVolt=MinVolt+0.001;
			 count++;
		 }
		 //将设置的电压值赋给DAC，让DAC输出相应的电压
		 if(count>=10)
		 {
			 //printf("END");
				 break;
		 }
		 else
		 {
			 if(digitalVolt<0)
				 D=digitalVolt*32768/10+0XFFFF;    //D是一个16位数，且采用双极性，
		   else
			   D=digitalVolt*32768/10;

		   AD5760_SetDacValue(D);    //DAC输出模拟量
			 if(count!=200)
		   printf("%.3f, ",digitalVolt);
		 }
		 
		 //递增递减，改变电压值
		 if(flag==0)   //增大方向
		 {
			if(current_accur_measure_flag)  //测量电流精度
				digitalVolt=digitalVolt+0.4;
			else
			   digitalVolt=digitalVolt+0.001;//循环伏安法
		 }
		 else         //减小方向
		 {
			 if(current_accur_measure_flag)   //电流精度测试
				 digitalVolt=digitalVolt-0.4;
			 else
				 digitalVolt=digitalVolt-0.001; //循环伏安法测量
		 }

		 
//	  printf("\nFILTCON0 data = %X \r\n", AD7172_GetRegisterValue(ADC_FILTCON0_REG_ADDR,2));

		 //根据设置的扫速来控制延时的大小，暂时50、20、10是准确的
		 t=1000*1.0/ScanSpeed;
		 t=t-20;
//		if(current_accur_measure_flag)
//			Delay(100);
//		else
			delay_timer_ms(t);
		
		//    开始转换ADC
		vin=0;    
		current=0;
		for(i=0;i<sample;i++)
		{
			DATA=AD7172_ReadFromDATAReg_1();    //连续转换模式下的读取数据
		  //printf("\nADC data = %X \r\n",DATA);//不能输出负电压的数字量？？？
      //这里算出来的是实际三电极系统的电压，除以AFE的增益就是电流，三电极系统相当于一个电池，这样的话，测量出来的值与电化学平台的是一模一样的才对
		  //VIN[i]=-(DATA-0X800000)*2.500*0X400000/0X555770/8388608/0.75*14.99/10;  
		  //VIN[i]=-(((DATA-0X800000)*2.499*0X400000/Gainreg_value/8388608/0.75)+((Offsetreg_value-0X800000)*2.499/0.75/0X80000))*14.99/10; 
		  //VIN[i]=-((0XFFFFFF-DATA)*2.499/0X800000-2.499);
		  VIN[i]=((DATA-0X800000)*2.499/0X800000)*14.99/10*0.8;   //这里删除了一个*0.8,因为校准才有0.8，把2V当做是满量程校准
		  vin=vin+VIN[i];
		}
		current=(vin*1.0/sample)/gain;  //换算成电流
		
		//数据拟合，在校准以后做数据拟合
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
		
	
	if(count!=10)                      //输出电流
	{
		printf("%.4e\r\n",current);    //20190701加上了\n以便于以后在vs2017上进行数据处理
//		printf(" %.4e\r\n",vin);     //   \r\n只是代表换了一行
	}
			
//  if(current_accur_measure_flag)    //只有测量电流精度时才执行这里
//		while(Get_Integer_value()!=1);
//			
  }
}
}
	




