#include <reg52.h>
#include <intrins.h>
#include "gpio_spi.h"
#include "oledfont.h"
#include "head.h"

static unsigned char index_line = 0;

struct Position
{
    /* 
        page£ºÒ³
        col: ÁĞ
        rem£ºÒ³ÓàÊı
     */
    int page;
    int col;
    int rem;
};

static void OLED_Set_DC(unsigned char val)
{
	if(val)
		DC = 1;
	else
		DC = 0;
}

static void OLED_Set_CS(unsigned char val)
{
	if(val)
		CS = 1;
	else
		CS = 0;
}

static void OLEDWriteCmd(unsigned char cmd)
{
	OLED_Set_DC(0); /*command*/
	OLED_Set_CS(0); /* selected */

	SPISendByte(cmd);

	OLED_Set_DC(1);
	OLED_Set_CS (1);
}

static void OLEDWriteDat(unsigned char dat)
{
	OLED_Set_DC(1); /*dat*/
	OLED_Set_CS(0); /* selected */

	SPISendByte(dat);

	OLED_Set_DC(1);
	OLED_Set_CS (1);
}

static void OLEDSetPos(unsigned char page,unsigned char col)
{
	/* è®¾ç½®é¡µåœ°å€ */
	OLEDWriteCmd(0xB0+page);

	/* è®¾ç½®åˆ—åœ°å€ */
	OLEDWriteCmd(0x00|(col&0x0f));	      /* ä½åœ°å€ */
	OLEDWriteCmd(0x10|(col >> 4));  /* é«˜åœ°å€ */
}

/* OLEDÁÁ¶È¿ØÖÆ 256¼¶ */
void OLEDSetBrightness(unsigned char value)
{

}

void OLEDClear(void)
{
	int page, col;
	for(page = 0;page < 8;page++)
	{
		OLEDSetPos(page,0);
		for (col = 0; col<128;col++)
			OLEDWriteDat(0);
	}
}

/*
	A[1:0] = 00b, Horizontal Addressing Mode
	A[1:0] = 01b, Vertical Addressing Mode
	A[1:0] = 10b, Page Addressing Mode (RESET)
	A[1:0] = 11b, Invalid
	
*/
static void OLEDSetMemoryAddressingMode(int mode)
{
	OLEDWriteCmd(0x20);
	switch(mode)
	{
		case 0:OLEDWriteCmd(0x00);break;	//Horizontal Addressing Mode
		case 1:OLEDWriteCmd(0x01);break;	//Vertical Addressing Mode
		case 2:OLEDWriteCmd(0x02);break;	//Page Addressing Mode
		case 3:OLEDWriteCmd(0x03);break;	//Invalid
		default:OLEDWriteCmd(0x02);break;	//Page Addressing Mode
	}
	
}
void OLEDInit(void)
{
	OLEDWriteCmd(0xae);	//--turn off oled panel

	OLEDWriteCmd(0x00);	//--set low column address
	OLEDWriteCmd(0x10);	//--set high column address

	OLEDWriteCmd(0x40);	//--set start line address

	OLEDWriteCmd(0xb0);	//--set page address

	OLEDWriteCmd(0x81);	//--set contrast control register
	OLEDWriteCmd(0xff);

	OLEDWriteCmd(0xa1);	//--set segment re-map 127 to 0   a0:0 to seg127
	OLEDWriteCmd(0xa6);	//--set normal display

	OLEDWriteCmd(0xc9);	//--set com(N-1)to com0  c0:com0 to com(N-1)

	OLEDWriteCmd(0xa8);	//--set multiples ratio(1to64)
	OLEDWriteCmd(0x3f);	//--1/64 duty

	OLEDWriteCmd(0xd3);	//--set display offset
	OLEDWriteCmd(0x00);	//--not offset

	OLEDWriteCmd(0xd5);	//--set display clock divide ratio/oscillator frequency
	OLEDWriteCmd(0x80);	//--set divide ratio

	OLEDWriteCmd(0xd9);	//--set pre-charge period
	OLEDWriteCmd(0xf1);

	OLEDWriteCmd(0xda);	//--set com pins hardware configuration
	OLEDWriteCmd(0x12);

	OLEDWriteCmd(0xdb);	//--set vcomh
	OLEDWriteCmd(0x40);

	OLEDWriteCmd(0x8d);	//--set chare pump enable/disable
	OLEDWriteCmd(0x14);	//--set(0x10) disable
	OLEDSetMemoryAddressingMode(2);

	OLEDClear();
	
	//OLEDWriteCmd(0x10);	//--set(0x10) disable
	OLEDWriteCmd(0xaf);	//--turn on oled panel
}


