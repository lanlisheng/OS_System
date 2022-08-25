#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H

#include "main.h"

/* IO Mode Selection */
#define CT_SDA_IN()                                                            \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 0 << 3 * 2;                                                \
  } /* PI3输入模式 */

#define CT_SDA_OUT()                                                           \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 1 << 3 * 2;                                                \
  } /* PI3输出模式 */

/* IO operation function */
#define CT_IIC_SCL(n)                                                          \
  (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET)) // SCL

#define CT_IIC_SDA(n)                                                          \
  (n ? HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET)) // SDA

#define CT_READ_SDA HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3) // input SDA

// IIC所有操作函??
void CT_IIC_Init(void);                      //初???化IIC的IO??
void CT_IIC_Start(void);                     //发送IIC开始信??
void CT_IIC_Stop(void);                      //发送IIC停???信号
void CT_IIC_Send_Byte(uint8_t txd);          // IIC发送一??字节
uint8_t CT_IIC_Read_Byte(unsigned char ack); // IIC读取一??字节
uint8_t CT_IIC_Wait_Ack(void);               // IIC等待ACK信号
void CT_IIC_Ack(void);                       // IIC发送ACK信号
void CT_IIC_NAck(void);                      // IIC不发送ACK信号

#endif