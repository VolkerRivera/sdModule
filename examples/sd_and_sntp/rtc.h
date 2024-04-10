#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file

#ifndef __RTC_H

/* Defines related to Clock configuration ------------------------------------*/
#define RTC_ASYNCH_PREDIV  0x7F   /* LSE as RTC clock */
#define RTC_SYNCH_PREDIV   0xFF /* LSE as RTC clock */
/*typedef struct{
	
	char time[30];
	char date[30];

}HoraFecha;*/

//FUNCIONES
  void SystemClock_Config(void);
  void Error_Handler(void);
	void RTC_init(uint8_t hora [], uint8_t fecha []);
	void set_time_date(uint8_t time [], uint8_t date []);
	/*HoraFecha getHoraFecha (void);*/
	void set_alarma(void);//alarma periodica cada minuto


	
#endif
