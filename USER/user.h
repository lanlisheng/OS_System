#ifndef _USER_H_
#define _USER_H_

/* 1. ����һ���˵��ṹ�� */
typedef struct MODE_MENU {
  unsigned char ID;            /* �˵�ΨһID�� */
  MENU_POS menuPos;            /* ��ǰ�˵���λ����Ϣ */
  unsigned char *pModeType;    /* ָ��ǰģʽ���� */
  void (*action)(void);        /* ��ǰģʽ�µ���Ӧ���� */
  SCREEN_CMD refreshScreenCmd; /* ˢ������ʾָ�� */
  unsigned char reserved;      /* Ԥ��������������� */
  unsigned char keyVal;        /* ����ֵ��0xff�����ް������� */
  struct MODE_MENU *pLase;     /* ָ����һ��ѡ�� */
  struct MODE_MENU *pNext;     /* ָ����һ��ѡ�� */
  struct MODE_MENU *pParent;   /* ָ�򸸼��˵� */
  struct MODE_MENU *pChild;    /* ָ���Ӽ��˵� */
} stu_mode_menu;

void UserInit(void);
void UserProc(void);

#endif