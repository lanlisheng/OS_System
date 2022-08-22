#include "led.h"
#include "OS_System.h"
#include "gpio.h"
#include "hal_time.h"
#include "main.h"
#include <stdio.h>

Queue4 LedCmdBuff[LED_SUM]; /* LED Control Mail */

static void hal_Led1Drive(unsigned char sta);
static void hal_BuzDrive(unsigned char sta);
static void hal_ledConfig(void);
static void hal_LedHandle(void);

void (*hal_LedDrive[LED_SUM])(unsigned char) = {
    hal_Led1Drive,
    hal_BuzDrive,
};

unsigned short Led_Dark[] = {0, 10, LED_EFFECT_END};
unsigned short Led_Light[] = {1, 10, LED_EFFECT_END};
unsigned short Led_Light100ms[] = {1, 10, 0, 10, LED_EFFECT_END};
unsigned short Led_Blink1[] = {1, 10, 0, 10, LED_EFFECT_AGN, 2};
unsigned short Led_Blink2[] = {
    1, 10, 0, 10, 1, 10, 0, 10, 1, 10, 0, 200, LED_EFFECT_AGN, 6};
unsigned short Led_Blink3[] = {1, 30, 0, 30, LED_EFFECT_AGN, 2};
unsigned short Led_Blink4[] = {1, 50, 0, 50, LED_EFFECT_AGN, 2};

/* LED Timer Status */
unsigned short LedTimer[LED_SUM];
/* LED Status :1 or 0 */
unsigned short *pLed[LED_SUM];

/* LED Load Flag */
unsigned char LedLoadFlag[LED_SUM];

void hal_LedInit(void) {
  unsigned char i;
  hal_ledConfig(); /* LED GPIO Initialization */
  hal_CreatTimer(T_LED, hal_LedHandle, 200, T_STA_START); /* Timer */

  for (i = 0; i < LED_SUM; i++) {
    LedLoadFlag[i] = 0;
    pLed[i] = (unsigned short *)Led_Dark;
    LedTimer[i] = *(pLed[i] + 1);
    QueueEmpty(LedCmdBuff[i]);
  }
}

/* Toggle LED effects function
 * Run all the time under SysTick as an OS task
 * */
void hal_LedProc(void) {
  unsigned char i = 0;
  unsigned char cmd;
  for (i = 0; i < LED_SUM; i++) {
    if ((QueueDataLen(LedCmdBuff[i]) > 0) && (LedLoadFlag[i] == 0)) {
      /* Data exists in the LED queue, */
      QueueDataOut(LedCmdBuff[i], &cmd); /* Take the data */
      LedLoadFlag[i] = 1;
      switch (cmd) {
      case LED_DARK: {
        pLed[i] = (unsigned short *)Led_Dark;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_LIGHT: {
        pLed[i] = (unsigned short *)Led_Light;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_LIGHT_100MS: {
        pLed[i] = (unsigned short *)Led_Light100ms;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_BLINK1: {
        pLed[i] = (unsigned short *)Led_Blink1;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_BLINK2: {
        pLed[i] = (unsigned short *)Led_Blink2;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_BLINK3: {
        pLed[i] = (unsigned short *)Led_Blink3;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }

      case LED_BLINK4: {
        pLed[i] = (unsigned short *)Led_Blink4;
        LedTimer[i] = *(pLed[i] + 1);
        break;
      }
      }
    }
  }
}

/* LED special effect control function
 * input: type - Specify the led
 *        cmd  - LED special Effects Command
 *        clr  - Executed immediately:Clears data from the current queue
 * * */
void LedMsgInput(unsigned char type, LED_EFFECT_TYPEDEF cmd,
                 unsigned char clr) {
  unsigned char bLedCMD;
  if (type >= LED_SUM) {
    return;
  }
  bLedCMD = cmd;
  if (clr) {
    QueueEmpty(LedCmdBuff[type]);
    LedLoadFlag[type] = 0;
  }
  QueueDataIn(LedCmdBuff[type], &bLedCMD, 1);
}

static void hal_ledConfig(void) { /* GPIO Init */
}

static void hal_LedHandle(void) {
  unsigned char i;

  for (i = 0; i < LED_SUM; i++) {
    if (LedTimer[i]) {
      LedTimer[i]--;
    }
    if (!LedTimer[i]) {
      /* Time for timing */
      // Led_Light[] = {1,10,LED_EFFECT_END};
      if (*(pLed[i] + 2) == LED_EFFECT_END) {
        /* Determine if you need to loop the effects */
        LedLoadFlag[i] = 0; /* A single - run special effect can be overwritten
                               by other special effects during its run */
      } else {
        pLed[i] += 2; /* There are two cases:1.*(pLed[i]+2) ==
                         LED_EFFECT_AGN  2. It's still in special effects */
        if (*pLed[i] == LED_EFFECT_AGN) {
          // Led_Blink1[] = {1,10,0,10,LED_EFFECT_AGN,2};
          pLed[i] = pLed[i] - (*(pLed[i] + 1) * 2);
          /* Need to loop:The pointer repoints to the first one */
        }
        LedTimer[i] = *(pLed[i] + 1); /* To assign a value */
      }
    }
    /* Select which device performs the effects:0/1 */
    hal_LedDrive[i](*pLed[i]);
  }
  hal_ResetTimer(T_LED, T_STA_START); /* Initializing a Timer */
}

static void hal_Led1Drive(unsigned char sta) {
  if (sta) {
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
  }
}

static void hal_BuzDrive(unsigned char sta) {
  if (sta) {
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  } else {
    HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
  }
}