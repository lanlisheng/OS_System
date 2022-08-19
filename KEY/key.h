#ifndef _KEY_H_
#define _KEY_H_

#include "main.h"

/* �������ܶ��� */
typedef enum {
  KEY_S1, //��
  KEY_S2, //��
  KEY_S3, //��
  KEY_S4, //��
  KEY_SUM
} KEY_TYPEDEF;

/* ����һ�������������� */
typedef enum {
  KEY_STEP_WAIT,            //�ȴ�����
  KEY_STEP_CLICK,           //��������
  KEY_STEP_LONG_PRESS,      //����
  KEY_SYEP_CONTINUOUS_PRESS //��������
} KEY_STEP_TYPEDEF;

/* ����һ����������״ֵ̬ */
typedef enum {
  KEY_IDLE,                  //��������
  KEY_CLICK,                 //����ȷ��
  KEY_CLICK_RELEASE,         //�����ͷ�
  KEY_LONG_PRESS,            //����ȷ��
  KEY_LONG_PRESS_CONTINUOUS, //��������
  KEY_LONG_PRESS_RELEASE     //�����ͷ�
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

/* ����һ������id�ص����� */
typedef void (*KeyEvent_CallBack_t)(KEY_VALUE_TYPEDEF keys);

/* ɨ�谴���Ķ�ʱ��Tick,��ϵͳTick(1ms)Ϊ��λ��10 = 10ms */
#define KEY_SCANT_TICK 10 // 10ms

/* ��������ʱ�䣬��10msΪTick,2 = 20ms */
#define KEY_SCANTIME 2 // 20ms

/* ��������ʱ�� */
#define KEY_PRESS_LONG_TIME 200 // 2S

/* �����������ʱ�� */
#define KEY_PRESS_CONTINUE_TIME 15 // 150MS

void hal_KeyInit(void);
void hal_KeyProc(void);
void hal_KeyScanCBSRegister(KeyEvent_CallBack_t pCBS);
#endif
