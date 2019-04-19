#ifndef  __MY_LIGHT_H
#define  __MY_LIGHT_H




#ifdef __cplusplus
	extern "C" {
#endif





		
void led_light (u16 num,u8 state,u8 red,u8 green,u8 blue);


			//流水
void led_run (u8 *meg);

						//键盘周围的灯闪烁
void light_run (u8 *msg);


//周围灯光渐变至指定颜色
void light_runto (u8 *msg);


			//灯带亮纯色
void led_lightall (u8 *meg);

		//灯光任务函数
void my_light (void * t);



#ifdef __cplusplus
	}
#endif




#endif

