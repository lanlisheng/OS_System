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
static void menuInit(void);

/* Desktop interface handler function */
static void gnlMenu_DesktopCBS(void);

/* Set the interface handler */
static void stgMenu_FactorySettingsCBS(void);
static void stgMenu_MachineInfoCBS(void);
static void stgMenu_WifiCBS(void);
static void stgMenu_DTCListCBS(void);
static void stgMenu_LearnSensorCBS(void);
static void stgMenu_MainMenuCBS(void);

Queue8 RFDRcvMsg; // RFD接收队列

stu_mode_menu *pModeMenu;      // Current System Menu
stu_system_time stuSystemtime; // System time

// Initialize the desktop menu
stu_mode_menu generalModeMenu[GNL_MENU_SUM] = {
    {GNL_MENU_DESKTOP, DESKTOP_MENU_POS, "Desktop", gnlMenu_DesktopCBS,
     SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0},
};

// Initialize the setup menu
stu_mode_menu settingModeMenu[STG_MENU_SUM] = {
    {STG_MENU_MAIN_SETTING, STG_MENU_POS, "Main Menu", stgMenu_MainMenuCBS,
     SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0}, // Set the main page
    {STG_MENU_LEARNING_SENSOR, STG_SUB_2_MENU_POS, "1. Learning Dtc",
     stgMenu_LearnSensorCBS, SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0,
     0}, // Probe learning
    {STG_MENU_DTC_LIST, STG_SUB_2_MENU_POS, "2. Dtc List", stgMenu_DTCListCBS,
     SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0}, // to play
    {STG_MENU_WIFI, STG_WIFI_MENU_POS, "3. WiFi", stgMenu_WifiCBS,
     SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0},
    {STG_MENU_MACHINE_INFO, STG_SUB_2_MENU_POS, "4. Mac Info",
     stgMenu_MachineInfoCBS, SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0},
    {STG_MENU_FACTORY_SETTINGS, STG_SUB_2_MENU_POS, "5. Default Setting",
     stgMenu_FactorySettingsCBS, SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0},
};

// 3.DTC list->Zone xxx->Review
stu_mode_menu DL_ZX_Review[STG_MENU_SUM] = {
    {STG_MENU_DL_ZX_REVIEW_MAIN, STG_SUB_2_MENU_POS, "View",
     stgMenu_dl_ReviewMainCBS, SCREEN_CMD_RESET, 0, 0xFF, 0, 0, 0, 0},
    {},
    {},
    {}};

void UserInit(void) {
  // unsigned char writeAry[66], readAry[66], i;

  hal_eepromInit();
  ParaInit();
  menuInit();
  QueueEmpty(RFDRcvMsg);
  hal_KeyScanCBSRegister(KeyEventHandle);
  hal_RFCRcvCBSRegister(RfdRcvHandle);

  stuSystemtime.year = 2022;
  stuSystemtime.mon = 8;
  stuSystemtime.day = 25;
  stuSystemtime.hour = 23;
  stuSystemtime.min = 37;
  stuSystemtime.week = 4;
}

void UserProc(void) { pModeMenu->action(); }

static void menuInit(void) {
  unsigned char i;
  /* Main menu initialization, the menu list into a linked list form, easy to
  call */
  settingModeMenu[1].pLase = &settingModeMenu[STG_MENU_SUM - 1];
  settingModeMenu[1].pNext = &settingModeMenu[2];
  settingModeMenu[1].pParent = &settingModeMenu[STG_MENU_MAIN_SETTING];

  for (i = 0; i < STG_MENU_SUM - 1; i++) {
    settingModeMenu[i].pLase = &settingModeMenu[i - 1];
    settingModeMenu[i].pNext = &settingModeMenu[i + 1];
    settingModeMenu[i].pParent = &settingModeMenu[STG_MENU_MAIN_SETTING];
  }

  settingModeMenu[STG_MENU_SUM - 1].pLase = &settingModeMenu[i - 1];
  settingModeMenu[STG_MENU_SUM - 1].pNext = &settingModeMenu[1];
  settingModeMenu[STG_MENU_SUM - 1].pParent =
      &settingModeMenu[STG_MENU_MAIN_SETTING];

  pModeMenu = &generalModeMenu[GNL_MENU_DESKTOP]; // Set the power-on display
  // menu to desktop display
  pModeMenu->refreshScreenCmd =
      SCREEN_CMD_RESET; // Update the refresh interface flag. After entering the
                        // interface, refresh the entire UI
}

