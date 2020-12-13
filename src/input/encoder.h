#ifndef __ENCODER_H__
#define __ENCODER_H__
#include <reg52.h>
sfr P4 = 0xe8;
sbit ENCODER_CLK_A = P1^0; 
sbit ENCODER_DT_B = P1^1;
sbit ENCODER_SW	= P4^2;
void Encoder_Init(void);
unsigned char Encoder_Key_Scan();
#endif