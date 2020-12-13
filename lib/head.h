#ifndef __HEAD_H__
#define __HEAD_H__

#define NULL ((void*)0)

#define MAX(a,b) (a>b)?a:b
#define MIN(a,b) (a<b)?a:b
/* 实体状态 */
#define ALIVE 1
#define DIED -1
/* 执行状态 */
#define SUCCESS 1
#define FAILED -1
/* 显示方式 */
#define NORMAL 1
#define INVERT -1
#define CLEAR 0
/* 对齐方式 */
#define LEFT -1
#define CENTER 0
#define RIGHT 1
/* 开启or关闭 */
#define ON 1
#define OFF -1
/* yes or no */
#define YES -1
#define NO 1
#define EMPTY -1
/* 中断号 */

#define INT_INT0 0
#define INT_TIMER0 1
#define INT_INT1 2
#define INT_TIMER1 3
#define INT_UART 4
#define INT_TIMER2 5
#define INT_INT2 6
#define INT_INT3 7

#endif