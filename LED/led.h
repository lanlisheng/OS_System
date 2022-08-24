#ifndef _LED_H_
#define _LED_H_

#include "main.h"

#define LED_EFFECT_END 0xFFFE
#define LED_EFFECT_AGN 0xFFFF

typedef enum {
  LED1, // 0
  BUZ,  // 1
  LED_SUM
} LED_TYPEDEF;

typedef enum {
  LED_DARK,        //熄灭特效
  LED_LIGHT,       //常亮特性
  LED_LIGHT_100MS, // 100ms闪烁一次
  LED_BLINK1,
  LED_BLINK2,
  LED_BLINK3,
  LED_BLINK4
} LED_EFFECT_TYPEDEF;

void hal_LedProc(void);
void hal_LedInit(void);
void LedMsgInput(unsigned char type, LED_EFFECT_TYPEDEF cmd, unsigned char clr);
#endif
