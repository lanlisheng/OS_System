#include "key.h"
#include "gpio.h"
#include "main.h"

KeyEvent_CallBack_t KeyScanCBS;

static unsigned char hal_getKey1Sta(void);
static unsigned char hal_getKey2Sta(void);
static unsigned char hal_getKey3Sta(void);

unsigned char (*getKeyState[KEY_SUM])() = {hal_getKey1Sta, hal_getKey2Sta,
                                           hal_getKey3Sta};

unsigned char KeyStep[KEY_SUM];            //按键检测流程
unsigned short KeyScanTime[KEY_SUM];       //按键去抖延时
unsigned short KeyPressLongTimer[KEY_SUM]; //按键长按延时
unsigned short KeyContPressTimer[KEY_SUM]; //按键连续长按延时

void hal_KeyInit(void) {
  unsigned char i = 0;

  KeyScanCBS = 0;

  for (i = 0; i < KEY_SUM; i++) {
    KeyStep[i] = KEY_STEP_WAIT;
    KeyScanTime[i] = KEY_SCANTIME;
    KeyPressLongTimer[i] = KEY_PRESS_LONG_TIME;
    KeyContPressTimer[i] = KEY_PRESS_CONTINUE_TIME;
  }
}

void hal_KeyScanCBSRegister(KeyEvent_CallBack_t pCBS) {
  if (KeyScanCBS == 0) {
    KeyScanCBS = pCBS;
  }
}

void hal_KeyProc(void) {
  unsigned char keys = 0;
  unsigned char i, KeyState[KEY_SUM];
  for (i = 0; i < KEY_SUM; i++) {
    keys = 0;

    KeyState[i] = getKeyState[i]();
    switch (KeyStep[i]) {
    case KEY_STEP_WAIT: //等待按键状态
      if (KeyState[i]) {
        KeyStep[i] = KEY_STEP_CLICK;
      }
      break;

    case KEY_STEP_CLICK: //按键单击按下
      if (KeyState[i]) {
        if (!(--KeyScanTime[i])) {
          KeyScanTime[i] = KEY_SCANTIME;
          KeyStep[i] = KEY_STEP_LONG_PRESS;
          keys = (i * 5) + 1; //记录按键ID号
        }
      } else {
        KeyScanTime[i] = KEY_SCANTIME;
        KeyStep[i] = KEY_STEP_WAIT;
      }
      break;

    case KEY_STEP_LONG_PRESS: //按键长按
      if (KeyState[i]) {
        if (!(--KeyPressLongTimer[i])) {
          KeyPressLongTimer[i] = KEY_PRESS_LONG_TIME;
          KeyStep[i] = KEY_SYEP_CONTINUOUS_PRESS;
          keys = (i * 5) + 3; //长按确认ID号
        }
      } else {
        KeyPressLongTimer[i] = KEY_PRESS_LONG_TIME;
        KeyStep[i] = KEY_STEP_WAIT;
        keys = (i * 5) + 2;
      }
      break;

    case KEY_SYEP_CONTINUOUS_PRESS: //持续长按
      if (KeyState[i]) {
        if (!(--KeyContPressTimer[i])) {
          KeyContPressTimer[i] = KEY_PRESS_CONTINUE_TIME;
          keys = (i * 5) + 4;
        }
      } else {
        KeyContPressTimer[i] = KEY_PRESS_CONTINUE_TIME;
        KeyStep[i] = KEY_STEP_WAIT;
        keys = (i * 5) + 5;
      }
      break;
    }
    if (keys) {
      if (KeyScanCBS) {
        KeyScanCBS((KEY_VALUE_TYPEDEF)keys);
      }
    }
  }
}

static unsigned char hal_getKey1Sta(void) { return 0; }
static unsigned char hal_getKey2Sta(void) { return 0; }
static unsigned char hal_getKey3Sta(void) { return 0; }
