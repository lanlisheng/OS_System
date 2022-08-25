#ifndef _USER_H_
#define _USER_H_

/* 1. 创建一个菜单结构体 */
typedef struct MODE_MENU {
  unsigned char ID;            /* 菜单唯一ID号 */
  MENU_POS menuPos;            /* 当前菜单的位置信息 */
  unsigned char *pModeType;    /* 指向当前模式类型 */
  void (*action)(void);        /* 当前模式下的响应函数 */
  SCREEN_CMD refreshScreenCmd; /* 刷新屏显示指令 */
  unsigned char reserved;      /* 预留，方便参数传递 */
  unsigned char keyVal;        /* 按键值，0xff代表无按键触发 */
  struct MODE_MENU *pLase;     /* 指向上一个选项 */
  struct MODE_MENU *pNext;     /* 指向下一个选项 */
  struct MODE_MENU *pParent;   /* 指向父级菜单 */
  struct MODE_MENU *pChild;    /* 指向子级菜单 */
} stu_mode_menu;

void UserInit(void);
void UserProc(void);

#endif