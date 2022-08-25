#include "eeprom.h"

// //控制I2C速度的延时
// void CT_Delay(void) { I2C_delay(2); }

static void hal_I2CConfig(void);
static void I2C_delay(__IO uint32_t Delay);
static void I2C_Start(void);
static void I2C_Stop(void);
static void I2C_NoAck(void);
static unsigned char I2C_WaitAck(void);
static void I2C_SendByte(unsigned char SendByte);
static unsigned char I2C_ReceiveByte(void);
static void I2C_WriteByte(unsigned short address, unsigned char dat);

static void hal_I2CConfig(void) {
  GPIO_InitTypeDef GPIO_Initure;
  __HAL_RCC_GPIOH_CLK_ENABLE(); // Start the GPIOH clock
  __HAL_RCC_GPIOI_CLK_ENABLE(); // Start the GPIOI clock

  GPIO_Initure.Pin = GPIO_PIN_6;           // PH6
  GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP; // Push-pull output
  GPIO_Initure.Pull = GPIO_PULLUP;         // 上拉
  GPIO_Initure.Speed = GPIO_SPEED_HIGH;    // 高速
  HAL_GPIO_Init(GPIOH, &GPIO_Initure);     // Initialize

  GPIO_Initure.Pin = GPIO_PIN_3;       // PI3
  HAL_GPIO_Init(GPIOI, &GPIO_Initure); // Initialize
}

static void I2C_delay(__IO uint32_t Delay) {
  uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * Delay);
  while (delay--)
    ;
}

void hal_eepromInit(void) { hal_I2CConfig(); }

static void I2C_Start(void) {
  hal_I2C_SDA(1);
  I2C_delay(1);
  hal_I2C_SCL(1);
  I2C_delay(1);
  hal_I2C_SDA(0);
  I2C_delay(1);
}

static void I2C_Stop(void) {
  hal_I2C_SDA(0);
  I2C_delay(1);
  hal_I2C_SCL(1);
  I2C_delay(1);
  hal_I2C_SDA(1);
  I2C_delay(1);
}

static void I2C_Ack(void) {
  hal_I2C_SCL(0);
  I2C_delay(1);
  hal_I2C_SDA(0);
  I2C_delay(1);
  hal_I2C_SCL(1);
  I2C_delay(1);
  hal_I2C_SCL(0);
  I2C_delay(1);
}

static void I2C_NoAck(void) {
  hal_I2C_SCL(0);
  I2C_delay(1);
  hal_I2C_SDA(1);
  I2C_delay(1);
  hal_I2C_SCL(1);
  I2C_delay(1);
  hal_I2C_SCL(0);
  I2C_delay(1);
}

static unsigned char I2C_WaitAck(void) {
  hal_I2C_SDA(1);
  hal_I2C_SDA_IO_IN();
  hal_I2C_SCL(1);
  I2C_delay(1);
  if (hal_I2C_SDA_INPUT()) {
    return 0;
  }
  hal_I2C_SCL(0);
  hal_I2C_SDA_IO_OUT();
  I2C_delay(1);
  return 1;
}

static void I2C_SendByte(unsigned char SendByte) {
  unsigned char i = 0;
  unsigned char temp;
  temp = SendByte;
  for (i = 0; i < 8; i++) {
    hal_I2C_SCL(0);
    I2C_delay(1);
    if (temp & 0x80) {
      hal_I2C_SDA(1);
    } else {
      hal_I2C_SDA(0);
    }
    I2C_delay(1);
    hal_I2C_SCL(1);
    I2C_delay(1);
    temp <<= 1;
  }
  hal_I2C_SCL(0);
  I2C_delay(1);
  hal_I2C_SDA(1);
  I2C_delay(1);
}

/* Bytes to write */
static void I2C_WriteByte(unsigned short address, unsigned char dat) {
  I2C_Start();

  I2C_SendByte(0xA0);
  I2C_WaitAck();

  I2C_SendByte((address >> 8) & 0xFF);
  I2C_WaitAck();

  I2C_SendByte(address & 0xFF);
  I2C_WaitAck();

  I2C_SendByte(dat);
  I2C_WaitAck();

  I2C_Stop();
}

static unsigned char I2C_ReceiveByte(void) {
  unsigned char i;
  unsigned char ReceiveByte = 0;

  hal_I2C_SCL(0);
  I2C_delay(1);
  hal_I2C_SDA(1);
  hal_I2C_SDA_IO_IN(); // SDA设置成输入
  for (i = 0; i < 8; i++) {
    ReceiveByte <<= 1;
    I2C_delay(1);
    hal_I2C_SCL(1);
    I2C_delay(1);
    if (hal_I2C_SDA_INPUT()) {
      ReceiveByte |= 0x01;
    }
    hal_I2C_SCL(0);
  }
  hal_I2C_SDA_IO_OUT(); // SDA设置成输出
  I2C_delay(1);
  return ReceiveByte;
}

// Read 1 byte
unsigned char I2C_ReadByte(unsigned short address) {
  unsigned char dat;
  I2C_Start();
  I2C_SendByte(0xA0);
  I2C_WaitAck();

  I2C_SendByte((address >> 8) & 0xFF);
  I2C_WaitAck();

  I2C_SendByte(address & 0xFF);
  I2C_WaitAck();

  I2C_Start();
  I2C_SendByte(0xA1);
  I2C_WaitAck();

  dat = I2C_ReceiveByte();
  I2C_NoAck();
  I2C_Stop();
  return dat;
}

// Read multiple bytes in succession
void I2C_Read(unsigned short address, unsigned char *pBuffer,
              unsigned short len) {
  unsigned short length;
  length = len;
  I2C_Start();
  I2C_SendByte(0xA0);
  I2C_WaitAck();

  I2C_SendByte((address >> 8) & 0xFF);
  I2C_WaitAck();

  I2C_SendByte(address & 0xFF);
  I2C_WaitAck();

  I2C_Start();
  I2C_SendByte(0xA1);
  I2C_WaitAck();

  // dat = I2C_ReceiveByte();
  while (length) {
    *pBuffer = I2C_ReceiveByte();
    if (length == 1)
      I2C_NoAck();
    else
      I2C_Ack();
    pBuffer++;
    length--;
  }

  I2C_Stop();
}