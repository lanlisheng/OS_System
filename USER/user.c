#include "user.h"
#include "key.h"
#include "main.h"
#include "stm32f7xx.h"

static void KeyEventHandle(KEY_VALUE_TYPEDEF keys);

void UserInit(void) { hal_KeyScanCBSRegister(KeyEventHandle); }
void UserProc(void) {}

/* 按键回调函数，驱动层回调处理函数 */
static void KeyEventHandle(KEY_VALUE_TYPEDEF keys) {
  if ((keys == KEY1_CLICK) || (keys == KEY2_CLICK) || (keys == KEY3_CLICK)) {
    HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
    //执行命令
  } else if ((keys == KEY1_LONG_PRESS) || (keys == KEY2_LONG_PRESS) ||
             (keys == KEY3_LONG_PRESS)) {
    HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET);
    HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
    //执行命令
  }
}