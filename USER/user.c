#include "user.h"
#include "OS_System.h"
#include "eeprom.h"
#include "key.h"
#include "led.h"
#include "main.h"
#include "para.h"
#include "rfd.h"
#include "stm32f7xx.h"
#include <stdio.h>

static void KeyEventHandle(KEY_VALUE_TYPEDEF keys);
static void RfdRcvHandle(unsigned char *pBuff);

Queue8 RFDRcvMsg; // RFD接收队列

void UserInit(void) {
  // unsigned char writeAry[66], readAry[66], i;

  hal_eepromInit();
  ParaInit();
  QueueEmpty(RFDRcvMsg);
  hal_KeyScanCBSRegister(KeyEventHandle);
  hal_RFCRcvCBSRegister(RfdRcvHandle);
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

static void RfdRcvHandle(unsigned char *pBuff) {
  unsigned char temp;
  temp = '#';
  QueueDataIn(RFDRcvMsg, &temp, 1);
  QueueDataIn(RFDRcvMsg, &pBuff[0], 3);
}