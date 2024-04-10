#include "stm32f4xx_hal.h"
#include <stdio.h>
#include <string.h>
#include "rtc.h"
#include "sntp.h"

RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef sTime = {0};
RTC_DateTypeDef sDate = {0};
RTC_AlarmTypeDef alarma;

typedef struct{
	
	char time[30];
	char date[30];

}HoraFecha;

extern osThreadId_t tid_Thread_Timers; //hilo de los timers

//uint8_t hora []= {0x23,0x59,0x50}; /*[0]HH [1]MM [2]SS*/
//uint8_t fecha [] = {0x07,0x01,0x03,0x24};/*[0]WEEKDAY [1]DAY [2]MONTH [3]YEAR-2000*/

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct; //RTC

  /* Enable Power Control clock */
  __HAL_RCC_PWR_CLK_ENABLE();

  /* The voltage scaling allows optimizing the power consumption when the device is 
     clocked below the maximum system frequency, to update the voltage scaling value 
     regarding system frequency refer to product datasheet.  */
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
	
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
	/* Enable LSE Oscillator!!!!!!!!!!!!!!! (RTC)*/
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    
    Error_Handler();
  }
	

  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
	/* INICIALIZACION DEL RELOJ PARA EL RTC*/
	
	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
  PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
    Error_Handler();
	}
	

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}

void RTC_init(uint8_t hora [], uint8_t fecha []){

	__HAL_RCC_PWR_CLK_ENABLE();//quitar luego
	HAL_PWR_EnableBkUpAccess();
	__HAL_RCC_RTC_ENABLE();
	
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
  hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    /* Initialization Error */
    Error_Handler();
  }
	
	set_time_date(hora, fecha);
	//set_alarma();
	
}

void set_time_date(uint8_t time[], uint8_t date[]){
	/*TIME [0]HH [1]MM [2]SS*/
	/*DATE [0]WEEKDAY [1]DAY [2]MONTH [3]YEAR-2000*/
	//todo de 0x01 a 0xnn
	
	/*CONFIGURAR FECHA*/
	
	sDate.Year = date[3];
	sDate.Month = date[2];
	sDate.Date = date[1];
	sDate.WeekDay = date[0];
	
	/* Usamos BIN en lugar de BCD porque la estructura tm almacena los valores 
		 de esta forma */
	
	//if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BCD) != HAL_OK)
	if(HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	
	/*CONFIGURAR HORA*/
	sTime.Hours = time[0];
	sTime.Minutes = time[1];
	sTime.Seconds = time[2];
	sTime.TimeFormat = RTC_HOURFORMAT_24;
	sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sTime.StoreOperation = RTC_STOREOPERATION_RESET;
	
	//if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
	if(HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_Handler();
	}
	/* SOLO PARA EL EXAMEN: EN LUGAR DE SETEAR LA ALARMA (SE USARA PARA EL SIGUIENTE GUION), EN ESTE CASO SE SETEARA UN TIMEOUT
     QUE EJECUTE LA SINCRONIZACION SNTP EN SU CALLBACK */
  
  
	//set_alarma();
}

HoraFecha getHoraFecha (void){
	HoraFecha horafecha;
	
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	/* Por alguna razon poner aqui printf hace que luego se quede pillado al enviar la informacion a la pagina web */
	sprintf(horafecha.time, "Hora: %.2d:%.2d:%.2d",sTime.Hours,sTime.Minutes,sTime.Seconds);
	sprintf(horafecha.date, "Fecha: %.2d/%.2d/20%.2d",sDate.Date, sDate.Month, sDate.Year);
	
	return horafecha;
}

void set_alarma(void){
	HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BIN);
	
	/* Establecemos hora a la que sonará la próxima en base a la hora que es*/
	
	uint8_t WakeUpHour = sTime.Hours;
	uint8_t WakeUpMin = sTime.Minutes;
  uint8_t WakeUpSec = sTime.Seconds + 5;
	uint8_t WakeUpWeekDay = sDate.WeekDay;
	
	/* Configuramos los limites*/
	/* Si linea 175 y 176 usan RTC_FORMAT_BIN la comparacion se hace en decimal/binario,
		 si estan en RTC_FORMAT_BCD la comparación se hará en hexadecimal.
		 Lo mismo para establecer la hora. */
	
  if(WakeUpSec > 59){
    WakeUpSec -= 60;
    WakeUpMin++;
    if(WakeUpMin > 59){
      WakeUpMin -= 60;
      WakeUpHour++;
      
      if(WakeUpHour > 23){
        WakeUpHour = 0;
        WakeUpWeekDay ++; //Para que no se pierda cuando haya cambio de dia
        if(WakeUpWeekDay > 7){
          WakeUpWeekDay = 1; /* Si la mascara obvia el weekday da igual esta gestion */
        }
      }
    }
  }
  
	/* Configuramos la alarma*/
	
	alarma.AlarmTime.Hours = WakeUpHour;
  alarma.AlarmTime.Minutes = WakeUpMin;
  alarma.AlarmTime.Seconds = WakeUpSec;
  alarma.AlarmTime.SubSeconds = 0x0;
  alarma.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
  alarma.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
  alarma.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;
  alarma.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
  alarma.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY;
	/*
	- Si alarma.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_WEEKDAY -> Se refiere a dia de la semana (1-7)
	-	Si alarma.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE -> Se refiere a dia del mes (1-31)
	*/
	
  alarma.AlarmDateWeekDay = WakeUpWeekDay; //ajustar para que tambien suene en cambio de dia
  alarma.Alarm = RTC_ALARM_A;
  HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &alarma, RTC_FORMAT_BIN) != HAL_OK) {
		Error_Handler();
  }
	
}



void RTC_Alarm_IRQHandler(void) {//salta cuando salta la alarma
  HAL_RTC_AlarmIRQHandler(&hrtc);//limpiamos la interrupción
	
	/* Una vez salte la alrma tendremos que configurarla otra vez para que salte dentro de 1 minuto*/
	
	//get_time();
  
  /* SOLO PARA EXAMEN: AQUI SE DEBERIA MANDAR EL FLAG QUE ACTIVE LOS TIMERS QUE CONTROLEN LOS LEDS */
	
	/* Se manda una flag al hilo que se encarga de iniciarlos dado que Timers Cannot be called form Interrupt Services Routines*/
	
	//osThreadFlagsSet(tid_Thread_Timers ,0x00000002U); //Esta flag desbloqueara el hilo
	
}
