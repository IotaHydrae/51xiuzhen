#ifndef __GAME_ENGINE_H__
#define __GAME_ENGINE_H__


struct option
{
    unsigned char which;
    unsigned char *text;     //option name
	unsigned char isSelected;
    unsigned char alignment;
	unsigned char *next;
};

struct section
{
    struct option line1;
    struct option line2;
    struct option line3;
    struct option line4;
	unsigned char *name;
    struct section *preSection;
};

/*
    实体
    可以是玩家敌人npc等等
*/
struct entity
{
    /* 常规属性 */
    unsigned char damage;
    unsigned char defense;
    unsigned char health;
};

int strlen(unsigned char *p);
void Show_Game_Menu(struct section sec);
void Hide_Game_Menu(struct section sec);

struct position MakePosition(unsigned char page, unsigned char col);
struct option MakeOption(unsigned char which_in, 
							unsigned char *text_in, 
							unsigned char isSelected,
							unsigned char alignment);

struct section MakeSection(
                            struct option line1,
							struct option line2,
							struct option line3,
							struct option line4);

struct entity MakeEntity(unsigned char damage,
                         unsigned char defense,
                         unsigned char health);
struct option findWitchIsSelected(struct section current_section);
unsigned char clacNextOption(struct section sec_in, unsigned char selected_line,unsigned char direction);
void Change_Game_Menu();
void Delayms(unsigned int n);
void DelayS(unsigned char n);
#endif