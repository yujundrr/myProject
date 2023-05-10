/*
*********************************************************************************************************
*	                                  
*	模块名称 : ThreadX
*	文件名称 : mian.c
*	版    本 : V1.0
*	说    明 : 便携式电化学测量系统实现
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
         


/*
*********************************************************************************************************
*                                 任务优先级，数值越小优先级越高
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


/* 在demo_dm9162_netx.c定义，网线插入后从28提升到3 */
//#define  APP_CFG_TASK_NETX_PRIO                         28u
//#define  APP_CFG_TASK_NETX_PRIO1                         3u

/* 在demo_dm9162_netx.c定义，网线插入后从29提升到6 */
//#define  APP_CFG_TASK_NetXPro_PRIO                      29u 
//#define  APP_CFG_TASK_NetXPro_PRIO1                     6u

/* 在ux_port.h定义，*/
//#define UX_THREAD_PRIORITY_CLASS                        4u

/*
*********************************************************************************************************
*                                    任务栈大小，单位字节
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
*                                       静态全局变量
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
*                                      函数声明
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
*                               变量
*******************************************************************************************************
*/
static  TX_MUTEX   AppPrintfSemp;	/* 用于printf互斥 */

TX_EVENT_FLAGS_GROUP  EventGroup;

TX_QUEUE RxQueueHandle1;             /* 用于接收消息队列 */
TX_QUEUE RxQueueHandle3;             /* 用于发送消息队列 */

uint32_t MessageQueuesBuf1[10]; /* 定义消息队列缓冲1 */
uint32_t MessageQueuesBuf3[10]; /* 定义消息队列缓冲2 */


/* 统计任务使用 */
__IO uint8_t   OSStatRdy;        /* 统计任务就绪标志 */
__IO uint32_t  OSIdleCtr;        /* 空闲任务计数 */
__IO float     OSCPUUsage;       /* CPU百分比 */
uint32_t       OSIdleCtrMax;     /* 1秒内最大的空闲计数 */
uint32_t       OSIdleCtrRun;     /* 1秒内空闲任务当前计数 */

/*
*********************************************************************************************************
*	函 数 名: main
*	功能说明: 标准c程序入口。
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
 	/* HAL库，MPU，Cache，时钟等系统初始化 */
	System_Init();

	/* 内核开启前关闭HAL的时间基准 */
	HAL_SuspendTick();
	
    /* 进入ThreadX内核 */
    tx_kernel_enter();

	while(1);
}

