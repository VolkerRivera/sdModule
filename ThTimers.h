#include "stm32f4xx_hal.h"
#include "cmsis_os2.h" 


#ifndef _THLTIMERS_H
#define _THTIMERS_H


int Init_Thread_Timers (void);
void Thread_Timers (void *argument);                   // thread function


#endif
