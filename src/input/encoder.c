#include <reg52.h>
#include <intrins.h>
#include "head.h"
#include "encoder.h"
#define delay() {_nop_();_nop_();_nop_();}

sfr XICON = 0xc0;
sbit IT3 = XICON^4;
sbit IE3 = XICON^5;
sbit EX3 = XICON^6;

void Encoder_Init(void)
{
    IT3=1;
    EX3=1;
    EA=1; //¿ª×ÜÖÐ¶Ï
}

unsigned char Encoder_Key_Scan()
{
    static bit ec11_flag=0,left_flag=0,right_flag=0;
    unsigned char res=0;
    if(ENCODER_CLK_A&&ENCODER_DT_B)
    {
        ec11_flag = 1;
    }
    if(ENCODER_CLK_A!=ENCODER_DT_B)
    {
        left_flag = ENCODER_CLK_A;
        right_flag = ENCODER_DT_B;
    }
    if(ec11_flag)
    {
        if((ENCODER_CLK_A==0)&&(ENCODER_DT_B==0))
        {
            if(left_flag)
            {
                res='L';
            }
            else if(right_flag)
            {
                res='R';
            }
            ec11_flag = 0;
        }
    }

    return res;
}

