#include "user.h"
#include "eeprom.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "stm32f7xx.h"
#include <stdio.h>

static void KeyEventHandle(KEY_VALUE_TYPEDEF keys);

void UserInit(void) {
  unsigned char writeAry[66], readAry[66], i;

  hal_eepromInit();
  hal_KeyScanCBSRegister(KeyEventHandle);

  for (i = 0; i < 66; i++) {
    writeAry[i] = i + 1;
    readAry[i] = 0;
  }

  I2C_Read(0, readAry, 66);
  printf("eeprom is ok\r\nDATA IS:\r");
  for (uint8_t i = 0; i < 66; i++) {
    printf("%d\n");
  }
  printf("\r\n");
  if ((readAry[0] == 1) && (readAry[1] == 2) && (readAry[2] == 3)) {
    LedMsgInput(LED1, LED_BLINK4, 1);

  } else {
    // I2C_PageWrite(0, writeAry, 66);
  }
}

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