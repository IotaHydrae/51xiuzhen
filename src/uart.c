#include <REG52.H>
#include "uart.h"
unsigned char index = 0;
unsigned char data buf[20];
void Uart_Init(void)		//9600bps@11.0592MHz
{
	SCON=0X50;			//设置为工作方式1
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X80;			//波特率加倍
	TH1=0XF3;				//计数器初始值设置，注意波特率是9600的
	TL1=0XF3;
	ES=1;						//打开接收中断
	EA=1;						//打开总中断
	TR1=1;					//打开计数器
}

/*void Uart_Isr() interrupt 4 using 1
{
	if(RI==0)return;
	ES=0;
	RI=0;
	buf[index++]=SBUF;
	if(SBUF=='!');
	ES=1;

}	*/

void Uart_SendData(unsigned char *dat)
{
    ES = 0;
    while(*dat != '\0')
	{
		SBUF = *dat;
    	dat++;
		while(!TI);
    	TI = 0;
	}
    ES = 1;
}
