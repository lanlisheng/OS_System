#include "eeprom.h"
#include "ctiic.h"

#define EEPROM_PAGE_SIZE 32

void hal_eepromInit(void) { CT_IIC_Init(); }

//�ֽ�д
static void I2C_WriteByte(unsigned short address, unsigned char dat) {
  CT_IIC_Start();

  I2C_SendByte(0xA0);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte((address >> 8) & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte(address & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte(dat);
  CT_IIC_Wait_Ack();

  CT_IIC_Stop();
  // I2C_delay(1000);			//��ʱ10ms
}

//ҳд����,���Զ���ҳ����,24C64һҳ32Byte,num����д65523���ֽ�
void I2C_PageWrite(unsigned short address, unsigned char *pDat,
                   unsigned short num) {
  unsigned char *pBuffer, j;
  unsigned short len, i, page, remainder, addr, temp;
  pBuffer = pDat;
  len = num;
  addr = address;
  temp = 0;
  if (addr % EEPROM_PAGE_SIZE) //�ж�Ҫд�ĵ�ַ
  {
    temp =
        EEPROM_PAGE_SIZE -
        (addr % EEPROM_PAGE_SIZE); // 32-7=25 //�������ǰ��ַ��������ֽ���1ҳ
    if (len <= temp) {
      temp = len;
    }
  }

  //������д���ַҳ
  if (temp) {
    CT_IIC_Start();

    CT_IIC_Send_Byte(0xA0);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte((addr >> 8) & 0xFF);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte(addr & 0xFF);
    CT_IIC_Wait_Ack();

    for (j = 0; j < temp; j++) {
      CT_IIC_Send_Byte(pBuffer[j]);
      CT_IIC_Wait_Ack();
    }
    CT_IIC_Stop();
    // I2C_delay(20000);
  }

  len -= temp;
  addr += temp; //��ַ�����Ѿ�д����ֽ�

  page = len / EEPROM_PAGE_SIZE;
  remainder = len % EEPROM_PAGE_SIZE;
  for (i = 0; i < page; i++) {
    CT_IIC_Start();
    CT_IIC_Send_Byte(0xA0);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte((addr >> 8) & 0xFF);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte(addr & 0xFF);
    CT_IIC_Wait_Ack();

    for (j = 0; j < EEPROM_PAGE_SIZE; j++) {
      CT_IIC_Send_Byte(pBuffer[temp + j]);
      CT_IIC_Wait_Ack();
    }
    CT_IIC_Stop();
    addr += EEPROM_PAGE_SIZE;
    temp += EEPROM_PAGE_SIZE;
    // I2C_delay(20000);
  }

  if (remainder) {
    CT_IIC_Start();
    CT_IIC_Send_Byte(0xA0);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte((addr >> 8) & 0xFF);
    CT_IIC_Wait_Ack();

    CT_IIC_Send_Byte(addr & 0xFF);
    CT_IIC_Wait_Ack();

    for (j = 0; j < remainder; j++) {
      CT_IIC_Send_Byte(pBuffer[temp + j]);
      CT_IIC_Wait_Ack();
    }
    CT_IIC_Stop();
    // I2C_delay(20000);
  }
}

//��1���ֽ�
unsigned char I2C_ReadByte(unsigned short address) {
  unsigned char dat;
  CT_IIC_Start();
  CT_IIC_Send_Byte(0xA0);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte((address >> 8) & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte(address & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Start();
  CT_IIC_Send_Byte(0xA1);
  CT_IIC_Wait_Ack();

  dat = CT_IIC_Read_Byte(1);
  CT_IIC_NAck();
  CT_IIC_Stop();
  return dat;
}

//����������ֽ�
void I2C_Read(unsigned short address, unsigned char *pBuffer,
              unsigned short len) {
  unsigned short length;
  length = len;
  CT_IIC_Start();
  CT_IIC_Send_Byte(0xA0);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte((address >> 8) & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Send_Byte(address & 0xFF);
  CT_IIC_Wait_Ack();

  CT_IIC_Start();
  CT_IIC_Send_Byte(0xA1);
  CT_IIC_Wait_Ack();

  // dat = I2C_ReceiveByte();
  while (length) {
    *pBuffer = CT_IIC_Read_Byte(1);
    if (length == 1)
      CT_IIC_NAck();
    else
      CT_IIC_Ack();
    pBuffer++;
    length--;
  }

  CT_IIC_Stop();
}