static void gnlMenu_DesktopCBS(void) {
  unsigned char keys;
  if (pModeMenu->refreshScreenCmd == SCREEN_CMD_RESET) {
    pModeMenu->refreshScreenCmd = SCREEN_CMD_NULL;
    pModeMenu->keyVal = 0xFF;
    QueueEmpty(RFDRcvMsg);
  }

  if (pModeMenu->keyVal != 0xff) {
    keys = pModeMenu->keyVal;
    pModeMenu->keyVal = 0xFF; // Restore the menu key value
    switch (keys) {
    case KEY6_LONG_PRESS:
      pModeMenu = &settingModeMenu[0];
      pModeMenu->refreshScreenCmd = SCREEN_CMD_RESET;
      break;
    }
  }
}

/* Set the main menu */
static void stgMenu_MainMenuCBS(void) {
  unsigned char keys;
  unsigned char i;
  unsigned char ClrScreenFlag;
  static stu_mode_menu *pMenu;  /* Use to save the currently selected menu */
  static stu_mode_menu *bpMenu; /* Used to back up the last menu options, mainly
                                   used to brush the screen judgment */
  static unsigned char stgMainMenuSelectedPos =
      0; /* Used to record the location of the currently selected menu */
  static stu_mode_menu *MHead,
      *MTail; /* These two structure Pointers are used to flip pages when
                 switching menus up and down */
  if (pModeMenu->refreshScreenCmd == SCREEN_CMD_RESET) {
    pModeMenu->refreshScreenCmd = SCREEN_CMD_NULL;
    pMenu = &settingModeMenu[0];
    pMenu = &settingModeMenu[1];
    MHead = pMenu;     /* Records the first item in the current display menu */
    MTail = MHead + 3; /* Record the last item in the current display menu,
                          showing 4 lines per page */
    bpMenu = 0;
    ClrScreenFlag = 1;
    stgMainMenuSelectedPos = 1;
    keys = 0xFF;
  }

  if (pModeMenu->keyVal != 0xFF) {
    keys = pModeMenu->keyVal;
    pModeMenu->keyVal = 0xFF; /* Restore the menu key value */
    switch (keys) {
    case KEY1_CLICK: /* on */
      if (stgMainMenuSelectedPos == 1) {
        MHead = MHead->pLase;
        pMenu = pMenu->pLase;
        MTail = MTail->pLase;
        stgMainMenuSelectedPos = 1;
        ClrScreenFlag = 1;
      } else {
        pMenu = pMenu->pLase;
        stgMainMenuSelectedPos--;
      }
      break;
    case KEY2_CLICK: /* Down */
      if (stgMainMenuSelectedPos == 4) {
        MHead = MHead->pNext;
        pMenu = pMenu->pNext;
        MTail = MTail->pNext;
        stgMainMenuSelectedPos = 4;
        ClrScreenFlag = 1;
      } else {
        pMenu = pMenu->pNext; /* Toggle the next option */
        stgMainMenuSelectedPos++;
      }
      break;
    case KEY5_CLICK_RELEASE: /* Cancel */
      pModeMenu = &generalModeMenu[GNL_MENU_DESKTOP];
      pModeMenu->refreshScreenCmd = SCREEN_CMD_RESET;
      break;
    }
  }

  if (bpMenu != pMenu) {
    bpMenu = pMenu;
    if (ClrScreenFlag) {
      ClrScreenFlag = 0;
      pMenu = MHead;

      for (i = 0; i < 5; i++) {
        pMenu = pMenu->pNext;
      }
      pMenu = bpMenu;
    }
  }
}

