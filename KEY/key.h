#ifndef _KEY_H
#define _KEY_H

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

/* 扫描按键的定时器Tick,以系统Tick(1ms)为单位，10 = 10ms */
#define KEY_SCANT_TICK 10 // 10ms

/* 按键消抖时间，以10ms为Tick,2 = 20ms */
#define KEY_SCANTIME 2 // 20ms
#endif