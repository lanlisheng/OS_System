#ifndef __MYCT_IIC_H
#define __MYCT_IIC_H

#include "main.h"

/* IO Mode Selection */
#define CT_SDA_IN()                                                            \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 0 << 3 * 2;                                                \
  } /* PI3����ģʽ */

#define CT_SDA_OUT()                                                           \
  {                                                                            \
    GPIOI->MODER &= ~(3 << (3 * 2));                                           \
    GPIOI->MODER |= 1 << 3 * 2;                                                \
  } /* PI3���ģʽ */

/* IO operation function */
#define CT_IIC_SCL(n)                                                          \
  (n ? HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOH, GPIO_PIN_6, GPIO_PIN_RESET)) // SCL

#define CT_IIC_SDA(n)                                                          \
  (n ? HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_SET)                      \
     : HAL_GPIO_WritePin(GPIOI, GPIO_PIN_3, GPIO_PIN_RESET)) // SDA

#define CT_READ_SDA HAL_GPIO_ReadPin(GPIOI, GPIO_PIN_3) // input SDA

// IIC���в�����??
void CT_IIC_Init(void);                      //��???��IIC��IO??
void CT_IIC_Start(void);                     //����IIC��ʼ��??
void CT_IIC_Stop(void);                      //����IICͣ???�ź�
void CT_IIC_Send_Byte(uint8_t txd);          // IIC����һ??�ֽ�
uint8_t CT_IIC_Read_Byte(unsigned char ack); // IIC��ȡһ??�ֽ�
uint8_t CT_IIC_Wait_Ack(void);               // IIC�ȴ�ACK�ź�
void CT_IIC_Ack(void);                       // IIC����ACK�ź�
void CT_IIC_NAck(void);                      // IIC������ACK�ź�

#endif