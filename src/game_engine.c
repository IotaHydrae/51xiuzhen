#include "oled.h"
#include "head.h"
#include "game_engine.h"
#include <intrins.h>

int strlen(unsigned char *p)
{
	int len=0;
	while(*p!='\0')
	{
		p++;
		len++;
	}
	return len;
}

/* 鏀诲嚮 */
/* 显示页面到oled */
void Show_Game_Menu(struct section sec)
{
	if(strlen(sec.line1.text)>0) 
	OLEDPrint(0,0,sec.line1.text, sec.line1.alignment,sec.line1.isSelected);
	if(strlen(sec.line2.text)>0) 
	OLEDPrint(2,0,sec.line2.text, sec.line2.alignment,sec.line2.isSelected);
	if(strlen(sec.line3.text)>0) 
	OLEDPrint(4,0,sec.line3.text, sec.line3.alignment,sec.line3.isSelected);
	if(strlen(sec.line4.text)>0) 
	OLEDPrint(6,0,sec.line4.text, sec.line4.alignment,sec.line4.isSelected);
}
void Hide_Game_Menu(struct section sec)
{
	//if(strlen(sec.line1.text)>0) 
	OLEDPrint(0,0,sec.line1.text, sec.line1.alignment,CLEAR);
	//if(strlen(sec.line1.text)>0)	
	OLEDPrint(2,0,sec.line2.text, sec.line2.alignment,CLEAR);
	//if(strlen(sec.line1.text)>0)
	OLEDPrint(4,0,sec.line3.text, sec.line3.alignment,CLEAR);
	//if(strlen(sec.line1.text)>0)
	OLEDPrint(6,0,sec.line4.text, sec.line4.alignment,CLEAR);	
}
void Change_Game_Menu()
{
	Delayms(1000);
}
/*struct position MakePosition(unsigned char page, unsigned char col)
{
	struct position pos;
	pos.page = page;
	pos.col = col;
	return pos;
}*/

struct option MakeOption(unsigned char which_in, 
							unsigned char *text_in, 
							unsigned char isSelected,
							unsigned char alignment)
{
	struct option opt;
	opt.which = which_in;
	opt.text = text_in;
	opt.isSelected = isSelected;
	opt.alignment = alignment;
	return opt;
}
/* 结构体构造函数 */
struct section MakeSection(
                            struct option line1,
							struct option line2,
							struct option line3,
							struct option line4)
{
	struct section sec;
	sec.line1= line1;
	sec.line2= line2;
	sec.line3= line3;
	sec.line4= line4;
	return sec;	
} 

struct entity MakeEntity(unsigned char damage,
							unsigned char defense,
							unsigned char health)
{
	struct entity object;
	object.damage = damage;
	object.defense = defense;
	object.health = health;
	return object;
}

unsigned char clacNextOption(struct section sec_in, unsigned char selected_line,unsigned char direction)
{
	unsigned char buf[4];
	unsigned char index=0;
	unsigned char i;
	unsigned char sum=0;
	struct ring_buffer
	{
	    unsigned char which_this;
		struct ring_buffer *next;
	};
	struct ring_buffer pos1;
	struct ring_buffer pos2;
	struct ring_buffer pos3;
	struct ring_buffer pos4;
	struct ring_buffer *pos_now;

	if(strlen(sec_in.line1.text)>0)
	{
		buf[index]=	sec_in.line1.which;
		index++;
	}
	if(strlen(sec_in.line2.text)>0)
	{
		buf[index]=	sec_in.line2.which;
		index++;
	}
	if(strlen(sec_in.line3.text)>0)
	{
		buf[index]=	sec_in.line3.which;
		index++;
	}
	if(strlen(sec_in.line4.text)>0)
	{
		buf[index]=	sec_in.line4.which;
		index++;
	}
	//get option buf buf[inedx] index 1~4
	for(i=0;i<index;i++)
	{
		sum+=buf[i];
	}
	
	switch(index)
	{
		case 1:
		{
		   return selected_line;
		}
		case 2:
		{ 
		   return (sum-selected_line);
		}
		case 3:
		{
		   pos1.which_this = buf[0];
		   pos2.which_this = buf[1];
		   pos3.which_this = buf[2];
		   if(direction=='L')
		   {
			   pos1.next=&pos3;
			   pos2.next=&pos1;
			   pos3.next=&pos2;
		   }else
		   {
			   pos1.next=&pos2;
			   pos2.next=&pos3;
			   pos3.next=&pos1;		   
			}
		   if(selected_line==pos1.which_this) pos_now=pos1.next;
		   if(selected_line==pos2.which_this) pos_now=pos2.next;
		   if(selected_line==pos3.which_this) pos_now=pos3.next;
		   return pos_now->which_this;

		}
		case 4:
		{
		   pos1.which_this = buf[0];
		   pos2.which_this = buf[1];
		   pos3.which_this = buf[2];
		   pos4.which_this = buf[3];
		   if(direction=='L')
		   {
		   pos1.next=&pos4;
		   pos2.next=&pos1;
		   pos3.next=&pos2;
		   pos4.next=&pos3;
		   }else
		   {
		   pos1.next=&pos2;
		   pos2.next=&pos3;
		   pos3.next=&pos4;
		   pos4.next=&pos1;	   
			}
		   if(selected_line==pos1.which_this) pos_now=pos1.next;
		   if(selected_line==pos2.which_this) pos_now=pos2.next;
		   if(selected_line==pos3.which_this) pos_now=pos3.next;	
		   if(selected_line==pos4.which_this) pos_now=pos4.next;				
		   return pos_now->which_this;
		}
		default: break;
	}
			 	
}

struct option findWitchIsSelected(struct section current_section)
{
	if((current_section.line1.isSelected==(unsigned char)-1) && (strlen(current_section.line1.text)>0))
	{
		 return  current_section.line1;
	}
	if(current_section.line2.isSelected==(unsigned char)-1  && (strlen(current_section.line2.text)>0))
	{
		return  current_section.line2;
	}
	if(current_section.line3.isSelected==(unsigned char)-1  && (strlen(current_section.line3.text)>0))
	{
		return  current_section.line3;	
	}
	if((current_section.line4.isSelected==(unsigned char)-1)  && (strlen(current_section.line4.text)>0))
	{
		return  current_section.line4;	
	}
}

unsigned char isAlive(struct entity *entity_in)
{
    if(entity_in->health <=0)
    {
        return DIED;
    }
    else
    {
        return ALIVE;
    }
}
void Delayms(unsigned int n) //@11.0592MHz
{
    unsigned char i, j;
    unsigned int m;

    for (m = 0; m < n; m++)
    {
        _nop_();
        i = 2;
        j = 199;
        do
        {
            while (--j)
                ;
        } while (--i);
    }
}

void DelayS(unsigned char n) //@11.0592MHz
{
    unsigned char i, j, k;
    unsigned char m;
    for (m = 0; m < n; m++)
    {
        _nop_();
        i = 8;
        j = 1;
        k = 243;
        do
        {
            do
            {
                while (--k)
                    ;
            } while (--j);
        } while (--i);
    }
}