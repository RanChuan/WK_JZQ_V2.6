#ifndef MY_TOPMSG_H__
#define MY_TOPMSG_H__


#ifdef __cplusplus
	extern "C" {
#endif



		
		
		
		
		

//������Ϣ�ַ�����
void my_topmsg (void *);


		
u8 addSoftTimerIrq10ms (void (*irq)(void));
		
u8 delSoftTimerIrq10ms (void (*irq)(void));
		
		
		
		
		
#ifdef __cplusplus
	}
#endif


#endif











