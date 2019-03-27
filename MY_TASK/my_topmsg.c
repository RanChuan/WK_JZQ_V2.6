#include "includes.h"
#include "key.h"
#include "my_messeg.h"
#include "beep.h"
#include "hard_irq.h"
#include "w5500.h"
#include "timer.h"
#include "my_topmsg.h"


/*********************************************

		顶级消息分发任务：
		分发按键消息，
		各个端口的网络消息
		10ms的软件定时器


***********************************************/



//本文件宏定义
#define SOFT_TIMER_IRQ  (SOFT_MSG|1)


//本文件局域函数
void key_deal (void);
void taskmsg_deal (u32 msg);
void soft_timer_irq (void);
void soft_timer_run (void);

//本文件局域变量
void (**soft_timer_10ms) (void)=0;



void my_topmsg (void *t)
{
	u32 irq_msg=0;
	Key_Init();
	soft_timer_10ms=mymalloc(4*10);
	mymemset(soft_timer_10ms,0,4*10);
	addTimerIrq10ms(soft_timer_irq);
	while(1)
	{
		irq_msg=TaskGetMsg();
		switch (irq_msg)
		{
			case SYS_MSG_KEY:
				key_deal();
				break;
			case SYS_MSG_USART2:
				TaskSendMsg(USART2_GetFocus(),SYS_MSG_USART2);
				break;
			case SYS_MSG_USART1:
				TaskSendMsg(USART1_GetFocus(),SYS_MSG_USART1);
				break;
			case SYS_MSG_TIMER:
				break;
			case SYS_MSG_W5500:
				W5500_IRQ();
				break;
			case SOFT_TIMER_IRQ:
				soft_timer_run();
			default:
				taskmsg_deal (irq_msg);//其他消息处理，
				break;
		}
	}
}


static u8 key[6]={0};
static u8 state[6]={0,0,0,0,0x3f,0};//短按状态
static u8 statel[6]={0,0,0,0,1,0};//长安状态

u8 key_color[3][6]={0};


void key_deal (void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
	u8 recv[MESSEG_DATA]={0};
	
	for (u8 i=0;i<6;i++)//获取按键值并实时反馈
	{
		key[i]=PRESS_NONE;
		key[i]=Get_Key (i+1);
		if (key[i]==PRESS_SHORT)
		{
			light[0]=5;//屏幕蜂鸣器叫
			light[1]=1;
			send_messeg(LCD_MESSEG,light);
			
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STOP;
			light[1]=LIGHT_LIGHT_RUNTO;
			send_messeg(LIT_MESSEG,light);
		}
		else if (key[i]==PRESS_LONG)
		{
			light[0]=5;//屏幕蜂鸣器叫
			light[1]=2;
			send_messeg(LCD_MESSEG,light);
			
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STOP;
			light[1]=LIGHT_LIGHT_RUNTO;
			send_messeg(LIT_MESSEG,light);
		}
		else if (key[i]==PRESS_DOWN)
		{
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STCOR;
			light[1]=LIGHT_LIGHT_RUNTO;
			light[3]=255;
			light[4]=255;
			light[5]=255;
			send_messeg(LIT_MESSEG,light);
		}
		else
		{
		}
	}
		
		if (key[0]==PRESS_SHORT) //按键1短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[0]==PRESS_LONG)//按键1长安
		{
			if (statel[0]==0)
			{
				TaskPend(8);//挂起自动控制
				Load_down();
			}
			else
			{
				TaskRepend(8);//自动控制
				Load_up();
			}
			statel[0]=!statel[0];
		}

		if (key[1]==PRESS_SHORT) //按键2短按
		{
			meg[0]=2;//空调
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;//降按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[1]==PRESS_LONG)//按键2长安
		{
			if (statel[1]==0)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=1;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==1)
			{
				meg[0]=3;meg[1]=3;//彩灯
				meg[2]=2;//闪烁
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=2;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==2)
			{
				meg[0]=3;meg[1]=4;//彩灯
				meg[2]=0;//停止
				meg[3]=200;meg[4]=100;meg[5]=50;//色彩值
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=0;
				Lcd_SetHandstate(0);
			}
		}


		if (key[2]==PRESS_SHORT) //按键3短按
		{
			meg[0]=5;//加湿机
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=1;//升按键
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[2]==PRESS_LONG)//按键3长安
		{
			if (statel[2]==0)
			{
				Load_song3();//一首歌会占用很大的存储空间
			}
			else if (statel[2]==1)
			{
				Load_song1();
			}
			else if (statel[2]==2)
			{
				Load_song();
			}
			else if (statel[2]==3)
			{
				Load_song2();
			}
			statel[2]++;
			if (statel[2]>4) statel[2]=0;
		}

		if (key[3]==PRESS_SHORT) //按键4短按
		{
			meg[0]=3;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[3]==PRESS_LONG)//按键4长安
		{
			meg[0]=2;
			send_messeg(CFG_MESSEG,meg);//切换配置模式
		}


		if (key[4]==PRESS_SHORT) //按键5短按，屏幕控制
		{
			
			if (state[4]>0x30)
			{
				state[4]-=0x30;
			}
			else if (state[4]!=0)
			{
				state[4]=0;
			}
			else
			{
				state[4]=0x3f;
			}
			if (state[4]!=0)
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=0;
				light[2]=LIGHT_LCD_STATE;
			}
			else   //背光为0时亮灯
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
			}
			send_messeg(LIT_MESSEG,light);//空调升温按键
			send_messeg(LCD_MESSEG,meg);//
		}
		else if (key[4]==PRESS_LONG)//按键5长安
		{
			if (statel[4]==0)
			{
				meg[0]=3;meg[1]=0;
				meg[2]=1;meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=1;light[1]=0;
				light[2]=LIGHT_LCD_STATE;
				statel[4]=1;
				send_messeg(LIT_MESSEG,light);
				send_messeg(LCD_MESSEG,meg);//

				
			}
			else
			{
				meg[0]=3;meg[1]=0;
				meg[2]=0;meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=1;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
				statel[4]=0;
				send_messeg(LIT_MESSEG,light);//空调升温按键
				send_messeg(LCD_MESSEG,meg);//
			}
			light[0]=2;light[1]=0;
			light[2]=LIGHT_LCD_STATE;
			send_messeg(LIT_MESSEG,light);
		}



		
		if (key[5]==PRESS_SHORT) //按键6短按
		{
			meg[0]=4;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//按键通用参数
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[5]==PRESS_LONG)//按键6长安
		{
		}

}