void OLEDPutChar(unsigned char page,
					unsigned char col,
					unsigned char c,
					unsigned char display_type)
{
	int i = 0;
	/* è·å–å­—æ¨¡ */
	const unsigned char *dots = oled_asc2_8x16[c - ' '];

	/* å‘é€åˆ°OLED */
	OLEDSetPos(page, col);
	/* å‘å‡º8å­—èŠ‚æ•°æ® */
	switch(display_type)
	{
		case INVERT:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(~dots[i]);

		/* å†æ¬¡å‘é€8å­—èŠ‚æ•°æ® */
			OLEDSetPos(page+1, col);
			for(i=0;i<8;i++)
				OLEDWriteDat(~dots[i+8]);

			break;
		}
		case NORMAL:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(dots[i]);

		/* å†æ¬¡å‘é€8å­—èŠ‚æ•°æ® */
			OLEDSetPos(page+1, col);
			for(i=0;i<8;i++)
				OLEDWriteDat(dots[i+8]);

			break;
		}
		case CLEAR:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(0);

		/* å†æ¬¡å‘é€8å­—èŠ‚æ•°æ® */
			OLEDSetPos(page+1, col);
			for(i=0;i<8;i++)
				OLEDWriteDat(0);
		}
		default:break;
	}
}

/*
	page: 0-7
	col : 0-127
	str:×Ö·û´®
	alignment: ¶ÔÆë·½Ê½
*/
void OLEDPrint(unsigned char page,
				unsigned char col,
				unsigned char *str,
				unsigned char alignment,
				unsigned char display_type)
{
	/* res = 128*64 */
	/*
		¶ÔÆë·½Ê½
		1.¾Ó×ó Ä¬ÈÏ·½Ê½
		2.¾ÓÖĞ
		3.¾ÓÓÒ

		°´¶ÔÆë·½Ê½¼ÆËãcolumn
	*/
	unsigned char data len,i = 0;
	unsigned char data space_left;
	while(str[i])
	{
		i++;
	}
	len = i;
	space_left = 16-len;
	switch(alignment)
	{
		case LEFT:	//¾Ó×ó
		{	
		   	break;
		}
		case CENTER: //¾ÓÖĞ
		{
			col = (space_left/2.0)*8;
		  	break;
		}
		case RIGHT: //¾ÓÓÒ
		{
			col = space_left*8;
		   	break;
		}
		default: break;
	}
	for(i=0;i<len;i++)
	{
	  	OLEDPutChar(page, col, str[i], display_type);
		col += 8;
	}
}

void OLED_Set_Display(unsigned char val)
{

}

unsigned char SelectBitValue(int val)
{
	/* ÓàÊı×ªÊ®Áù½øÖÆº¯Êı */
    unsigned char temp = 0x00;
    unsigned char op = 0x01;
    unsigned char result;
    val = 7 - val;
    result = temp | (op << val);
    return result;
}


struct Position XY2PageCol(int x, int y)
{
   	/* xy×ø±êµã×ª»»ÎªpageºÍColum */
    struct Position pos;

    pos.page = 7 - y / 8;
    pos.col = x;
    pos.rem = y % 8;
    return pos;
}


void OLEDPutPixel(int x, int y)
{
	/* Ãèµãº¯Êıdraw range x:0~127 y:0~63 */
    /* ½«½ÓÊÕµ½µÄx,y×ø±ê×ª»»ÎªpageºÍcol */
    unsigned char dat;
    struct Position pos;

    pos = XY2PageCol(x, y); //×ø±ê×ª»»

    OLEDSetPos(pos.page, pos.col); //ÉèÖÃ»­±ÊÎ»ÖÃ
    dat = SelectBitValue(pos.rem); //»ñÈ¡Ò³ÓàÊı
    OLEDWriteDat(dat);             //ÏòoledÖĞĞ´Êı¾İ
}