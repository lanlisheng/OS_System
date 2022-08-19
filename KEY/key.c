#include "key.h"
#include "gpio.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>

KeyEvent_CallBack_t KeyScanCBS;

static unsigned char hal_getKey1Sta(void);
static unsigned char hal_getKey2Sta(void);
static unsigned char hal_getKey3Sta(void);

unsigned char (*getKeysState[KEY_SUM])() = {hal_getKey1Sta, hal_getKey2Sta,
                                            hal_getKey3Sta};

unsigned char KeyStep[KEY_SUM];            //�����������
unsigned short KeyScanTime[KEY_SUM];       //����ȥ����ʱ
unsigned short KeyPressLongTimer[KEY_SUM]; //����������ʱ
unsigned short KeyContPressTimer[KEY_SUM]; //��������������ʱ

void hal_KeyInit(void) {
  unsigned char i = 0;

  MX_GPIO_Init();

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

unsigned char KEY_Scan_Msg = KEY_IDLE_VAL;
unsigned char keys = 0;

void hal_KeyProc(void) {

  unsigned char i, KeyState[KEY_SUM];

  for (i = 0; i < KEY_SUM; i++) {
    keys = 0;

    KeyState[i] = getKeysState[i]();
    switch (KeyStep[i]) {
    case KEY_STEP_WAIT: //�ȴ�����
      if (KeyState[i]) {
        KeyStep[i] = KEY_STEP_CLICK;
      }
      break;
    case KEY_STEP_CLICK: //������������
      if (KeyState[i]) {
        if (!(--KeyScanTime[i])) {
          KeyScanTime[i] = KEY_SCANTIME;
          KeyStep[i] = KEY_STEP_LONG_PRESS;
          // keys = i+1;
          // //��¼����ID�� state = KEY_CLICK;
          // //������������
          keys = (i * 5) + 1;
        }
      } else {
        KeyScanTime[i] = KEY_SCANTIME;
        KeyStep[i] = KEY_STEP_WAIT;
      }
      break;
    case KEY_STEP_LONG_PRESS: //��������
      if (KeyState[i]) {
        if (!(--KeyPressLongTimer[i])) {
          KeyPressLongTimer[i] = KEY_PRESS_LONG_TIME;
          KeyStep[i] = KEY_STEP_CONTINUOUS_PRESS;
          keys = (i * 5) + 3; //����ȷ��
        }
      } else {
        KeyPressLongTimer[i] = KEY_PRESS_LONG_TIME;
        KeyStep[i] = KEY_STEP_WAIT;
        keys = (i * 5) + 2; //�����ͷ�
      }
      break;
    case KEY_STEP_CONTINUOUS_PRESS:
      if (KeyState[i]) {
        if (!(--KeyContPressTimer[i])) {
          KeyContPressTimer[i] = KEY_PRESS_CONTINUE_TIME;
          keys = (i * 5) + 4; //��������
        }
      } else {
        KeyStep[i] = KEY_STEP_WAIT;
        KeyContPressTimer[i] = KEY_PRESS_CONTINUE_TIME;
        keys = (i * 5) + 5; //�����ͷ�
      }

      break;
    }
    if (keys) {
      printf("keys is :%d\r\n", keys);
      KEY_Scan_Msg = keys;
    }
  }
}

static unsigned char hal_getKey1Sta(void) {
  return (!HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin));
}
static unsigned char hal_getKey2Sta(void) {
  return (!HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin));
}
static unsigned char hal_getKey3Sta(void) {
  return (!HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin));
}
