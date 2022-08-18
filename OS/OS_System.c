/* 移植流程：软件rtos与硬件结合(内核移植)
 * 1.把OS_ClockInterruptHandle()放到单片机定时器中断处理函数里，定时频率10ms，一般采用单片机systick
 * 2.重写单片机总中断
 * 3.通过OS_CPUInterruptCBSRegister()函数把内核中断处理函数指针指向单片机总中断开关处理函数
 * * * */

#include "OS_System.h"

#include <stdio.h>

#include "main.h"

CPUInterrupt_CallBack_t CPUInterruptCtrlCBS; //定义一个函数指针变量

volatile OS_TaskTypeDef OS_Task[OS_TASK_SUM]; //结构中的数据不会在中断中被改变.

//编写系统内核第八步：创建注册CUP中断控制函数
/*
 * Function Name : OS_CPUInterruptCBSRegister
 * Descriptin    : 注册CPU中断控制函数
 * Input         : - pCPUInterrupCtrlCBS:cpu中断控制回调函数地址
 * Return        : None
 * Attention     : None
 */
void OS_CPUInterruptCBSRegister(CPUInterrupt_CallBack_t pCPUInterrupCtrlCBS) {
  if (CPUInterruptCtrlCBS == 0) {
    CPUInterruptCtrlCBS = pCPUInterrupCtrlCBS;
  }
}

/* 编写系统内核之第二步:系统任务初始化 */
void OS_TaskInit(void) {
  unsigned char i;
  for (i = 0; i < OS_TASK_SUM; i++) {
    OS_Task[i].task = 0;
    OS_Task[i].RunFlag = OS_SLEEP;
    OS_Task[i].RunPeriod = 0;
    OS_Task[i].RunTimer = 0;
  }
}

/* 编写系统内核第三步:创建系统任务函数 */
/* Function Name:void OS_CreatTask(unsigned char ID, void (*proc)(void),
unsigned short Period, OS_TaskStatusTypeDef flag)
* Descriptin    : 创建任务
* Input         : -    ID       : 任务ID
*                 -   (*proc)(): 用户函数入口地址
*                 -   Period   : 任务执行频率,单位:ms
*                 -   flag     : 任务就绪状态:OS_SLEEP-休眠
OS_RUN-运行,定时唤醒,没有作用
*Output        :  None
*Return        :  None
*Attention     :  None
* */
void OS_CreatTask(unsigned char ID, void (*proc)(void), unsigned short Period,
                  OS_TaskStatusTypeDef flag) {
  if (!OS_Task[ID].task) { //判断任务有没有被创建,不能重复创建
    OS_Task[ID].task = proc;
    OS_Task[ID].RunFlag = OS_SLEEP;
    OS_Task[ID].RunPeriod = Period;
    OS_Task[ID].RunTimer = 0;
  }
}

/* 编写系统内核第四步:创建任务系统定时函数(系统任务调度函数) */
/*
 * Function Name : OS_ClockInterruptHandle
 * Descriptin    : 系统任务调度函数
 * Input         : None
 * Return        : None
 * Attention     : 为保证任务实时性,必须放到10ms的定时器或者系统时钟中断函数里
 *
 */
void OS_ClockInterruptHandle(void) {
  unsigned char i;
  for (i = 0; i < OS_TASK_SUM; i++) {
    if (OS_Task[i].task) { //通过task函数指针指向不等于0判断任务是否被创建
      OS_Task[i].RunTimer++;
      if (OS_Task[i].RunTimer >=
          OS_Task[i].RunPeriod) { //判断计时器值是否达到任务需要执行的时间
        OS_Task[i].RunTimer = 0;
        OS_Task[i].RunFlag =
            OS_RUN; //把任务的状态设置为运行状态,任务调度会一直判断这个变量的值,如果是OS_RUN就混执行task函数指针指向的函数
      }
    }
  }
}

//编写系统内核第五步：创建任务开始函数
/*
 * Function Name : void OS_Start(void)
 * Descriptin    : 开始任务函数
 * Input         : None
 * Return        : None
 * Attention     : None
 */
void OS_Start(void) {
  unsigned char i;
  while (1) {
    for (i = 0; i < OS_TASK_SUM; i++) {
      if (OS_Task[i].RunFlag == OS_RUN) {
        OS_Task[i].RunFlag = OS_SLEEP;
        (*(OS_Task[i].task))(); //通过函数指针指向外部的函数。从而实现功能
      }
    }
  }
}

