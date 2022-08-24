#include "user.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "stm32f7xx.h"

static void KeyEventHandle(KEY_VALUE_TYPEDEF keys);

void UserInit(void) { hal_KeyScanCBSRegister(KeyEventHandle); }

void UserProc(void) {}

/* 按键回调函数，驱动层回调处理函数 */
static void KeyEventHandle(KEY_VALUE_TYPEDEF keys) {
  if ((keys == KEY1_CLICK) || (keys == KEY2_CLICK) || (keys == KEY3_CLICK)) {
    LedMsgInput(LED1, LED_LIGHT, 1);
    //执行命令
  } else if ((keys == KEY1_CLICK_RELEASE) || (keys == KEY2_CLICK_RELEASE) ||
             (keys == KEY3_CLICK_RELEASE)) {
    LedMsgInput(LED1, LED_BLINK4, 1);
    //执行命令
  } else if ((keys == KEY1_LONG_PRESS) || (keys == KEY2_LONG_PRESS) ||
             (keys == KEY3_LONG_PRESS)) {
    LedMsgInput(LED1, LED_DARK, 1);
  }
}