/* Detector pairing menu handler */
static void stgMenu_LearnSensorCBS(void) {
  unsigned char keys, dat, tBuff[3];
  static unsigned char PairingComplete = 0;
  static unsigned short Timer = 0;
  Stu_DTC stuTempDevice; /* Used to initialize detector information when setting
                            detector parameters */
  if (pModeMenu->refreshScreenCmd == SCREEN_CMD_RESET) {
    pModeMenu->refreshScreenCmd = SCREEN_CMD_NULL;
    QueueEmpty(RFDRcvMsg);
    keys = 0xFF;
    PairingComplete = 0;
    Timer = 0;
  }

  if (QueueDataLen(RFDRcvMsg) && (!PairingComplete)) {
    QueueDataOut(RFDRcvMsg, &dat);
    if (dat = '#') {
      QueueDataOut(RFDRcvMsg, &tBuff[2]);
      QueueDataOut(RFDRcvMsg, &tBuff[1]);
      QueueDataOut(RFDRcvMsg, &tBuff[0]);

      stuTempDevice.Code[2] = tBuff[2];
      stuTempDevice.Code[1] = tBuff[1];
      stuTempDevice.Code[0] = tBuff[0];

      if (((stuTempDevice.Code[0] == SENSOR_CODE_DOOR_OPEN) ||
           (stuTempDevice.Code[0] == SENSOR_CODE_DOOR_CLOSE) ||
           (stuTempDevice.Code[0] == SENSOR_CODE_DOOR_TAMPER) ||
           (stuTempDevice.Code[0] == SENSOR_CODE_DOOR_LOWPWR))) {
        /* It's a wireless door status code */
        stuTempDevice.DTCType = DTC_DOOR;
      } else if ((stuTempDevice.Code[0] == SENSOR_CODE_REMOTE_ENARM) ||
                 (stuTempDevice.Code[0] == SENSOR_CODE_REMOTE_DISARM) ||
                 (stuTempDevice.Code[0] == SENSOR_CODE_REMOTE_HOMEARM) ||
                 (stuTempDevice.Code[0] == SENSOR_CODE_REMOTE_SOS)) {
        /* Wireless remote control code */
        stuTempDevice.DTCType = DTC_REMOTE;
      } else if ((stuTempDevice.Code[0] == SENSOR_CODE_PIR) ||
                 (stuTempDevice.Code[0] == SENSOR_CODE_PIR_LOWPWR) ||
                 (stuTempDevice.Code[0] == SENSOR_CODE_PIR_TAMPER)) {
        /* Wireless infrared code */
        stuTempDevice.DTCType = DTC_PIR_MOTION;
      }
      stuTempDevice.ZoneType = ZONE_TYP_1ST;
      if (AddDtc(&stuTempDevice) != 0xFF) {
        switch (stuTempDevice.DTCType) {
        case DTC_DOOR:
          /* UI displays a successful pairing:Added door dtc.. */
          break;
        case DTC_REMOTE:
          /* Added remote dtc.. */
          break;
        case DTC_PIR_MOTION:
          /* Added pir dtc.. */
          break;
        }
        PairingComplete = 1; /* Match completion flag */
        Timer = 0;
      } else {
        /* Fail... */
      }
    }
  }

  if (pModeMenu->keyVal != 0xFF) {
    keys = pModeMenu->keyVal;
    pModeMenu->keyVal = 0xFF;
    switch (keys) {
    case KEY5_CLICK_RELEASE: /* Cancel */
      pModeMenu = pModeMenu->pParent;
      pModeMenu->refreshScreenCmd = SCREEN_CMD_RECOVER;
      break;
    case KEY5_LONG_PRESS: /* Return to the desktop */
      pModeMenu = &generalModeMenu[GNL_MENU_DESKTOP];
      pModeMenu->refreshScreenCmd = SCREEN_CMD_RESET;
      break;
    }
  }

  if (PairingComplete) {
    Timer++;
    if (Timer > 150) {
      Timer = 0;
      pModeMenu = pModeMenu->pParent;
      pModeMenu->refreshScreenCmd = SCREEN_CMD_RESET;
    }
  }
}

/* Detector list menu handler */
static void stgMenu_DTCListCBS(void) {
  unsigned char keys;
  unsigned char i, j;
  unsigned char ClrScreenFlag;
  Stu_DTC tStuDtc;

  static unsigned char DtcNameBuff[PARA_DTC_SUM][16];
  static stu_mode_menu settingMode_DTCList_Sub_Menu[PARA_DTC_SUM];
  static stu_mode_menu *pMenu = 0;
  static stu_mode_menu *bpMenu = 0;
  static unsigned char stgMainMenuSelectedPos = 0;
  static stu_mode_menu *MHead, *MTail;
  static unsigned char pMenuIdx = 0;

  if (pModeMenu->refreshScreenCmd == SCREEN_CMD_RESET) {
    pModeMenu->refreshScreenCmd = SCREEN_CMD_NULL;
    pMenuIdx = 0;
    stgMainMenuSelectedPos = 1;
    bpMenu = 0;
    ClrScreenFlag = 1;
    keys = 0xFF;
    pMenu = settingMode_DTCList_Sub_Menu;
    /* Go through them one by one. Find the detectors that match */
    for (i = 0; i < PARA_DTC_SUM; i++) {
      if (CheckPresenceofDtc(i)) {
        GetDtcStu(&tStuDtc, i);
        (pMenu + pMenuIdx)->ID = pMenuIdx;
      }
    }
  }
}

/* Wifi distribution menu handler function */

static void stgMenu_WifiCBS(void) {}

/* Device information menu handler */
static void stgMenu_MachineInfoCBS(void) {}

/* 恢复出厂设置菜单处理函数 */
static void stgMenu_FactorySettingsCBS(void) {}

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