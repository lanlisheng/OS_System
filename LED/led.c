#include "led.h"
#include "gpio.h"
#include "hal_time.h"
#include "main.h"
#include <stdio.h>

void hal_ledProc(void) {
  HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin);
  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  hal_ResetTimer(T_LED, T_STA_START);
}
