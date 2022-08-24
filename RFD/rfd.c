#include "rfd.h"
#include "OS_System.h"
#include "hal_time.h"
#include "stm32f7xx.h"
#include <stdbool.h>

volatile Queue32 RFDBuff; /* RFD pulse level data receiving queue */
Queue8 RFDCodeBuff;       // RFD信箱
RFD_RcvCallBack_t RFDRcvCBS;
unsigned char RFDRcvSteps; /* RFD encodes the receive process variable */

volatile unsigned char RFD_DecodeFltTimerOk; // RFD接码定时时间到标志

// static void hal_RFDConfig(void);
static unsigned char hal_GetRFDIOSta(void);
static void hal_PulseAQT_Handle(void);
static void hal_RFDDecodeFlt_Handle(void);

void hal_RFDInit(void) {
  QueueEmpty(RFDBuff);
  QueueEmpty(RFDCodeBuff);
  hal_CreatTimer(T_RFD_PULSH_RCV, hal_PulseAQT_Handle, 1, T_STA_START);
}

void hal_RFDRcvCBSRegister(RFD_RcvCallBack_t pCBS) {
  if (RFDRcvCBS == 0) {
    RFDRcvCBS = pCBS;
  }
}

void hal_RFDProc(void) {
  Queue256 ClkTimeBuff;

  static unsigned short Time1 = 0, Time2 = 0;
  static unsigned char ReadDataFlag = 0;
  static unsigned char Len, Code[3];
  static unsigned char CodeTempBuff[3];

  /* Receive Code */
  switch (RFDRcvSteps) {
  case RFD_READ_CLKLEN: {
    unsigned char Temp, Num;
    static unsigned char Dsta = 0;
    static unsigned short Count = 0;
    QueueEmpty(ClkTimeBuff);
    while (QueueDataOut(RFDBuff, &Temp)) {
      Num = 8;
      while (Num--) {

        if (Dsta) {
          /* Bit "1" */
          if (!(Temp & 0x80)) {
            /* This checks whether the level is low. If it is low,
                    the pulse width of the high level ends */
            unsigned char Data;
            Data = Count / 256; /* Stores high level pulse width high bytes */
            Data |= 0x80;
            QueueDataIn(ClkTimeBuff, &Data, 1);
            Data = Count % 256;
            QueueDataIn(ClkTimeBuff, &Data, 1);
            /* 此处的Data的值*50us等于脉宽时间 */
            Dsta = 0;
            Count = 0;
          }
        } else {
          /* Bit "0" */
          if (Temp & 0x80) {
            unsigned char Data;
            Data = Count / 256;
            Data &= 0x7F;
            QueueDataIn(ClkTimeBuff, &Data, 1);
            Data = Count % 256;
            QueueDataIn(ClkTimeBuff, &Data, 1);
            Dsta = 1;
            Count = 0;
          }
        }
        Count++;
        Temp <<= 1;
      }
    }
  }

  case RFD_READ_DATA: {

    while (QueueDataLen(ClkTimeBuff)) {
      if (!ReadDataFlag) {
        unsigned char Temp;
        while (!Time1 || !Time2) {
          if (!Time1) {
            /* Get the first waveform data */
            while (QueueDataOut(ClkTimeBuff, &Temp)) {
              if (Temp & 0x80) {
                Temp &= 0xFF7F;
                Time1 = Temp * 256;
                QueueDataOut(ClkTimeBuff, &Temp);
                Time1 += Temp;
                Time2 = 0;
                break;
              } else {
                QueueDataOut(ClkTimeBuff, &Temp);
              }
            }

            if (!QueueDataLen(ClkTimeBuff)) {
              break;
            }
          }

          if (!Time2) {
            /* Gets the low level waveform */
            QueueDataOut(ClkTimeBuff, &Temp);
            Time2 = Temp * 256;
            QueueDataOut(ClkTimeBuff, &Temp);
            Time2 += Temp;
            /* Judge high level *20 time, and high level *44 time */
            if ((Time2 >= (Time1 * RFD_TITLE_CLK_MINL)) &&
                (Time2 <= (Time1 * RFD_TITLE_CLK_MAXL))) {
              Time1 = 0;
              Time2 = 0;
              Len = 0;
              ReadDataFlag = 1;
              break;
            } else {
              Time1 = 0;
              Time2 = 0;
            }
          }
        }
      }

      if (ReadDataFlag) {
        unsigned char Temp;
        if (!Time1) {
          if (QueueDataOut(ClkTimeBuff, &Temp)) {
            Temp &= 0xFF7F;
            Time1 = Temp * 256;
            QueueDataOut(ClkTimeBuff, &Temp);
            Time1 += Temp;
            Time2 = 0;
          } else {
            break;
          }
        }

        if (!Time2) {
          if (QueueDataOut(ClkTimeBuff, &Temp)) {
            bool recvSuccFlag;
            Time2 = Temp * 256;
            QueueDataOut(ClkTimeBuff, &Temp);
            Time2 += Temp;

            if ((Time1 > (Time2 * RFD_DATA_CLK_MINL)) &&
                (Time1 <= (Time2 * RFD_DATA_CLK_MAXL))) {
              unsigned char i, c = 0x80;
              /* "1" */
              for (i = 0; i < Len % 8; i++) {
                c >>= 1;
                c &= 0x7F;
              }
              Code[Len / 8] |= c;
              recvSuccFlag = 1;
            } else if ((Time2 >= (Time1 * RFD_DATA_CLK_MINL)) &&
                       (Time2 <= (Time1 * RFD_DATA_CLK_MAXL))) {
              unsigned char i, c = (unsigned char)0xFF7F;
              /* "0" */
              for (i = 0; i < Len % 8; i++) {
                c >>= 1;
                c |= 0x0080;
              }
              Code[Len / 8] &= c;
              recvSuccFlag = 1;
            } else {
              /* ERROR */
              recvSuccFlag = 0;
              ReadDataFlag = 0;
            }
            Time1 = 0;
            Time2 = 0;
            if ((++Len == 24) && recvSuccFlag) {
              ReadDataFlag = 0;
              if ((CodeTempBuff[0] == Code[0]) &&
                  (CodeTempBuff[1] == Code[1]) &&
                  (CodeTempBuff[2] == Code[2])) {
                RFD_CodeHandle(Code);
              } else {
                memcpy(CodeTempBuff, Code, 3);
              }
            }
          } else {
            break;
          }
        }
      }
    }
  }
  }
}

