#ifndef _RFD_H_
#define _RFD_H_

//����һ��clk��ʱ�䳤(��λus)
#define RFD_CLK_SENDLEN 400

//��ʱ���ж�Ƶ��x(x��us�ж�һ��)
#define RFD_INT_FRQ 50

#define RFDCLKEND 0xFFFF

//����������������Χ
#define RFD_TITLE_CLK_MINL 20
#define RFD_TITLE_CLK_MAXL 44

//����������������Χ
#define RFD_DATA_CLK_MINL 2
#define RFD_DATA_CLK_MAXL 5

// Rfd�����ظ�����
#define RFD_TX_NUM 15

enum { RFD_READ_CLKLEN, RFD_READ_DATA };

typedef void (*RFD_RcvCallBack_t)(unsigned char *pBuff);

void hal_RFDInit(void);
void hal_RFDProc(void);

void hal_RFCRcvCBSRegister(RFD_RcvCallBack_t pCBS);

#endif