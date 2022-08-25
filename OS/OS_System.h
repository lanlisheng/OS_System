#ifndef _OS_SYSTEM_H_
#define _OS_SYSTEM_H_

#include "main.h"

extern void S_QueueEmpty(unsigned char **Head, unsigned char **Tail,
                         unsigned char *pBuff);
extern void S_QueueDataIn(unsigned char **Head, unsigned char **Tail,
                          unsigned char *pBuff, unsigned char Len,
                          unsigned char *pData, unsigned char DataLen);
extern unsigned char S_QueueDataOut(unsigned char **Head, unsigned char **Tail,
                                    unsigned char *pBuff, unsigned char Len,
                                    unsigned char *pData);
extern unsigned short S_QueueDataLen(unsigned char **Head, unsigned char **Tail,
                                     unsigned char *pBuff, unsigned char Len);

#define QueueEmpty(x)                                                          \
  S_QueueEmpty((unsigned char **)&(x).Head, (unsigned char **)&(x).Tail,       \
               (unsigned char *)(x).Buff)

#define QueueDataIn(x, y, z)                                                   \
  S_QueueDataIn((unsigned char **)&(x).Head, (unsigned char **)&(x).Tail,      \
                (unsigned char *)(x).Buff, sizeof((x).Buff), y, z)

#define QueueDataOut(x, y)                                                     \
  S_QueueDataOut((unsigned char **)&(x).Head, (unsigned char **)&(x).Tail,     \
                 (unsigned char *)(x).Buff, sizeof((x).Buff), y)
#define QueueDataLen(x)                                                        \
  S_QueueDataLen((unsigned char **)&(x).Head, (unsigned char **)&(x).Tail,     \
                 (unsigned char *)(x).Buff, sizeof((x).Buff))

/* 1.�����һ�����е����ݽṹ */
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[4 + 1];
} Queue4;

typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[8 + 1];
} Queue8;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[16 + 1];
} Queue16;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[32 + 1];
} Queue32;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[64 + 1];
} Queue64;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[90 + 1];
} Queue90;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[120 + 1];
} Queue120;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[340 + 1];
} Queue340;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[745 + 1];
} Queue745;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[128 + 1];
} Queue128;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[248 + 1];
} Queue248;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[256 + 1];
} Queue256;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[512 + 1];
} Queue512;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[1024 + 1];
} Queue1K;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[2048 + 1];
} Queue2K;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[5120 + 1];
} Queue5K;
typedef struct {
  unsigned char *Head;
  unsigned char *Tail;
  unsigned char Buff[10240 + 1];
} Queue10K;

typedef enum {
  CPU_ENTER_CRITICAL, // CPU�����ٽ�״̬
  CPU_EXIT_CRITICAL   // CPU�˳��ٽ�״̬
} CPU_EA_TYPEDEF;

/* ����һ��CPU�жϿ��ƻص�����ָ�룬������CPUInterrupt_CallBack_t
 * �βΣ�- cmd ������CPU�жϴ򿪻��ǹر��ж��ٽ硣
 *      - *pSta  : ͨ��ָ�봫���ַ
 * */
typedef void (*CPUInterrupt_CallBack_t)(CPU_EA_TYPEDEF cmd,
                                        unsigned char *pSta);

//ϵͳ����ID.����:��ö�������������С
typedef enum {
  OS_TASK1,
  OS_TASK2,
  OS_TASK3,
  OS_TASK4,
  OS_TASK_SUM
} OS_TaskID_TypeDef;

//ϵͳ��������״̬
typedef enum {
  OS_SLEEP,          //��������
  OS_RUN = !OS_SLEEP //��������
} OS_TaskStatusTypeDef;

// ��дϵͳ�ں�֮��һ��:����ϵͳ����ṹ��
typedef struct {
  void (*task)(void); //������ָ��:����ָ��ָ������,һ����while(1)�еĺ���
  OS_TaskStatusTypeDef
      RunFlag; //��������״̬:���߻�������
  unsigned short RunPeriod; //�������Ƶ��:����ָ��Ĵ�����ִ�е�Ƶ��
  unsigned short RunTimer; //������ȼ�ʱ��:ʱ��ﵽ�������Ƶ��ʱ,����ᱻִ��
} OS_TaskTypeDef;

void OS_CPUInterruptCBSRegister(CPUInterrupt_CallBack_t pCPUInterrupCtrlCBS);
void OS_TaskInit(void);
void OS_CreatTask(unsigned char ID, void (*proc)(void), unsigned short Period,
                  OS_TaskStatusTypeDef flag);
void OS_ClockInterruptHandle(void);
void OS_Start(void);
void OS_TaskGetUp(OS_TaskID_TypeDef taskID);
void OS_TaskSleep(OS_TaskID_TypeDef taskID);

#endif
