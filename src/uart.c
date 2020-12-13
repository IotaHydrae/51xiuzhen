#include <REG52.H>
#include "uart.h"
unsigned char index = 0;
unsigned char data buf[20];
void Uart_Init(void)		//9600bps@11.0592MHz
{
	SCON=0X50;			//����Ϊ������ʽ1
	TMOD=0X20;			//���ü�����������ʽ2
	PCON=0X80;			//�����ʼӱ�
	TH1=0XF3;				//��������ʼֵ���ã�ע�Ⲩ������9600��
	TL1=0XF3;
	ES=1;						//�򿪽����ж�
	EA=1;						//�����ж�
	TR1=1;					//�򿪼�����
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
