#ifndef _EEPROM_H_
#define _EEPROM_H_

#include "main.h"

/* IO Mode Selection */
#define hal_I2C_SDA_IO_IN()                                                    \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 0 << 3 * 2;                                                \
  } /* PI3输入模式 */

#define hal_I2C_SDA_IO_OUT()                                                   \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 1 << 3 * 2;                                                \
  } /* PI3输出模式 */

/* IO operation function */
#define hal_I2C_SCL(n)                                                         \
  (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET)) // SCL

#define hal_I2C_SDA(n)                                                         \
  (n ? HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET)) // SDA

#define hal_I2C_SDA_INPUT() HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3) // input SDA

void hal_eepromInit(void);
void I2C_Read(unsigned short address, unsigned char *pBuffer,
              unsigned short len);
// void I2C_PageWrite(unsigned short address, unsigned char *pDat,
//                    unsigned short num);

#endif