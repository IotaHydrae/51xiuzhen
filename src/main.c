#include <reg52.h>
#include "oled.h"
#include "head.h"
#include "game_logic.h"
#include "input/encoder.h"

sfr AUXR = 0x8e;

void System_Init()
{
	AUXR=0x01;
    OLEDInit();
    Encoder_Init();
    //Uart_Init();
    OLEDPrint(0,0,"Encoder_Init", LEFT, INVERT);
    //Delayms(1000);
    OLEDClear();
}

int main(void)
{


    System_Init();
    Game_Init();
    Game_Start();

    //page_change();
    /* ??Ϸ??ѭ?? */
    while(1)
    {
        Game_Loop();
        
    }
}


