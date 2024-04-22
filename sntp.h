#include "stm32f4xx_hal.h"
#include "cmsis_os2.h"                          // CMSIS RTOS header file
#include <stdio.h>
#include <string.h>
#include <time.h> // conversion epoch to HH:MM:SS
#include "rl_net.h" // net_x variables
#include "rtc.h"


#ifndef __SNTP_H

	/*struct tm {
   int tm_sec;         / seconds,  range 0 to 59          /
   int tm_min;         / minutes, range 0 to 59           /
   int tm_hour;        / hours, range 0 to 23             /
   int tm_mday;        / day of the month, range 1 to 31  / 
   int tm_mon;         / month, range 0 to 11             / Sumar 1 en RTC
   int tm_year;        / The number of years since 1900   /
   int tm_wday;        / day of the week, range 0 to 6    / Sumar 1 en RTC
   int tm_yday;        / day in the year, range 0 to 365  /	RTC no lo admite
   int tm_isdst;       / daylight saving time             /
	};*/
	
	
	void get_time(void);
	static void time_callback (uint32_t seconds, uint32_t seconds_fraction);
	void epoch_to_human(uint32_t seconds);
	
	void Timeout2seg_Callback  (void const *arg);                   // prototypes for timer callback function
	void timer250ms_Callback  (void const *arg); 
	int Init_timer250ms (void);
	int Init_Timeout2seg (void);
	
#endif
