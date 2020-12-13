#ifndef _GPIO_SPI_H
#define _GPIO_SPI_H

/*#if defined _LT158_K51
sbit SCK = P2^0;
sbit SDA = P2^1;
sbit RES = P2^2;
sbit DC = P2^3;
sbit CS = P2^4;
#elif defined _LT158_tiny
sbit SCK = P1^7;
sbit SDA = P3^2;
sbit RES = P3^3;
sbit DC = P3^4;
sbit CS = P3^5; 
#else
sbit SCK = P2^0;
sbit SDA = P2^1;
sbit RES = P2^2;
sbit DC = P2^3;
sbit CS = P2^4;*/
sbit SCK = P1^7;
sbit SDA = P3^2;
sbit RES = P3^3;
sbit DC = P3^4;
sbit CS = P3^5; 



void SPIInit(void);
void SPISendByte(unsigned char val);

#endif