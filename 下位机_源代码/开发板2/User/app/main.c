/*
*********************************************************************************************************
*	                                  
*	ģ������ : ThreadX
*	�ļ����� : mian.c
*	��    �� : V1.0
*	˵    �� : ��Яʽ�绯ѧ����ϵͳʵ��
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
         


/*
*********************************************************************************************************
*                                 �������ȼ�����ֵԽС���ȼ�Խ��
*********************************************************************************************************
*/
#define  APP_CFG_TASK_START_PRIO                          3u
#define  APP_CFG_TASK_MsgPro_PRIO                         5u
#define  APP_CFG_TASK_NetXPro_PRIO                        29u 
#define  APP_CFG_TASK_USER_IF_PRIO                        7u
#define  APP_CFG_TASK_GUI_PRIO                            10u
#define  APP_CFG_TASK_STAT_PRIO                           30u
#define  APP_CFG_TASK_IDLE_PRIO                           31u
#define  APP_CFG_TASK_COM1_PRIO                            1u
#define  APP_CFG_TASK_COM3_PRIO                            2u


/* ��demo_dm9162_netx.c���壬���߲�����28������3 */
//#define  APP_CFG_TASK_NETX_PRIO                         28u
//#define  APP_CFG_TASK_NETX_PRIO1                         3u

/* ��demo_dm9162_netx.c���壬���߲�����29������6 */
//#define  APP_CFG_TASK_NetXPro_PRIO                      29u 
//#define  APP_CFG_TASK_NetXPro_PRIO1                     6u

/* ��ux_port.h���壬*/
//#define UX_THREAD_PRIORITY_CLASS                        4u

/*
*********************************************************************************************************
*                                    ����ջ��С����λ�ֽ�
*********************************************************************************************************
*/
#define  APP_CFG_TASK_START_STK_SIZE                    4096u
#define  APP_CFG_TASK_MsgPro_STK_SIZE                   4096u
#define  APP_CFG_TASK_NETXPRO_STK_SIZE                  4096u
#define  APP_CFG_TASK_GUI_STK_SIZE                      4096u
#define  APP_CFG_TASK_USER_IF_STK_SIZE                  4096u
#define  APP_CFG_TASK_IDLE_STK_SIZE                  	1024u
#define  APP_CFG_TASK_STAT_STK_SIZE                  	1024u
#define  APP_CFG_TASK_COM1_STK_SIZE                  	4096u
#define  APP_CFG_TASK_COM3_STK_SIZE                  	4096u


/*
*********************************************************************************************************
*                                       ��̬ȫ�ֱ���
*********************************************************************************************************
*/                                                        
static  TX_THREAD   AppTaskStartTCB;
static  uint64_t    AppTaskStartStk[APP_CFG_TASK_START_STK_SIZE/8];

static  TX_THREAD   AppTaskMsgProTCB;
static  uint64_t    AppTaskMsgProStk[APP_CFG_TASK_MsgPro_STK_SIZE/8];

TX_THREAD   AppTaskNetXProTCB;
static  uint64_t    AppTaskNetXProStk[APP_CFG_TASK_NETXPRO_STK_SIZE/8];

static  TX_THREAD   AppTaskGUITCB;
static  uint64_t    AppTaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE/8];

static  TX_THREAD   AppTaskUserIFTCB;
static  uint64_t    AppTaskUserIFStk[APP_CFG_TASK_USER_IF_STK_SIZE/8];

static  TX_THREAD   AppTaskIdleTCB;
static  uint64_t    AppTaskIdleStk[APP_CFG_TASK_IDLE_STK_SIZE/8];

static  TX_THREAD   AppTaskStatTCB;
static  uint64_t    AppTaskStatStk[APP_CFG_TASK_STAT_STK_SIZE/8];

static  TX_THREAD   AppTaskCOM1TCB;
static  uint64_t    AppTaskCOM1Stk[APP_CFG_TASK_COM1_STK_SIZE/8];

static  TX_THREAD   AppTaskCOM3TCB;
static  uint64_t    AppTaskCOM3Stk[APP_CFG_TASK_COM3_STK_SIZE/8];


/*
*********************************************************************************************************
*                                      ��������
*********************************************************************************************************
*/
static  void  AppTaskStart          (ULONG thread_input);
static  void  AppTaskMsgPro         (ULONG thread_input);
static  void  AppTaskUserIF         (ULONG thread_input);
static  void  AppTaskGUI			(ULONG thread_input);
static  void  AppTaskIDLE			(ULONG thread_input);
static  void  AppTaskStat			(ULONG thread_input);
static  void  AppTaskNetXPro        (ULONG thread_input);
static  void  AppTaskCOM1           (ULONG thread_input);
static  void  AppTaskCOM3           (ULONG thread_input);

