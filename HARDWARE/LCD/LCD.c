#include "includes.h"
#include "hard_irq.h"
#include "lcd.h"

/**************************************************
		文件说明：串口屏驱动，
		引脚使用：PA2，串口数据输出；PA3，串口数据输入；PB12，485时的数据流向控制脚
		外部引用：无
		实现方法：中断接收到缓存，阻塞式发送
		使用方法：使用读取函数读取接收到的数据，发送函数发送

****************************************************/







#define MY_USART USART2
#define USART_485_TX_EN		PBout(12)	//485模式控制.0,接收;1,发送.
#define USART2_RX_BUFLEN  30

//接收缓存区 	
static u8 USART2_RX_BUF[USART2_RX_BUFLEN];  	//接收缓冲,最大64个字节.

//接收到的数据长度
static u8 USART2_RX_CNT=0;   		  

//串口空闲标志位
static u8 USART2_IDLE=1;
  
	
	
	
	
void USART2_IRQHandler(void)
{
	u8 res;	    
 
 	if(USART_GetITStatus(MY_USART, USART_IT_RXNE) != RESET) //接收到数据
	{	  
		res =USART_ReceiveData(MY_USART); 	//读取接收到的数据
		if ((USART2_RX_CNT==0)&&(USART2_IDLE==1))
		{
			USART2_IDLE=0;
		}
		if (USART2_IDLE==0)
		{
			if(USART2_RX_CNT<USART2_RX_BUFLEN)
			{
				USART2_RX_BUF[USART2_RX_CNT]=res;			//记录接收到的值
				USART2_RX_CNT++;											//接收数据增加1 
			}
		}
	}  											 
	if(USART_GetITStatus(MY_USART, USART_IT_IDLE) != RESET)  //空闲中断
	{
		res=MY_USART->SR;
		res=MY_USART->DR;
		res=0;
		USART2_IDLE=1;
		TaskIntSendMsg(0,SYS_MSG_USART2);//发送给焦点进程
	}
} 


void LCD_SetFocus(u8 focus)
{
//	if (focus<TASK_MAX_NUM)
//		LCD_FOCUS=focus;
}



void LCD_Init(u32 bound)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);//使能GPIOA,G时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//使能GPIOA,G时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);//使能USART2时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;				 //PG9端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	//PA2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; //浮空输入
	GPIO_Init(GPIOA, &GPIO_InitStructure);  

	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,ENABLE);//复位串口2
	RCC_APB1PeriphResetCmd(RCC_APB1Periph_USART2,DISABLE);//停止复位


	USART_InitStructure.USART_BaudRate = bound;//一般设置为9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//8位数据长度
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;///奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//收发模式

	USART_Init(MY_USART, &USART_InitStructure);  //初始化串口

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn; //使能串口2中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; //先占优先级2级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; //从优先级2级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能外部中断通道
	NVIC_Init(&NVIC_InitStructure); //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

	USART_ITConfig(MY_USART, USART_IT_RXNE, ENABLE);//开启中断
	USART_ITConfig(MY_USART, USART_IT_IDLE, ENABLE);//开启空闲中断

	USART_Cmd(MY_USART, ENABLE);                    //使能串口 


	USART_485_TX_EN=0;			//默认为接收模式

}



//RS485发送len个字节.
//buf:发送区首地址
//len:发送的字节数(为了和本代码的接收匹配,这里建议不要超过64个字节)
void LCD_Send_Data(u8 *buf,u8 len)
{
	u8 t;
	USART_485_TX_EN=1;			//设置为发送模式
	for(t=0;t<len;t++)		//循环发送数据
	{		   
		while(USART_GetFlagStatus(MY_USART, USART_FLAG_TC) == RESET);	  
		USART_SendData(MY_USART,buf[t]);
	}	 
	while(USART_GetFlagStatus(MY_USART, USART_FLAG_TC) == RESET);		
	USART_485_TX_EN=0;				//设置为接收模式	
}



//RS485查询接收到的数据
//buf:接收缓存首地址
//len:读到的数据长度
void LCD_Receive_Data(u8 *buf,u16 *len)
{
	u8 rxlen=USART2_RX_CNT;
	*len=USART2_RX_CNT;				//默认为0
	if (USART2_IDLE==1)
	{
		mymemcpy (buf,USART2_RX_BUF,rxlen);
		mymemset (USART2_RX_BUF,0,rxlen);
		USART2_RX_CNT=0;
	}
}

















