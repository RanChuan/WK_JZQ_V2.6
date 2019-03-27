#include "includes.h"
#include "w5500.h"
#include "enternet.h"
#include "baidu_iot.h"
#include "my_topmsg.h"
#include "my_iot.h"

//IOT��ʱ����ʱ�ж�
void IOT_Hander(void);

u8 MQTT_STATE=0;

void my_iot (void *t)
{
	u32 msg=0;
	u8 *buff=mymalloc(2048);
	u16 len=0;
	u16 temper=0;
	
	addSoftTimerIrq10ms(IOT_Hander);
	
	while(1)
	{
		delay_ms(5000);//�����ӵ��Ʒ�����
	}
	
	
}







void IOT_Hander(void)
{
	static u8 i=0;
	static u16 sec=0;
	if (i<100)
	{
		i++;
	}
	else 
	{
		i=0;
		if (sec<60)
		{
			sec++;
		}
		else
		{
			sec=0;
			//���﻽��������������
			//TaskIntSendMsg(10,2);
		}		
	}
	
}



