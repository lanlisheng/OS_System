#ifndef _KEY_H
#define _KEY_H

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

/* ɨ�谴���Ķ�ʱ��Tick,��ϵͳTick(1ms)Ϊ��λ��10 = 10ms */
#define KEY_SCANT_TICK 10 // 10ms

/* ��������ʱ�䣬��10msΪTick,2 = 20ms */
#define KEY_SCANTIME 2 // 20ms
#endif