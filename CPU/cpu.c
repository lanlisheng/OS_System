#include "cpu.h"

#include "OS_System.h"
#include "main.h"
#include "stm32f1xx.h"

static void hal_CoreClockInit(void);
static void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd, unsigned char *pSta);

/*
 * Function Name : hal_CPUInit
 * Descriptin    : cpu系统时钟相关初始化
 * Input         : None
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void hal_CPUInit(void) {
  hal_CoreClockInit();
  OS_CPUInterruptCBSRegister(hal_CPU_Critical_Control);
}

/* 首先系统任务调度函数放到单片机定时器中断处理函数中，定时频率：10ms */
/*
 * Function Name : hal_CoreClockInit
 * Descriptin    : cpu系统时钟初始化
 * Input         : None
 * Output        : None
 * Return        : None
 * Attention     :
 * 这个时钟决定任务调度的Tick值，为了保证实时性，一般设置为10ms
 */
static void hal_CoreClockInit(void) {
  SysTick_Config(SystemCoreClock / 100);  //嘀嗒计数：72MHZ，计数值：72000000
  // STM32固件库函数
  //系统时钟使用72、48、216MHZ,计数器-1=1/SystemCoreClock ms,
}

/*
 * Function Name : hal_getprmask
 * Descriptin    : 获取CPU总中断状态
 * Input         : None
 * Output        : 0-总中断关闭 1-总中断打开
 * Return        : None
 * Attention     : STM32固件库系统函数
 */
static unsigned char hal_getprimask(void) {
  //此函数是STM32固件库的库函数__get_PRIMASK()
  return (!__get_PRIMASK());  // 0-总中断打开 1-总中断关闭  所以此处取反
}

/* 通过CPU注册函数把内核中断处理函数指针指向单片机总中断开关处理函数 */
/*
 * Function Name : hal_CPU_Critical_Control
 * Descriptin    : CPU临界处理控制
 * Input         : cmd - 控制命令  *pSta - 总中断状态
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd, unsigned char *pSta) {
  if (cmd == CPU_ENTER_CRITICAL) {
    *pSta = hal_getprimask();  //保存中断状态
    __disable_irq();           //关闭CPU中断
  } else if (cmd == CPU_EXIT_CRITICAL) {
    if (*pSta) {
      __enable_irq();  //打开CPU总中断
    } else {
      __disbale_irq();  //关闭CPU总中断
    }
  }
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) { OS_ClockInterruptHandle(); }