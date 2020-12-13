#include <reg52.h>
#include <intrins.h>
#include "gpio_spi.h"
#include "oledfont.h"
#include "head.h"

static unsigned char index_line = 0;

struct Position
{
    /* 
        page��ҳ
        col: ��
        rem��ҳ����
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
	/* 设置页地址 */
	OLEDWriteCmd(0xB0+page);

	/* 设置列地址 */
	OLEDWriteCmd(0x00|(col&0x0f));	      /* 低地址 */
	OLEDWriteCmd(0x10|(col >> 4));  /* 高地址 */
}

/* OLED���ȿ��� 256�� */
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
	/* 获取字模 */
	const unsigned char *dots = oled_asc2_8x16[c - ' '];

	/* 发送到OLED */
	OLEDSetPos(page, col);
	/* 发出8字节数据 */
	switch(display_type)
	{
		case INVERT:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(~dots[i]);

		/* 再次发送8字节数据 */
			OLEDSetPos(page+1, col);
			for(i=0;i<8;i++)
				OLEDWriteDat(~dots[i+8]);

			break;
		}
		case NORMAL:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(dots[i]);

		/* 再次发送8字节数据 */
			OLEDSetPos(page+1, col);
			for(i=0;i<8;i++)
				OLEDWriteDat(dots[i+8]);

			break;
		}
		case CLEAR:
		{
			for(i=0;i<8;i++)
				OLEDWriteDat(0);

		/* 再次发送8字节数据 */
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
	str:�ַ���
	alignment: ���뷽ʽ
*/
void OLEDPrint(unsigned char page,
				unsigned char col,
				unsigned char *str,
				unsigned char alignment,
				unsigned char display_type)
{
	/* res = 128*64 */
	/*
		���뷽ʽ
		1.���� Ĭ�Ϸ�ʽ
		2.����
		3.����

		�����뷽ʽ����column
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
		case LEFT:	//����
		{	
		   	break;
		}
		case CENTER: //����
		{
			col = (space_left/2.0)*8;
		  	break;
		}
		case RIGHT: //����
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
	/* ����תʮ�����ƺ��� */
    unsigned char temp = 0x00;
    unsigned char op = 0x01;
    unsigned char result;
    val = 7 - val;
    result = temp | (op << val);
    return result;
}


struct Position XY2PageCol(int x, int y)
{
   	/* xy�����ת��Ϊpage��Colum */
    struct Position pos;

    pos.page = 7 - y / 8;
    pos.col = x;
    pos.rem = y % 8;
    return pos;
}


void OLEDPutPixel(int x, int y)
{
	/* ��㺯��draw range x:0~127 y:0~63 */
    /* �����յ���x,y����ת��Ϊpage��col */
    unsigned char dat;
    struct Position pos;

    pos = XY2PageCol(x, y); //����ת��

    OLEDSetPos(pos.page, pos.col); //���û���λ��
    dat = SelectBitValue(pos.rem); //��ȡҳ����
    OLEDWriteDat(dat);             //��oled��д����
}