static  void  App_Printf (const char *fmt, ...);
static  void  AppTaskCreate         (void);
static  void  DispTaskInfo          (void);
static  void  AppObjCreate          (void);
static  void  OSStatInit (void);





/*
*******************************************************************************************************
*                               ����
*******************************************************************************************************
*/
static  TX_MUTEX   AppPrintfSemp;	/* ����printf���� */

TX_EVENT_FLAGS_GROUP  EventGroup;

TX_QUEUE RxQueueHandle1;             /* ���ڽ�����Ϣ���� */
TX_QUEUE RxQueueHandle3;             /* ���ڷ�����Ϣ���� */

uint32_t MessageQueuesBuf1[10]; /* ������Ϣ���л���1 */
uint32_t MessageQueuesBuf3[10]; /* ������Ϣ���л���2 */


/* ͳ������ʹ�� */
__IO uint8_t   OSStatRdy;        /* ͳ�����������־ */
__IO uint32_t  OSIdleCtr;        /* ����������� */
__IO float     OSCPUUsage;       /* CPU�ٷֱ� */
uint32_t       OSIdleCtrMax;     /* 1�������Ŀ��м��� */
uint32_t       OSIdleCtrRun;     /* 1���ڿ�������ǰ���� */

/*
*********************************************************************************************************
*	�� �� ��: main
*	����˵��: ��׼c������ڡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
 	/* HAL�⣬MPU��Cache��ʱ�ӵ�ϵͳ��ʼ�� */
	System_Init();

	/* �ں˿���ǰ�ر�HAL��ʱ���׼ */
	HAL_SuspendTick();
	
    /* ����ThreadX�ں� */
    tx_kernel_enter();

	while(1);
}