//编写系统内核第六步：创建任务唤醒函数
/*
 * Function Name : void OS_TaskGetUp(OS_TaskID_TypeDef taskID)
 * Descriptin    : 唤醒任务函数
 * Input         : -taskID : 需要被唤醒任务的ID
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void OS_TaskGetUp(OS_TaskID_TypeDef taskID) {
  unsigned char IptStatus;
  if (CPUInterruptCtrlCBS != 0) {
    //关闭单片机的总中断：防止在中断中改变当前任务状态
    CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
  }
  OS_Task[taskID].RunFlag = OS_RUN;
  if (CPUInterruptCtrlCBS != 0) {
    //打开单片机的总中断
    CPUInterruptCtrlCBS(CPU_EXIT_CRITICAL, &IptStatus);
  }
}

//编写系统内核第七步：创建任务休眠函数
/*
 * Function Name : void OS_TaskSleep(OS_TaskID_TypeDef taskID)
 * Descriptin    : 唤醒任务函数
 * Input         : -taskID : 需要被休眠任务的ID
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void OS_TaskSleep(OS_TaskID_TypeDef taskID) {
  unsigned char IptStatus;
  if (CPUInterruptCtrlCBS != 0) {
    //关闭单片机的总中断：防止在中断中改变当前任务状态
    CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
  }
  OS_Task[taskID].RunFlag = OS_SLEEP;
  if (CPUInterruptCtrlCBS != 0) {
    //打开单片机的总中断
    CPUInterruptCtrlCBS(CPU_EXIT_CRITICAL, &IptStatus);
  }
}

/* 队列程序设计思路
1.定义队列缓存数组大小
2.入列的操作:怎么入列，按照顺序存储到数组里面
3.出列的操作：先进先出原则
 */

/* 入列操作：
1.队列的缓存可以存储的数组小标位置
2.队列是否满了，后续应该怎么处理
 */

/********************************************************************************************************
 *  @函数名   S_QueueEmpty
 *  @描述     清空一个队列
 *  @参数     Head-队列头地址,  Tail-队列尾地址,   pBuff-队列缓存
 *  @返回值   无
 *  @注意    无
 ********************************************************************************************************/
void S_QueueEmpty(unsigned char **Head, unsigned char **Tail,
                  unsigned char *pBuff) {
  //队列的初始化过程
  *Head = pBuff;
  *Tail = pBuff;
}

/********************************************************************************************************
 *  @函数名   S_QueueDataIn
 *  @描述     输入一个字节数据进队列
 *  @参数     Head-队列头地址,  Tail-队列尾地址,   pBuff-队列缓存
 *  @返回值   无
 *  @注意     无
 ********************************************************************************************************/
/* 队列数据的入列操作 */
void S_QueueDataIn(unsigned char **Head, unsigned char **Tail,
                   unsigned char *pBuff, unsigned char Len,
                   unsigned char *pData, unsigned char DataLen) {
  unsigned char num;
  unsigned char IptStatus = 0;
  if (CPUInterruptCtrlCBS != 0) {
    CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
  }
  for (num = 0; num < DataLen; num++, pData++) {
    //单个单个入列的操作
    **Tail = *pData; //数据入列
    (*Tail)++;       //队尾指针加1

    //判断队列数据是否满了
    if (*Tail == pBuff + Len) {
      //数组首地址加上入列数据长度
      *Tail = pBuff;
    }
    //队尾指针与对头指针产生冲突：只入列，不出列的情况，防止违背先进先出原则
    if (*Tail == *Head) {
      if (++(*Head) == pBuff + Len) {
        *Head = pBuff;
      }
    }
  }
  if (CPUInterruptCtrlCBS != 0) {
    CPUInterruptCtrlCBS(CPU_EXIT_CRITICAL, &IptStatus);
  }
}

/********************************************************************************************************
 *  @函数名   S_QueueDataOut
 *  @描述     从队列里取出一个数据
 *  @参数     Head-队列头地址,  Tail-队列尾地址,   pBuff-队列缓存
 *  @返回值   取出的数据
 *  @注意     无
 ********************************************************************************************************/
unsigned char S_QueueDataOut(unsigned char **Head, unsigned char **Tail,
                             unsigned char *pBuff, unsigned char Len,
                             unsigned char *pData) {
  unsigned char status = 0;
  unsigned char IptStatus = 0;
  if (CPUInterruptCtrlCBS != 0) {
    CPUInterruptCtrlCBS(CPU_ENTER_CRITICAL, &IptStatus);
  }

  *pData = 0;
  //出列需要先判断缓存中有无数据
  if (*Head != *Tail) {
    //队列缓存中存在数据
    *pData = **Head;
    status = 1;
    if (++(*Head) == pBuff + Len) {
      *Head = pBuff;
    }
  }
  if (CPUInterruptCtrlCBS != 0) {
    CPUInterruptCtrlCBS(CPU_EXIT_CRITICAL, &IptStatus);
  }
  return status;
}

/********************************************************************************************************
 *  @函数名   S_QueueDataLen
 *  @描述     判断队列里数据的长度
 *  @参数     Head-队列头地址,  Tail-队列尾地址,   HBuff-队列缓存
 *  @返回值   队列里有数据个数
 *  @注意     无
 ********************************************************************************************************/
unsigned short S_QueueDataLen(unsigned char **Head, unsigned char **Tail,
                              unsigned char *pBuff, unsigned char Len) {
  //获取队列中的缓存长度
  if (*Tail > *Head) {
    return (*Tail - *Head);
  }

  //当入列很多数据，并且队列中没有进行出列操作，会存在队头指针大于队尾指针
  if (*Tail > *Head) {
    return (*Tail + Len - *Head);
  }
  return 0;
}