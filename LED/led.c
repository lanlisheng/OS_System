#include "led.h"

#include "gpio.h"
#include "main.h"

void hal_ledProc(void) {
  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
  HAL_GPIO_TogglePin(BUZ_GPIO_Port, BUZ_Pin);
}