#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

/* 按键功能定义 */
typedef enum {
  KEY_S1, //上
  KEY_S2, //下
  KEY_S3, //左
  KEY_S4, //右
  KEY_SUM
} KEY_TYPEDEF;

/* 定义一个按键检测的流程 */
typedef enum {
  KEY_STEP_WAIT,            //等待按键
  KEY_STEP_CLICK,           //按键按下
  KEY_STEP_LONG_PRESS,      //长按
  KEY_SYEP_CONTINUOUS_PRESS //持续长按
} KEY_STEP_TYPEDEF;

/* 定义一个按键检测的状态值 */
typedef enum {
  KEY_IDLE,                  //按键空闲
  KEY_CLICK,                 //单击确认
  KEY_CLICK_RELEASE,         //单击释放
  KEY_LONG_PRESS,            //长按确认
  KEY_LONG_PRESS_CONTINUOUS, //长按持续
  KEY_LONG_PRESS_RELEASE     //长按释放
} KEY_EVENT_TYPEDEF;

typedef enum {
  KEY_IDLE_VAL,               // 0
  KEY1_CLICK,                 // 1
  KEY1_CLICK_RELEASE,         // 2
  KEY1_LONG_PRESS,            // 3
  KEY1_LONG_PRESS_CONTINUOUS, // 4
  KEY1_LONG_PRESS_RELEASE,    // 5

  KEY2_CLICK, // 6
  KEY2_CLICK_RELEASE,
  KEY2_LONG_PRESS,
  KEY2_LONG_PRESS_CONTINUOUS,
  KEY2_LONG_PRESS_RELEASE,

  KEY3_CLICK, // 11
  KEY3_CLICK_RELEASE,
  KEY3_LONG_PRESS,
  KEY3_LONG_PRESS_CONTINUOUS,
  KEY3_LONG_PRESS_RELEASE,

  KEY4_CLICK, // 16
  KEY4_CLICK_RELEASE,
  KEY4_LONG_PRESS,
  KEY4_LONG_PRESS_CONTINUOUS,
  KEY4_LONG_PRESS_RELEASE,

  KEY5_CLICK, // 21
  KEY5_CLICK_RELEASE,
  KEY5_LONG_PRESS,
  KEY5_LONG_PRESS_CONTINUOUS,
  KEY5_LONG_PRESS_RELEASE,

  KEY6_CLICK, // 26
  KEY6_CLICK_RELEASE,
  KEY6_LONG_PRESS,
  KEY6_LONG_PRESS_CONTINUOUS,
  KEY6_LONG_PRESS_RELEASE,

} KEY_VALUE_TYPEDEF;

/* 定义一个按键id回调函数 */
typedef void (*KeyEvent_CallBack_t)(KEY_VALUE_TYPEDEF keys);

/* 扫描按键的定时器Tick,以系统Tick(1ms)为单位，10 = 10ms */
#define KEY_SCANT_TICK 10 // 10ms

/* 按键消抖时间，以10ms为Tick,2 = 20ms */
#define KEY_SCANTIME 2 // 20ms

/* 连续长按时间 */
#define KEY_PRESS_LONG_TIME 200 // 2S

/* 持续长按间隔时间 */
#define KEY_PRESS_CONTINUE_TIME 15 // 150MS

void hal_KeyInit(void);
void hal_KeyProc(void);
void hal_KeyScanCBSRegister(KeyEvent_CallBack_t pCBS);
#endif
