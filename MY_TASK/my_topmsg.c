#include "includes.h"
#include "key.h"
#include "my_messeg.h"
#include "beep.h"
#include "hard_irq.h"
#include "w5500.h"
#include "timer.h"
#include "my_topmsg.h"


/*********************************************

		������Ϣ�ַ�����
		�ַ�������Ϣ��
		�����˿ڵ�������Ϣ
		10ms�������ʱ��


***********************************************/



//���ļ��궨��
#define SOFT_TIMER_IRQ  (SOFT_MSG|1)


//���ļ�������
void key_deal (void);
void taskmsg_deal (u32 msg);
void soft_timer_irq (void);
void soft_timer_run (void);

//���ļ��������
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
				taskmsg_deal (irq_msg);//������Ϣ����
				break;
		}
	}
}


static u8 key[6]={0};
static u8 state[6]={0,0,0,0,0x3f,0};//�̰�״̬
static u8 statel[6]={0,0,0,0,1,0};//����״̬

u8 key_color[3][6]={0};


void key_deal (void)
{
	u8 meg[MESSEG_DATA]={0};
	u8 light[MESSEG_DATA]={0};
	u8 recv[MESSEG_DATA]={0};
	
	for (u8 i=0;i<6;i++)//��ȡ����ֵ��ʵʱ����
	{
		key[i]=PRESS_NONE;
		key[i]=Get_Key (i+1);
		if (key[i]==PRESS_SHORT)
		{
			light[0]=5;//��Ļ��������
			light[1]=1;
			send_messeg(LCD_MESSEG,light);
			
			light[0]=LIGHT_ROUND_LIGHT;
			light[2]=LIGHT_LIGHT_STOP;
			light[1]=LIGHT_LIGHT_RUNTO;
			send_messeg(LIT_MESSEG,light);
		}
		else if (key[i]==PRESS_LONG)
		{
			light[0]=5;//��Ļ��������
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
		
		if (key[0]==PRESS_SHORT) //����1�̰�
		{
			meg[0]=2;//�յ�
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=1;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[0]==PRESS_LONG)//����1����
		{
			if (statel[0]==0)
			{
				TaskPend(8);//�����Զ�����
				Load_down();
			}
			else
			{
				TaskRepend(8);//�Զ�����
				Load_up();
			}
			statel[0]=!statel[0];
		}

		if (key[1]==PRESS_SHORT) //����2�̰�
		{
			meg[0]=2;//�յ�
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[1]==PRESS_LONG)//����2����
		{
			if (statel[1]==0)
			{
				meg[0]=3;meg[1]=4;//�ʵ�
				meg[2]=2;//��˸
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=1;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==1)
			{
				meg[0]=3;meg[1]=3;//�ʵ�
				meg[2]=2;//��˸
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=2;
				Lcd_SetHandstate(1);
			}
			else if (statel[1]==2)
			{
				meg[0]=3;meg[1]=4;//�ʵ�
				meg[2]=0;//ֹͣ
				meg[3]=200;meg[4]=100;meg[5]=50;//ɫ��ֵ
				send_messeg(LIT_MESSEG,meg);//
				statel[1]=0;
				Lcd_SetHandstate(0);
			}
		}


		if (key[2]==PRESS_SHORT) //����3�̰�
		{
			meg[0]=5;//��ʪ��
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=1;//������
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[2]==PRESS_LONG)//����3����
		{
			if (statel[2]==0)
			{
				Load_song3();//һ�׸��ռ�úܴ�Ĵ洢�ռ�
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

		if (key[3]==PRESS_SHORT) //����4�̰�
		{
			meg[0]=3;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[3]==PRESS_LONG)//����4����
		{
			meg[0]=2;
			send_messeg(CFG_MESSEG,meg);//�л�����ģʽ
		}


		if (key[4]==PRESS_SHORT) //����5�̰�����Ļ����
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
			else   //����Ϊ0ʱ����
			{
				meg[0]=3;meg[1]=1;
				meg[2]=state[4];meg[3]=0;
				meg[4]=1;meg[5]=0;
				light[0]=2;light[1]=1;
				light[2]=LIGHT_LCD_STATE;
			}
			send_messeg(LIT_MESSEG,light);//�յ����°���
			send_messeg(LCD_MESSEG,meg);//
		}
		else if (key[4]==PRESS_LONG)//����5����
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
				send_messeg(LIT_MESSEG,light);//�յ����°���
				send_messeg(LCD_MESSEG,meg);//
			}
			light[0]=2;light[1]=0;
			light[2]=LIGHT_LCD_STATE;
			send_messeg(LIT_MESSEG,light);
		}



		
		if (key[5]==PRESS_SHORT) //����6�̰�
		{
			meg[0]=4;
			meg[1]=2;meg[2]=0;meg[3]=0;meg[4]=2;//����ͨ�ò���
			meg[5]=2;
			send_messeg(RF_MESSEG,meg);
		}
		else if (key[5]==PRESS_LONG)//����6����
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











//�����ʱ���жϣ�
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


//�ⲿ���õĺ���

//0�ɹ���1ʧ��
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


//���10ms��ʱ���ն˷�����
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