/*
*********************************************************************************************************
*	函 数 名: tx_application_define
*	功能说明: ThreadX专用的任务创建，通信组件创建函数
*	形    参: first_unused_memory  未使用的地址空间
*	返 回 值: 无
*********************************************************************************************************
*/
void  tx_application_define(void *first_unused_memory)
{
	/*
	   如果实现任务CPU利用率统计的话，此函数仅用于实现启动任务，统计任务和空闲任务，其它任务在函数
	   AppTaskCreate里面创建。
	*/
	/**************创建启动任务*********************/
    tx_thread_create(&AppTaskStartTCB,              /* 任务控制块地址 */   
                       "App Task Start",              /* 任务名 */
                       AppTaskStart,                  /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       &AppTaskStartStk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_START_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_START_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_START_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,               /* 不开启时间片 */
                       TX_AUTO_START);                 /* 创建后立即启动 */
   	   
	/**************创建统计任务*********************/
    tx_thread_create(&AppTaskStatTCB,               /* 任务控制块地址 */    
                       "App Task STAT",              /* 任务名 */
                       AppTaskStat,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskStatStk[0],           /* 堆栈基地址 */
                       APP_CFG_TASK_STAT_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_STAT_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_STAT_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */
					   
				   
	/**************创建空闲任务*********************/
    tx_thread_create(&AppTaskIdleTCB,               /* 任务控制块地址 */    
                       "App Task IDLE",              /* 任务名 */
                       AppTaskIDLE,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskIdleStk[0],           /* 堆栈基地址 */
                       APP_CFG_TASK_IDLE_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_IDLE_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_IDLE_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */		   
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStart
*	功能说明: 启动任务。
*	形    参: thread_input 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 2
*********************************************************************************************************
*/
static  void  AppTaskStart (ULONG thread_input)
{

	(void)thread_input;

	/* 先挂起定时器组 */
#ifndef TX_NO_TIMER
	tx_thread_suspend(&_tx_timer_thread);
#endif
	
	/* 优先执行任务统计 */
	OSStatInit();

	/* 恢复定时器组 */
#ifndef TX_NO_TIMER
	tx_thread_resume(&_tx_timer_thread);
#endif	

	/* 内核开启后，恢复HAL里的时间基准 */
    HAL_ResumeTick();
	
    /* 外设初始化 */
    bsp_Init();
	
	/* 创建任务 */
    AppTaskCreate(); 

	/* 创建任务间通信机制 */
	AppObjCreate();	

    while (1)
	{  
		/* 需要周期性处理的程序，对应裸机工程调用的SysTick_ISR */
        bsp_ProPer1ms();
        tx_thread_sleep(1);
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskMsgPro
*	功能说明: 消息处理，这里用作FileX文件系统
*	形    参: thread_input 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 5
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
*	函 数 名: AppTaskNetXPro
*	功能说明: 消息处理，这里用作NetX网络任务处理
*	形    参: thread_input 是在创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 上电是29，网线插入后提升至6
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
*	函 数 名: AppTaskUserIF
*	功能说明: 按键消息处理
*	形    参: thread_input 创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 7
*********************************************************************************************************
*/
static void AppTaskUserIF(ULONG thread_input)
{
	uint8_t ucKeyCode;	/* 按键代码 */
	
	(void)thread_input;
    
		  
	while(1)
	{        
		
	    // printf("AppTaskUserIF");

        tx_thread_sleep(2000);
	}
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskGUI
*	功能说明: GUI应用任务
*	形    参: thread_input 创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 8
*********************************************************************************************************
*/
static void AppTaskGUI(ULONG thread_input)
{	
    MainTask();
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskStatistic
*	功能说明: 统计任务，用于实现CPU利用率的统计。为了测试更加准确，可以开启注释调用的全局中断开关
*	形    参: thread_input 创建该任务时传递的形参 
*	返 回 值: 无
*   优 先 级: 30
*********************************************************************************************************
*/
void  OSStatInit (void)
{
	OSStatRdy = FALSE;
	
    tx_thread_sleep(2u);        /* 时钟同步 */
	
    //__disable_irq();
    OSIdleCtr    = 0uL;         /* 清空闲计数 */
	//__enable_irq();
	
    tx_thread_sleep(100);       /* 统计100ms内，最大空闲计数 */
	
   	//__disable_irq();
    OSIdleCtrMax = OSIdleCtr;   /* 保存最大空闲计数 */
    OSStatRdy    = TRUE;
	//__enable_irq();
}

static void AppTaskStat(ULONG thread_input)
{
	(void)thread_input;

    while (OSStatRdy == FALSE) 
	{
        tx_thread_sleep(200);     /* 等待统计任务就绪 */
    }

    OSIdleCtrMax /= 100uL;
    if (OSIdleCtrMax == 0uL) 
	{
        OSCPUUsage = 0u;
    }
	
    //__disable_irq();
    OSIdleCtr = OSIdleCtrMax * 100uL;  /* 设置初始CPU利用率 0% */
	//__enable_irq();
	
    for (;;) 
	{
       // __disable_irq();
        OSIdleCtrRun = OSIdleCtr;    /* 获得100ms内空闲计数 */
        OSIdleCtr    = 0uL;          /* 复位空闲计数 */
       //	__enable_irq();            /* 计算100ms内的CPU利用率 */
        OSCPUUsage   = (100uL - (float)OSIdleCtrRun / OSIdleCtrMax);
        tx_thread_sleep(100);        /* 每100ms统计一次 */
    }
}

/*
*********************************************************************************************************
*	函 数 名: AppTaskIDLE
*	功能说明: 空闲任务
*	形    参: thread_input 创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 31
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
*	函 数 名: AppTaskCreate
*	功能说明: 创建应用任务
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static  void  AppTaskCreate (void)
{
	/**************创建MsgPro任务*********************/
    tx_thread_create(&AppTaskMsgProTCB,               /* 任务控制块地址 */    
                       "App Msp Pro",                 /* 任务名 */
                       AppTaskMsgPro,                  /* 启动任务函数地址 */
                       0,                             /* 传递给任务的参数 */
                       &AppTaskMsgProStk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_MsgPro_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_MsgPro_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_MsgPro_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,               /* 不开启时间片 */
                       TX_AUTO_START);                /* 创建后立即启动 */
   

	/**************创建USER IF任务*********************/
    tx_thread_create(&AppTaskUserIFTCB,               /* 任务控制块地址 */      
                       "App Task UserIF",              /* 任务名 */
                       AppTaskUserIF,                  /* 启动任务函数地址 */
                       0,                              /* 传递给任务的参数 */
                       &AppTaskUserIFStk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_USER_IF_STK_SIZE,  /* 堆栈空间大小 */  
                       APP_CFG_TASK_USER_IF_PRIO,      /* 任务优先级*/
                       APP_CFG_TASK_USER_IF_PRIO,      /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,               /* 不开启时间片 */
                       TX_AUTO_START);                 /* 创建后立即启动 */

	/**************创建GUIX任务*********************/
    tx_thread_create(&AppTaskGUITCB,               /* 任务控制块地址 */    
                       "App Task GUI",              /* 任务名 */
                       AppTaskGUI,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskGUIStk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_GUI_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_GUI_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_GUI_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */
                       
	/**************创建NetX处理任务*********************/
    tx_thread_create(&AppTaskNetXProTCB,               /* 任务控制块地址 */    
                      "App NETX Pro",                   /* 任务名 */
                       AppTaskNetXPro,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskNetXProStk[0],           /* 堆栈基地址 */
                       APP_CFG_TASK_NETXPRO_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_NetXPro_PRIO,    /* 任务优先级*/
                       APP_CFG_TASK_NetXPro_PRIO,    /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */

    /**************创建COM任务*********************/
    tx_thread_create(&AppTaskCOM1TCB,               /* 任务控制块地址 */    
                       "App Task COM1",              /* 任务名 */
                       AppTaskCOM1,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskCOM1Stk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_COM1_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_COM1_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_COM1_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */
#if 1    
     tx_thread_create(&AppTaskCOM3TCB,               /* 任务控制块地址 */    
                       "App Task COM3",              /* 任务名 */
                       AppTaskCOM3,                  /* 启动任务函数地址 */
                       0,                           /* 传递给任务的参数 */
                       &AppTaskCOM3Stk[0],            /* 堆栈基地址 */
                       APP_CFG_TASK_COM3_STK_SIZE,    /* 堆栈空间大小 */  
                       APP_CFG_TASK_COM3_PRIO,        /* 任务优先级*/
                       APP_CFG_TASK_COM3_PRIO,        /* 任务抢占阀值 */
                       TX_NO_TIME_SLICE,             /* 不开启时间片 */
                       TX_AUTO_START);               /* 创建后立即启动 */
#endif
}

/*
*********************************************************************************************************
*	函 数 名: AppObjCreate
*	功能说明: 创建任务通讯
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
static  void  AppObjCreate (void)
{
	 /* 创建互斥信号量 */
    tx_mutex_create(&AppPrintfSemp,"AppPrintfSemp",TX_NO_INHERIT);
    /* 创建事件标志组 */
	tx_event_flags_create(&EventGroup, "EventGroupName");
	
	/* 创建接收消息队列1 */
	tx_queue_create(&RxQueueHandle1, 
					"RxQueueHandle1", 
					1,                         /* 每次消息队列发送的数据大小，单位32bit，范围1-16 */
					(VOID *)MessageQueuesBuf1, 
					sizeof(RxQueueHandle1));   /* 消息队列大小，单位字节 */
	
	/* 创建接收消息队列2 */
	tx_queue_create(&RxQueueHandle3, 
					"RxQueueHandle3", 
					1,                         /* 每次消息队列发送的数据大小，单位32bit，范围1-16 */
					(VOID *)MessageQueuesBuf3, 
					sizeof(RxQueueHandle3));   /* 消息队列大小，单位字节 */
}

/*
*********************************************************************************************************
*	函 数 名: App_Printf
*	功能说明: 线程安全的printf方式		  			  
*	形    参: 同printf的参数。
*             在C中，当无法列出传递函数的所有实参的类型和数目时,可以用省略号指定参数表
*	返 回 值: 无
*********************************************************************************************************
*/
static  void  App_Printf(const char *fmt, ...)
{
    char  buf_str[200 + 1]; /* 特别注意，如果printf的变量较多，注意此局部变量的大小是否够用 */
    va_list   v_args;


    va_start(v_args, fmt);
   (void)vsnprintf((char       *)&buf_str[0],
                   (size_t      ) sizeof(buf_str),
                   (char const *) fmt,
                                  v_args);
    va_end(v_args);

	/* 互斥操作 */
    tx_mutex_get(&AppPrintfSemp, TX_WAIT_FOREVER);

    //printf("%s", buf_str);

    tx_mutex_put(&AppPrintfSemp);
}
 
/*
*********************************************************************************************************
*	函 数 名: DispTaskInfo
*	功能说明: 将ThreadX任务信息通过串口打印出来
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
static void DispTaskInfo(void)
{
	TX_THREAD      *p_tcb;	        /* 定义一个任务控制块指针 */

    p_tcb = &AppTaskStartTCB;
	
	/* 打印标题 */
	App_Printf("===============================================================\r\n");
	App_Printf("OS CPU Usage = %5.2f%%\r\n", OSCPUUsage);
	App_Printf("===============================================================\r\n");
	App_Printf(" 任务优先级 任务栈大小 当前使用栈  最大栈使用   任务名\r\n");
	App_Printf("   Prio     StackSize   CurStack    MaxStack   Taskname\r\n");

	/* 遍历任务控制块(TCB list)，打印所有的任务的优先级和名字 */
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
*	函 数 名: AppTaskCom
*	功能说明: 这里用作消息队列MessageQueues2的数据接收
*	形    参: thread_input 创建该任务时传递的形参
*	返 回 值: 无
	优 先 级: 5
*********************************************************************************************************
*/
    static void AppTaskCOM1(ULONG thread_input)
    {   
        MSG_T *ptMsg;
        UINT status;
            
        while(1)
        {
            /* 消息队列数据接收 */
            status = tx_queue_receive(&RxQueueHandle1, 
                                      &ptMsg,
                                       200);
            
            if(status == TX_SUCCESS)
            {
                /* 成功接收，并通过串口将数据打印出来 */
                printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
                printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
                printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
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
        
		/* 接收到的串口命令处理 */
		if (comGetChar(COM1, &read))
		{
	        //Printf("COM1接收COM3发送：%c\r\n", read);
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
		/* 接收到的串口命令处理 */
		if (comGetChar(COM3, &read))
		{
		    //App_Printf("COM3接收COM1发送：%c\r\n", read);
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
		/* 消息队列数据接收 */
		status = tx_queue_receive(&RxQueueHandle1, 
								  &ptMsg,
								   200);
		
		if(status == TX_SUCCESS)
		{
			/* 成功接收，并通过串口将数据打印出来 */
			printf("接收到消息队列数据ptMsg->ucMessageID = %d\r\n", ptMsg->ucMessageID);
			printf("接收到消息队列数据ptMsg->ulData[0] = %d\r\n", ptMsg->ulData[0]);
			printf("接收到消息队列数据ptMsg->usData[0] = %d\r\n", ptMsg->usData[0]);
		}
	
	}			  	 	       											   
}
#endif

/************************************************************************************/
