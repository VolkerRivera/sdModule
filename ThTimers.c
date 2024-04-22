#include "cmsis_os2.h"                          // CMSIS RTOS header file
 
/*----------------------------------------------------------------------------
 *      Thread  'Thread_Name': Timers
 *---------------------------------------------------------------------------*/
 
osThreadId_t tid_Thread_Timers;                        // thread id

/* extern from rtc.c*/

extern osTimerId_t timer250ms;
extern osTimerId_t timeout2seg;
extern	int Init_timer250ms (void);
extern	int Init_Timeout2seg (void);

/* -----------------------------------*/
 
void Thread_Timers (void *argument);                   // thread function
 
int Init_Thread_Timers (void) {
 
  tid_Thread_Timers = osThreadNew(Thread_Timers, NULL, NULL);
  if (tid_Thread_Timers == NULL) {
    return(-1);
  }
 
  return(0);
}
 
void Thread_Timers (void *argument) {
	Init_timer250ms();
	Init_Timeout2seg();
	
  while (1) {
    /* Hilo bloqueado hasta que llega un flag que lo desbloquea */
		osThreadFlagsWait(0x00000002U, osFlagsWaitAny, osWaitForever);
		osTimerStart(timer250ms,250U); // Toggle LED rojo
		osTimerStart(timeout2seg,2000U); //Duracion toogle LED rojo
		
    osThreadYield();                            // suspend thread
  }
}