void RFD_CodeHandle(unsigned char *pCode) {
  static unsigned char tBuff[3];
  unsigned char temp;

  if ((hal_GetTimerState(T_RFD_RECODEFLT) == T_STA_START) &&
      (!RFD_DecodeFltTimerOk)) {
    // 1-定时器正在运行
    // 2-定时器计数未完成
    return;
  }

  hal_ResetTimer(T_RFD_RECODEFLT, T_STA_START); /* 复位并重启定时器 */
  memcpy(tBuff, pCode, 3);
  RFD_DecodeFltTimerOk = 0;
  temp = '#';
  QueueDataIn(RFDCodeBuff, &temp, 1);
  QueueDataIn(RFDCodeBuff, &tBuff[0], 3);
  if (RFDRcvCBS) {
    RFDRcvCBS(tBuff);
  }
}

/* Gets the level status of the RFD receiving I/O port */
static unsigned char hal_GetRFDIOSta(void) {
  return 0; /* Find the IO port where the module is linked */
}

/* RFD pulse acquisition Function,Acquisition cycle:50us */
static void hal_PulseAQT_Handle(void) {
  static unsigned char Temp, Count1 = 0;
  Temp <<= 1; /* Temp the left Bit, Assigned to Temp */
  if (hal_GetRFDIOSta()) {
    Temp |= 0x01; /* It's going to be plus 1 every time */
  } else {
    Temp &= 0xFE; /* I'm going to keep the first one 0, and everything else the
                     same */
  }
  if (++Count1 == 8) {
    /* Each time, 8-bit level data is collected and put into the queue */
    Count1 = 0;
    QueueDataIn(RFDBuff, &Temp, 1);
  }
  hal_ResetTimer(T_RFD_PULSH_RCV, T_STA_START);
}

// RFD重复码过滤处理，700ms内有重复码的话不做重复处理，这个作为回调函数放在700ms定时器中断里
static void hal_RFDDecodeFlt_Handle(void) { RFD_DecodeFltTimerOk = 1; }
