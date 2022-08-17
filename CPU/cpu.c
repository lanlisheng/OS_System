#include "cpu.h"

#include "OS_System.h"
#include "main.h"
#include "stm32f1xx.h"

static void hal_CoreClockInit(void);
static void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd, unsigned char *pSta);

/*
 * Function Name : hal_CPUInit
 * Descriptin    : cpuϵͳʱ����س�ʼ��
 * Input         : None
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void hal_CPUInit(void) {
  hal_CoreClockInit();
  OS_CPUInterruptCBSRegister(hal_CPU_Critical_Control);
}

/* ����ϵͳ������Ⱥ����ŵ���Ƭ����ʱ���жϴ������У���ʱƵ�ʣ�10ms */
/*
 * Function Name : hal_CoreClockInit
 * Descriptin    : cpuϵͳʱ�ӳ�ʼ��
 * Input         : None
 * Output        : None
 * Return        : None
 * Attention     :
 * ���ʱ�Ӿ���������ȵ�Tickֵ��Ϊ�˱�֤ʵʱ�ԣ�һ������Ϊ10ms
 */
static void hal_CoreClockInit(void) {
  SysTick_Config(SystemCoreClock / 100);  //��઼�����72MHZ������ֵ��72000000
  // STM32�̼��⺯��
  //ϵͳʱ��ʹ��72��48��216MHZ,������-1=1/SystemCoreClock ms,
}

/*
 * Function Name : hal_getprmask
 * Descriptin    : ��ȡCPU���ж�״̬
 * Input         : None
 * Output        : 0-���жϹر� 1-���жϴ�
 * Return        : None
 * Attention     : STM32�̼���ϵͳ����
 */
static unsigned char hal_getprimask(void) {
  //�˺�����STM32�̼���Ŀ⺯��__get_PRIMASK()
  return (!__get_PRIMASK());  // 0-���жϴ� 1-���жϹر�  ���Դ˴�ȡ��
}

/* ͨ��CPUע�ắ�����ں��жϴ�����ָ��ָ��Ƭ�����жϿ��ش����� */
/*
 * Function Name : hal_CPU_Critical_Control
 * Descriptin    : CPU�ٽ紦�����
 * Input         : cmd - ��������  *pSta - ���ж�״̬
 * Output        : None
 * Return        : None
 * Attention     : None
 */
void hal_CPU_Critical_Control(CPU_EA_TYPEDEF cmd, unsigned char *pSta) {
  if (cmd == CPU_ENTER_CRITICAL) {
    *pSta = hal_getprimask();  //�����ж�״̬
    __disable_irq();           //�ر�CPU�ж�
  } else if (cmd == CPU_EXIT_CRITICAL) {
    if (*pSta) {
      __enable_irq();  //��CPU���ж�
    } else {
      __disbale_irq();  //�ر�CPU���ж�
    }
  }
}

/**
 * @brief This function handles System tick timer.
 */
void SysTick_Handler(void) { OS_ClockInterruptHandle(); }