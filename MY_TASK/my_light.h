#ifndef  __MY_LIGHT_H
#define  __MY_LIGHT_H




#ifdef __cplusplus
	extern "C" {
#endif





		
void led_light (u16 num,u8 state,u8 red,u8 green,u8 blue);


			//��ˮ
void led_run (u8 *meg);

						//������Χ�ĵ���˸
void light_run (u8 *msg);


//��Χ�ƹ⽥����ָ����ɫ
void light_runto (u8 *msg);


			//�ƴ�����ɫ
void led_lightall (u8 *meg);

		//�ƹ�������
void my_light (void * t);



#ifdef __cplusplus
	}
#endif




#endif

