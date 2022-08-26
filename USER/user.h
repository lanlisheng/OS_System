#ifndef _USER_H_
#define _USER_H_

//Ì½²âÆ÷Âë¶¨Òå
//ÃÅ´ÅÌ½²âÆ÷ 2262±àÂë
#define SENSOR_CODE_DOOR_OPEN 0x0A   //¿ªÃÅ
#define SENSOR_CODE_DOOR_CLOSE 0x0E  //¹ØÃÅ
#define SENSOR_CODE_DOOR_TAMPER 0x07 //·À²ð,Ô¤Áô
#define SENSOR_CODE_DOOR_LOWPWR 0x06 //µÍÑ¹

//Ò£¿ØÆ÷ EV1527±àÂë
#define SENSOR_CODE_REMOTE_ENARM 0x02
#define SENSOR_CODE_REMOTE_DISARM 0x01
#define SENSOR_CODE_REMOTE_HOMEARM 0x04
#define SENSOR_CODE_REMOTE_SOS 0x08

#define SENSOR_CODE_PIR 0x03
#define SENSOR_CODE_PIR_TAMPER 0x0B
#define SENSOR_CODE_PIR_LOWPWR 0x05

/* 2. Defines the location of the menu, mainly used to determine the timeout  */
typedef enum {
  DESKTOP_MENU_POS,
  STG_MENU_POS,
  STG_WIFI_MENU_POS,
  STG_SUB_2_MENU_POS,
  STG_SUB_3_MENU_POS,
  STG_SUB_4_MENU_POS,
} MENU_POS;

/* 3. General menu list */
typedef enum {
  GNL_MENU_DESKTOP, //×ÀÃæ
  GNL_MENU_SUM
} GENERAL_MENU_LIST;

/* 4. Menu related declaration area */
typedef enum {
  SCREEN_CMD_NULL,    // Useless command
  SCREEN_CMD_RESET,   // Reset the screen display
  SCREEN_CMD_RECOVER, // Restore the original display
  SCREEN_CMD_UPDATE   // Update the original display
} SCREEN_CMD;         // Refresh the screen

/* 5. Set the menu list ID */
typedef enum {
  STG_MENU_MAIN_SETTING,     // 0
  STG_MENU_LEARNING_SENSOR,  // 1
  STG_MENU_DTC_LIST,         // 2
  STG_MENU_WIFI,             // 3
  STG_MENU_MACHINE_INFO,     // 4
  STG_MENU_FACTORY_SETTINGS, // 5
  STG_MENU_SUM               // 6
} STG_MENU_LIST;

/* 6. DTC List->Zone xxx->Review */
typedef enum {
  STG_MENU_DL_ZX_REVIEW_MAIN,
  STG_MENU_DL_ZX_REVIEW,
  STG_MENU_DL_ZX_EDIT,
  STG_MENU_DL_ZX_DELETE,
  STG_MENU_DL_ZX_SUM,
} STG_MENU_DZ_ZX_LIST;

/* 1. Create a menu structure */
typedef struct MODE_MENU {
  unsigned char ID;            /* Menu unique ID number */
  MENU_POS menuPos;            /* The location of the current menu */
  unsigned char *pModeType;    /* Point to the current schema type */
  void (*action)(void);        /* Response function in current mode */
  SCREEN_CMD refreshScreenCmd; /* Refresh the screen display command */
  unsigned char reserved;      /* Reserved for easy parameter transfer */
  unsigned char keyVal;        /* Key value, 0xff indicates no key trigger */
  struct MODE_MENU *pLase;     /* Point to the previous option */
  struct MODE_MENU *pNext;     /* Point to the next option */
  struct MODE_MENU *pParent;   /* Point to the parent menu */
  struct MODE_MENU *pChild;    /* Points to the sub-menu */
} stu_mode_menu;

typedef struct SYSTEM_TIME {
  unsigned short year;
  unsigned char mon;
  unsigned char day;
  unsigned char week;
  unsigned char hour;
  unsigned char min;
  unsigned char sec;
} stu_system_time;

void UserInit(void);
void UserProc(void);

#endif