void taskmsg_deal (u32 msg)
{
	switch (msg)
	{
		case W5500_SOCKET0:
			TaskSendMsg(SOCKET0_GetFocus(),W5500_SOCKET0);
			break;
		case W5500_SOCKET1:
			TaskSendMsg(SOCKET1_GetFocus(),W5500_SOCKET1);
			break;
		case W5500_SOCKET2:
			TaskSendMsg(SOCKET2_GetFocus(),W5500_SOCKET2);
			break;
		case W5500_SOCKET3:
			TaskSendMsg(SOCKET3_GetFocus(),W5500_SOCKET3);
			break;
		case W5500_SOCKET4:
			TaskSendMsg(SOCKET4_GetFocus(),W5500_SOCKET4);
			break;
		case W5500_SOCKET5:
			TaskSendMsg(SOCKET5_GetFocus(),W5500_SOCKET5);
			break;
		case W5500_SOCKET6:
			TaskSendMsg(SOCKET6_GetFocus(),W5500_SOCKET6);
			break;
		case W5500_SOCKET7:
			TaskSendMsg(SOCKET7_GetFocus(),W5500_SOCKET7);
			break;
		default:
			break;
	}
}











//软件定时器中断，
void soft_timer_irq (void)
{
	TaskIntSendMsg(0,SOFT_TIMER_IRQ);
}


void soft_timer_run (void)
{
	if (soft_timer_10ms)
	{
		for(u8 i=0;*(soft_timer_10ms+i);i++)
		{
			(*(soft_timer_10ms+i))();
		}
	}
}


//外部调用的函数

//0成功，1失败
u8 addSoftTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (soft_timer_10ms[i]==0)
		{
			soft_timer_10ms[i]=irq;
			return 0;
		}
	}
	return 1;
	
}


//清除10ms定时器终端服务函数
u8 delSoftTimerIrq10ms (void (*irq)(void))
{
	for (u8 i=0;i<10;i++)
	{
		if (soft_timer_10ms[i]==irq)
		{
			soft_timer_10ms[i]=0;
			return 0;
		}
	}
	return 1;
}







