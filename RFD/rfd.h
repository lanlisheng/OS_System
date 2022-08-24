#ifndef _RFD_H_
#define _RFD_H_

//定义一个clk的时间长(单位us)
#define RFD_CLK_SENDLEN 400

//定时器中断频率x(x个us中断一次)
#define RFD_INT_FRQ 50

#define RFDCLKEND 0xFFFF

//引导码脉宽允许误差范围
#define RFD_TITLE_CLK_MINL 20
#define RFD_TITLE_CLK_MAXL 44

//数据码脉宽允许误差范围
#define RFD_DATA_CLK_MINL 2
#define RFD_DATA_CLK_MAXL 5

// Rfd发送重复次数
#define RFD_TX_NUM 15

enum { RFD_READ_CLKLEN, RFD_READ_DATA };

typedef void (*RFD_RcvCallBack_t)(unsigned char *pBuff);

void hal_RFDInit(void);
void hal_RFDProc(void);

void hal_RFCRcvCBSRegister(RFD_RcvCallBack_t pCBS);

#endif