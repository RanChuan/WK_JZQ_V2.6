
#ifndef __OS_TASK_H
#define __OS_TASK_H

#ifdef __cplusplus
	extern "C" {
#endif



/************************************************

				任务调度最多支持32个任务
				

************************************************/


typedef struct
{
	OS_STK *pMYStack;//栈顶指针
	void (*pTask)(void *p_arg);//任务的地址
	INT32U MYDelay_ms;//本次想延时的时间，单位，ms，
	INT32U MYWork;//我的工作，不为0时系统调度这个任务
	INT32U Pend;	//知否挂起
	INT32U TackUsed;//栈使用率
	OS_STK *TackInitial;//初始栈地址
	INT32U LastTime;		//最后一次调度的时间
}OS_TCB,*pOS_TCB;

 


extern				INT32U  OSRunning    ;           // External references系统正在运行标志位
extern        INT32U  OSPrioCur;
extern        INT32U  OSPrioHighRdy;
extern       pOS_TCB  OSTCBCur;							//任务结构体，堆栈地址在首位
extern       pOS_TCB OSTCBHighRdy;
extern        INT32U  OSIntNesting;
extern        INT32U  OSIntExit;




#define TASK_MAX_NUM 32u

//任务未使用的栈默认值，用于做使用统计
#define IDLE_TACK_VALUE 0xffffffff


//定义任务结构体
extern OS_TCB TCB_Table[TASK_MAX_NUM];
  
								//定义任务注册表,
#if TASK_MAX_NUM<=32u
extern INT32U   TASK_Free;
#else
extern INT32U  TASK_Free[TASK_MAX_NUM/32+1];
#endif






INT8U CreateTask (void   (*task)(void *p_arg),void *p_arg,OS_STK  *ptos,INT8U prio)	;		

INT8U CreateTaskN (void   (*task)(void *p_arg),//任务首地址
                     void    *p_arg,					//任务参数
                     OS_STK  *ptos_,						//任务堆栈地址顶，向下生长
										 u32 tacksize,
                     INT8U    prio);						//任务优先级


//校验任务堆栈使用率
void CheckTaskUsege ( void );
										 
//在任务切换时统计任务使用率
void CheckHighRdyTaskUsege ( void );

u16 GetTaskUsed (u8 prio);
u16 GetTaskSize (u8 prio);



void  OSStart (void);
					//任务挂起
INT8U TaskPend (INT32U prio);
INT8U TaskRepend (INT32U prio);




						//任务堆栈初始化
OS_STK *OSTaskStkInit (void (*task)(void *p_arg), void *p_arg, OS_STK *ptos, INT16U opt);




					//任务快结构体初始化
INT8U OS_TCBInit (void (*task)(void *p_arg),OS_STK *ptos,INT8U prio);





				//任务调度初始化
void os_task_init (void);



//唤醒任务，通过优先级确定,这个函数中断调用可以唤醒任务
void TaskIntSendMsg(u8 pro,INT32U msg);

//效果同上，任务调用
u8 TaskSendMsg(u8 pro,INT32U msg);

void TaskMsgZero(void);

//等待消息唤醒
INT32U TaskGetMsg(void);


void OS_Enter_Onlyme(void);

void OS_Exit_Onlyme(void);


			//一定要成对使用
#define OS_ENTER_ONLYME() OS_Enter_Onlyme( )

#define OS_EXIT_ONLYME() OS_Exit_Onlyme()



#ifdef __cplusplus
	}
#endif




#endif


