#ifndef __WDT_H__
#define __WDT_H__
/* 振荡器频率 */
//#define OSCILLATOR_FREQUENCY 12000000
#define OSCILLATOR_FREQUENCY 11059200

/* 看门狗相关定义 */
//-----------------------------
#define WDT_CONTR 0xe1
#define PRE_SCALE 64

//看门狗及其溢出时间 = (12 * PRESCALE * 32768)/时钟
#if OSCILLATOR_FREQUENCY == 12000000
#if PRE_SCALE == 32
#define PRE_SCALE_WORD = 0b00111100     //启动看门狗，清零，预分频数32 溢出时间 1.0485 S
#elif PRE_SCALE == 64
#define PRE_SCALE_WORD = 0b00111101     //启动看门狗，清零，预分频数64 溢出时间 2.0971 S
#elif PRE_SCALE == 128
#define PRE_SCALE_WORD = 0b00111110     //启动看门狗，清零，预分频数128 溢出时间 4.1943 S
#elif PRE_SCALE == 256
#define PRE_SCALE_WORD = 0b00111111     //启动看门狗，清零，预分频数256 溢出时间 8.3886 S
#endif

#elif OSCILLATOR_FREQUENCY == 11059200
#if PRE_SCALE == 32
#define PRE_SCALE_WORD = 0b00111100     //启动看门狗，清零，预分频数32 溢出时间 1.1377 S
#elif PRE_SCALE == 64
#define PRE_SCALE_WORD = 0b00111101     //启动看门狗，清零，预分频数64 溢出时间 2.2755 S
#elif PRE_SCALE == 128 
#define PRE_SCALE_WORD = 0b00111110     //启动看门狗，清零，预分频数128 溢出时间 4.5511 S
#elif PRE_SCALE == 256
#define PRE_SCALE_WORD = 0b00111111     //启动看门狗，清零，预分频数256 溢出时间 9.1022 S
#endif

#endif
//-----------------------------
#endif