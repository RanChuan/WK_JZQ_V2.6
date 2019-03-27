#ifndef HARD_IRQ_H__
#define HARD_IRQ_H__



#ifdef __cplusplus
	extern "C" {
#endif

		
		
		
		


//定义各种中断唤醒的值		
#define SYS_MSG_KEY					(SYS_MSG|1)		//按键输入中断
#define SYS_MSG_USART2   		(SYS_MSG|2)		//串口2中断，接LCD屏
#define SYS_MSG_USART1			(SYS_MSG|3)		//串口1中断，接RF
#define SYS_MSG_TIMER				(SYS_MSG|4)		//软件定时器，10ms
#define SYS_MSG_W5500				(SYS_MSG|5)		//网络中断

#define TASK_MSG_KEY				(TASK_MSG|1)		//按键输入中断
#define TASK_MSG_USART2   	(TASK_MSG|2)		//串口2中断，接LCD屏
#define TASK_MSG_USART1			(TASK_MSG|3)		//串口1中断，接RF
#define TASK_MSG_TIMER			(TASK_MSG|4)		//软件定时器，10ms
#define TASK_MSG_W5500			(TASK_MSG|5)		//网络中断

//网络端口消息定义
#define W5500_SOCKET        (TASK_MSG|0x0100)
#define W5500_SOCKET0				(W5500_SOCKET|0)
#define W5500_SOCKET1				(W5500_SOCKET|1)
#define W5500_SOCKET2				(W5500_SOCKET|2)
#define W5500_SOCKET3				(W5500_SOCKET|3)
#define W5500_SOCKET4				(W5500_SOCKET|4)
#define W5500_SOCKET5				(W5500_SOCKET|5)
#define W5500_SOCKET6				(W5500_SOCKET|6)
#define W5500_SOCKET7				(W5500_SOCKET|7)


void USART1_SetFocus(u8 focus);
void USART2_SetFocus(u8 focus);

u8 USART1_GetFocus(void);
u8 USART2_GetFocus(void);

void SOCKET0_SetFocus (u8 focus);
void SOCKET1_SetFocus (u8 focus);
void SOCKET2_SetFocus (u8 focus);
void SOCKET3_SetFocus (u8 focus);
void SOCKET4_SetFocus (u8 focus);
void SOCKET5_SetFocus (u8 focus);
void SOCKET6_SetFocus (u8 focus);
void SOCKET7_SetFocus (u8 focus);


u8 SOCKET0_GetFocus (void);
u8 SOCKET1_GetFocus (void);
u8 SOCKET2_GetFocus (void);
u8 SOCKET3_GetFocus (void);
u8 SOCKET4_GetFocus (void);
u8 SOCKET5_GetFocus (void);
u8 SOCKET6_GetFocus (void);
u8 SOCKET7_GetFocus (void);





		
#ifdef __cplusplus
	}
#endif


#endif