/*
*********************************************************************************************************
*	�� �� ��: tx_application_define
*	����˵��: ThreadXר�õ����񴴽���ͨ�������������
*	��    ��: first_unused_memory  δʹ�õĵ�ַ�ռ�
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void  tx_application_define(void *first_unused_memory)
{
	/*
	   ���ʵ������CPU������ͳ�ƵĻ����˺���������ʵ����������ͳ������Ϳ����������������ں���
	   AppTaskCreate���洴����
	*/
	/**************������������*********************/
    tx_thread_create(&AppTaskStartTCB,              /* ������ƿ��ַ */   
                       "App Task Start",              /* ������ */
                       AppTaskStart,                  /* ������������ַ */
                       0,                             /* ���ݸ�����Ĳ��� */
                       &AppTaskStartStk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_START_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_START_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_START_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,               /* ������ʱ��Ƭ */
                       TX_AUTO_START);                 /* �������������� */
   	   
	/**************����ͳ������*********************/
    tx_thread_create(&AppTaskStatTCB,               /* ������ƿ��ַ */    
                       "App Task STAT",              /* ������ */
                       AppTaskStat,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskStatStk[0],           /* ��ջ����ַ */
                       APP_CFG_TASK_STAT_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_STAT_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_STAT_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */
					   
				   
	/**************������������*********************/
    tx_thread_create(&AppTaskIdleTCB,               /* ������ƿ��ַ */    
                       "App Task IDLE",              /* ������ */
                       AppTaskIDLE,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskIdleStk[0],           /* ��ջ����ַ */
                       APP_CFG_TASK_IDLE_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_IDLE_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_IDLE_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */		   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStart
*	����˵��: ��������
*	��    ��: thread_input ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (ULONG thread_input)
{

	(void)thread_input;

	/* �ȹ���ʱ���� */
#ifndef TX_NO_TIMER
	tx_thread_suspend(&_tx_timer_thread);
#endif
	
	/* ����ִ������ͳ�� */
	OSStatInit();

	/* �ָ���ʱ���� */
#ifndef TX_NO_TIMER
	tx_thread_resume(&_tx_timer_thread);
#endif	

	/* �ں˿����󣬻ָ�HAL���ʱ���׼ */
    HAL_ResumeTick();
	
    /* �����ʼ�� */
    bsp_Init();
	
	/* �������� */
    AppTaskCreate(); 

	/* ���������ͨ�Ż��� */
	AppObjCreate();	

    while (1)
	{  
		/* ��Ҫ�����Դ���ĳ��򣬶�Ӧ������̵��õ�SysTick_ISR */
        bsp_ProPer1ms();
        tx_thread_sleep(1);
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskMsgPro
*	����˵��: ��Ϣ������������FileX�ļ�ϵͳ
*	��    ��: thread_input ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 5
*********************************************************************************************************
*/
extern void DemoFileX(void);
static void AppTaskMsgPro(ULONG thread_input)
{
	(void)thread_input;

	while(1)
	{
        //DemoFileX();
        tx_thread_sleep(2000);
        
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskNetXPro
*	����˵��: ��Ϣ������������NetX����������
*	��    ��: thread_input ���ڴ���������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: �ϵ���29�����߲����������6
*********************************************************************************************************
*/
extern void NetXTest(void);
static void AppTaskNetXPro(ULONG thread_input)
{
    (void)thread_input;
    
	while(1)
	{
        //NetXTest();
        tx_thread_sleep(2000);
	}   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskUserIF
*	����˵��: ������Ϣ����
*	��    ��: thread_input ����������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 7
*********************************************************************************************************
*/
static void AppTaskUserIF(ULONG thread_input)
{
	uint8_t ucKeyCode;	/* �������� */
	
	(void)thread_input;
    
		  
	while(1)
	{        
		
	    // printf("AppTaskUserIF");

        tx_thread_sleep(2000);
	}
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskGUI
*	����˵��: GUIӦ������
*	��    ��: thread_input ����������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 8
*********************************************************************************************************
*/
static void AppTaskGUI(ULONG thread_input)
{	
    MainTask();
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskStatistic
*	����˵��: ͳ����������ʵ��CPU�����ʵ�ͳ�ơ�Ϊ�˲��Ը���׼ȷ�����Կ���ע�͵��õ�ȫ���жϿ���
*	��    ��: thread_input ����������ʱ���ݵ��β� 
*	�� �� ֵ: ��
*   �� �� ��: 30
*********************************************************************************************************
*/
void  OSStatInit (void)
{
	OSStatRdy = FALSE;
	
    tx_thread_sleep(2u);        /* ʱ��ͬ�� */
	
    //__disable_irq();
    OSIdleCtr    = 0uL;         /* ����м��� */
	//__enable_irq();
	
    tx_thread_sleep(100);       /* ͳ��100ms�ڣ������м��� */
	
   	//__disable_irq();
    OSIdleCtrMax = OSIdleCtr;   /* ���������м��� */
    OSStatRdy    = TRUE;
	//__enable_irq();
}

static void AppTaskStat(ULONG thread_input)
{
	(void)thread_input;

    while (OSStatRdy == FALSE) 
	{
        tx_thread_sleep(200);     /* �ȴ�ͳ��������� */
    }

    OSIdleCtrMax /= 100uL;
    if (OSIdleCtrMax == 0uL) 
	{
        OSCPUUsage = 0u;
    }
	
    //__disable_irq();
    OSIdleCtr = OSIdleCtrMax * 100uL;  /* ���ó�ʼCPU������ 0% */
	//__enable_irq();
	
    for (;;) 
	{
       // __disable_irq();
        OSIdleCtrRun = OSIdleCtr;    /* ���100ms�ڿ��м��� */
        OSIdleCtr    = 0uL;          /* ��λ���м��� */
       //	__enable_irq();            /* ����100ms�ڵ�CPU������ */
        OSCPUUsage   = (100uL - (float)OSIdleCtrRun / OSIdleCtrMax);
        tx_thread_sleep(100);        /* ÿ100msͳ��һ�� */
    }
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskIDLE
*	����˵��: ��������
*	��    ��: thread_input ����������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 31
*********************************************************************************************************
*/
static void AppTaskIDLE(ULONG thread_input)
{	
  TX_INTERRUPT_SAVE_AREA

  (void)thread_input;
	
  while(1)
  {
	   TX_DISABLE
       OSIdleCtr++;
	   TX_RESTORE
  }			  	 	       											   
}

/*
*********************************************************************************************************
*	�� �� ��: AppTaskCreate
*	����˵��: ����Ӧ������
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
	/**************����MsgPro����*********************/
    tx_thread_create(&AppTaskMsgProTCB,               /* ������ƿ��ַ */    
                       "App Msp Pro",                 /* ������ */
                       AppTaskMsgPro,                  /* ������������ַ */
                       0,                             /* ���ݸ�����Ĳ��� */
                       &AppTaskMsgProStk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_MsgPro_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_MsgPro_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_MsgPro_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,               /* ������ʱ��Ƭ */
                       TX_AUTO_START);                /* �������������� */
   

	/**************����USER IF����*********************/
    tx_thread_create(&AppTaskUserIFTCB,               /* ������ƿ��ַ */      
                       "App Task UserIF",              /* ������ */
                       AppTaskUserIF,                  /* ������������ַ */
                       0,                              /* ���ݸ�����Ĳ��� */
                       &AppTaskUserIFStk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_USER_IF_STK_SIZE,  /* ��ջ�ռ��С */  
                       APP_CFG_TASK_USER_IF_PRIO,      /* �������ȼ�*/
                       APP_CFG_TASK_USER_IF_PRIO,      /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,               /* ������ʱ��Ƭ */
                       TX_AUTO_START);                 /* �������������� */

	/**************����GUIX����*********************/
    tx_thread_create(&AppTaskGUITCB,               /* ������ƿ��ַ */    
                       "App Task GUI",              /* ������ */
                       AppTaskGUI,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskGUIStk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_GUI_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_GUI_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_GUI_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */
                       
	/**************����NetX��������*********************/
    tx_thread_create(&AppTaskNetXProTCB,               /* ������ƿ��ַ */    
                      "App NETX Pro",                   /* ������ */
                       AppTaskNetXPro,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskNetXProStk[0],           /* ��ջ����ַ */
                       APP_CFG_TASK_NETXPRO_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_NetXPro_PRIO,    /* �������ȼ�*/
                       APP_CFG_TASK_NetXPro_PRIO,    /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */

    /**************����COM����*********************/
    tx_thread_create(&AppTaskCOM1TCB,               /* ������ƿ��ַ */    
                       "App Task COM1",              /* ������ */
                       AppTaskCOM1,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskCOM1Stk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_COM1_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_COM1_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_COM1_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */
#if 1    
     tx_thread_create(&AppTaskCOM3TCB,               /* ������ƿ��ַ */    
                       "App Task COM3",              /* ������ */
                       AppTaskCOM3,                  /* ������������ַ */
                       0,                           /* ���ݸ�����Ĳ��� */
                       &AppTaskCOM3Stk[0],            /* ��ջ����ַ */
                       APP_CFG_TASK_COM3_STK_SIZE,    /* ��ջ�ռ��С */  
                       APP_CFG_TASK_COM3_PRIO,        /* �������ȼ�*/
                       APP_CFG_TASK_COM3_PRIO,        /* ������ռ��ֵ */
                       TX_NO_TIME_SLICE,             /* ������ʱ��Ƭ */
                       TX_AUTO_START);               /* �������������� */
#endif
}

/*
*********************************************************************************************************
*	�� �� ��: AppObjCreate
*	����˵��: ��������ͨѶ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	 /* ���������ź��� */
    tx_mutex_create(&AppPrintfSemp,"AppPrintfSemp",TX_NO_INHERIT);
    /* �����¼���־�� */
	tx_event_flags_create(&EventGroup, "EventGroupName");
	
	/* ����������Ϣ����1 */
	tx_queue_create(&RxQueueHandle1, 
					"RxQueueHandle1", 
					1,                         /* ÿ����Ϣ���з��͵����ݴ�С����λ32bit����Χ1-16 */
					(VOID *)MessageQueuesBuf1, 
					sizeof(RxQueueHandle1));   /* ��Ϣ���д�С����λ�ֽ� */
	
	/* ����������Ϣ����2 */
	tx_queue_create(&RxQueueHandle3, 
					"RxQueueHandle3", 
					1,                         /* ÿ����Ϣ���з��͵����ݴ�С����λ32bit����Χ1-16 */
					(VOID *)MessageQueuesBuf3, 
					sizeof(RxQueueHandle3));   /* ��Ϣ���д�С����λ�ֽ� */
}

/*
*********************************************************************************************************
*	�� �� ��: App_Printf
*	����˵��: �̰߳�ȫ��printf��ʽ		  			  
*	��    ��: ͬprintf�Ĳ�����
*             ��C�У����޷��г����ݺ���������ʵ�ε����ͺ���Ŀʱ,������ʡ�Ժ�ָ��������
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static  void  App_Printf(const char *fmt, ...)
{
    char  buf_str[200 + 1]; /* �ر�ע�⣬���printf�ı����϶࣬ע��˾ֲ������Ĵ�С�Ƿ��� */
    va_list   v_args;


    va_start(v_args, fmt);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) fmt,
                                  v_args);
    va_end(v_args);

	/* ������� */
    tx_mutex_get(&AppPrintfSemp, TX_WAIT_FOREVER);

    //printf("%s", buf_str);

    tx_mutex_put(&AppPrintfSemp);
}
 
/*
*********************************************************************************************************
*	�� �� ��: DispTaskInfo
*	����˵��: ��ThreadX������Ϣͨ�����ڴ�ӡ����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
	TX_THREAD      *p_tcb;	        /* ����һ��������ƿ�ָ�� */

    p_tcb = &AppTaskStartTCB;
	
	/* ��ӡ���� */
	App_Printf("===============================================================\r\n");
	App_Printf("OS CPU Usage = %5.2f%%\r\n", OSCPUUsage);
	App_Printf("===============================================================\r\n");
	App_Printf(" �������ȼ� ����ջ��С ��ǰʹ��ջ  ���ջʹ��   ������\r\n");
	App_Printf("   Prio     StackSize   CurStack    MaxStack   Taskname\r\n");

	/* ����������ƿ�(TCB list)����ӡ���е���������ȼ������� */
	while (p_tcb != (TX_THREAD *)0) 
	{
		
		App_Printf("   %2d        %5d      %5d       %5d      %s\r\n", 
                    p_tcb->tx_thread_priority,
                    p_tcb->tx_thread_stack_size,
                    (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_ptr,
                    (int)p_tcb->tx_thread_stack_end - (int)p_tcb->tx_thread_stack_highest_ptr,
                    p_tcb->tx_thread_name);


        p_tcb = p_tcb->tx_thread_created_next;

        if(p_tcb == &AppTaskStartTCB) break;
	}
}

#if 0
/*
*********************************************************************************************************
*	�� �� ��: AppTaskCom
*	����˵��: ����������Ϣ����MessageQueues2�����ݽ���
*	��    ��: thread_input ����������ʱ���ݵ��β�
*	�� �� ֵ: ��
	�� �� ��: 5
*********************************************************************************************************
*/
    static void AppTaskCOM1(ULONG thread_input)
    {   
        MSG_T *ptMsg;
        UINT status;
            
        while(1)
        {
            /* ��Ϣ�������ݽ��� */
            status = tx_queue_receive(&RxQueueHandle1, 
                                      &ptMsg,
                                       200);
            
            if(status == TX_SUCCESS)
            {
                /* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
                printf("���յ���Ϣ��������ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
                printf("���յ���Ϣ��������ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
                printf("���յ���Ϣ��������ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
            }
        
        }                                                                      
    }
#endif

static void AppTaskCOM1(ULONG thread_input)
{	
	uint8_t read;
    //App_Printf("COM1\r\n");    
    while(1)
    {
        
		/* ���յ��Ĵ�������� */
		if (comGetChar(COM1, &read))
		{
	        //Printf("COM1����COM3���ͣ�%c\r\n", read);
            //tx_thread_sleep(20);
            //comSendChar(COM1, read);
			comSendChar(COM3, read);
            

		}	
        else
        {
            tx_thread_sleep(5);
        }
        //tx_thread_sleep(5);
	}		
}

static void AppTaskCOM3(ULONG thread_input)
{	
	uint8_t read;
    //App_Printf("COM3\r\n");    
    while(1)
    {
		/* ���յ��Ĵ�������� */
		if (comGetChar(COM3, &read))
		{
		    //App_Printf("COM3����COM1���ͣ�%c\r\n", read);
            //tx_thread_sleep(20);
			comSendChar(COM1, read);
            //nx_tcp_socket_send(&TCPSocket, data_packet, NX_IP_PERIODIC_RATE);
		}
         else
        {
            tx_thread_sleep(5);
        }
        //tx_thread_sleep(5);
	}		
}

#if 0
static void AppTaskCOM3(ULONG thread_input)
{	
	MSG_T *ptMsg;
	UINT status;
        
    while(1)
    {
		/* ��Ϣ�������ݽ��� */
		status = tx_queue_receive(&RxQueueHandle1, 
								  &ptMsg,
								   200);
		
		if(status == TX_SUCCESS)
		{
			/* �ɹ����գ���ͨ�����ڽ����ݴ�ӡ���� */
			printf("���յ���Ϣ��������ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("���յ���Ϣ��������ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			printf("���յ���Ϣ��������ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
		}
	
	}			  	 	       											   
}
#endif

/************************************************************************************/
