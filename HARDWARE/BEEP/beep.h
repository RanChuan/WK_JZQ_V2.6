
#ifndef __BEEP_H
#define __BEEP_H

#ifdef  __cplusplus
	extern "C" {
#endif



#define BEEP PCout(13)

typedef struct 
{
	char tone[4];//声调，1_低音，1，中音，1~，高音 0，间歇
	char time[4];//1/8，8分之一音，1/4，四分之一音
} jianpu;


void BEEP_Init(void);

void Beep_Play (jianpu *jianpu_);
void Beep_End (void);

void set_jianpu(jianpu *j,char *tone_,char *time_);
void jianpu_end(jianpu *j);

void Load_song(void);
void Load_song1(void);
void Load_song2(void);
void Load_song3(void);

void Load_up(void);
void Load_down(void);




#ifdef  __cplusplus
	}
#endif





#endif



