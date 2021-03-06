#include <reg52.h>
#include <intrins.h>
#include "gpio_spi.h"


/* 使用GPIO模拟SPI */
static void SPI_GPIO_Init(void)
{
	/* init pin here */
	RES = 0;
	_nop_();
	_nop_();
	_nop_();
	RES = 1;
}

static void SPI_Set_CLK(char val)
{
	if(val)
		SCK = 1;
	else
		SCK = 0;
}

static void SPI_Set_DO(char val)
{
	if(val)
		SDA = 1;
	else
		SDA = 0;
		
}

void SPISendByte(unsigned char val)
{
	int i;
	for(i=0;i<8;i++)
	{
		SPI_Set_CLK(0);
		SPI_Set_DO((val & 0x80)>>7);
		_nop_();
		val <<= 1;
		SPI_Set_CLK(1);
		_nop_();
		SPI_Set_CLK(0);
		
	}
}

void SPIInit(void)
{
	/* 初始化引脚 */
	SPI_GPIO_Init();
}
