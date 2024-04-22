/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network
 * Copyright (c) 2004-2019 Arm Limited (or its affiliates). All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server.c
 * Purpose: HTTP Server example
 *----------------------------------------------------------------------------*/

#include <stdio.h>

//#include "main.h" /* Si sda algun fallo raro puede ser esto */

#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE

#include "stm32f4xx_hal.h"              // Keil::Device:STM32Cube HAL:Common
#include "Board_LED.h"                  // ::Board Support:LED
#include "Board_Buttons.h"              // ::Board Support:Buttons
#include "Board_ADC.h"                  // ::Board Support:A/D Converter
#include "lcd.h"
#include "adc.h"
#include "rtc.h"
#include "sntp.h"
#include "ThTimers.h"
#include "SD.h"

// Main stack size must be multiple of 8 Bytes
#define APP_MAIN_STK_SZ (1024U)
uint64_t app_main_stk[APP_MAIN_STK_SZ / 8];
const osThreadAttr_t app_main_attr = {
  .stack_mem  = &app_main_stk[0],
  .stack_size = sizeof(app_main_stk)
};

extern uint16_t AD_in          (uint32_t ch);
extern uint8_t  get_button     (void);
extern void     netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len);

extern bool LEDrun;
extern char lcd_text[2][20+1];

extern osThreadId_t TID_Display;
extern osThreadId_t TID_Led;

extern ADC_HandleTypeDef adchandle;

/* RTC related variables */

typedef struct{
	
	char time[30];
	char date[30];

}HoraFecha;

extern HoraFecha getHoraFecha (void);

/* --------------------- */

bool LEDrun;
char lcd_text[2][20+1] = { "LCD line 1",
                           "LCD line 2" };

/* Thread IDs */
osThreadId_t TID_Display;
osThreadId_t TID_Led;

/* Thread declarations */
static void BlinkLed (void *arg);
static void Display  (void *arg);

__NO_RETURN void app_main (void *arg);

/* Read analog inputs */
uint16_t AD_in (uint32_t ch) {
  float val = 0;

  /*if (ch == 0) {
    ADC_StartConversion();
    while (ADC_ConversionDone () < 0);
    val = ADC_GetValue();
  }*/
	val = ADC_getVoltage(&adchandle , ch);
  return ((uint16_t)val);
}

// Read digital inputs 
uint8_t get_button (void) {
  return ((uint8_t)Buttons_GetState ());
}

/* IP address change notification */
void netDHCP_Notify (uint32_t if_num, uint8_t option, const uint8_t *val, uint32_t len) {

  (void)if_num;
  (void)val;
  (void)len;

  if (option == NET_DHCP_OPTION_IP_ADDRESS) {
    /* IP address change, trigger LCD update */
    osThreadFlagsSet (TID_Display, 0x01);
  }
}

/*----------------------------------------------------------------------------
  Thread 'Display': LCD display handler
 *---------------------------------------------------------------------------*/
static __NO_RETURN void Display (void *arg) {
  static uint8_t ip_addr[NET_ADDR_IP6_LEN];
  static char    ip_ascii[40];

  (void)arg;
	LCD_reset();
	LCD_init();
	LCD_update();
	
	write(1, "       MDK-MW       ");
	write(2, "HTTP Server example ");

  /* Print Link-local IPv6 address */
  netIF_GetOption (NET_IF_CLASS_ETH,
                   netIF_OptionIP6_LinkLocalAddress, ip_addr, sizeof(ip_addr));

  netIP_ntoa(NET_ADDR_IP6, ip_addr, ip_ascii, sizeof(ip_ascii));


  while(1) {
    /* Wait for signal from DHCP */
    osThreadFlagsWait (0x01U, osFlagsWaitAny, osWaitForever);

    /* Retrieve and print IPv4 address */
    /*netIF_GetOption (NET_IF_CLASS_ETH,
                     netIF_OptionIP4_Address, ip_addr, sizeof(ip_addr));

    netIP_ntoa (NET_ADDR_IP4, ip_addr, ip_ascii, sizeof(ip_ascii));*/

		
		
		erase_screen();
		write(1, getHoraFecha().time);
		write(2, getHoraFecha().date);
  }
}

/*----------------------------------------------------------------------------
  Thread 'BlinkLed': Blink the LEDs on an eval board
 *---------------------------------------------------------------------------*/
static __NO_RETURN void BlinkLed (void *arg) {
	
	//const uint8_t led_val[6] = { 0x01,0x02,0x04,0x08,0x10,0x20};
																
  //uint32_t cnt = 0U;

  (void)arg;

  LEDrun = true;
	
  while(1) {
    //Every 500 ms
    /*if (LEDrun == true) {
      LED_SetOut (led_val[cnt]);
      if (++cnt >= sizeof(led_val)) {
        cnt = 0U;
      }
    }*/ 
		
		// Ya no hacemos blink con los led pero usamos este delay en while cada 500ms como si fuera un timer para mandar flag al hilo
		// del LCD
    osDelay (500);
		osThreadFlagsSet (TID_Display, 0x01);
  }
}


/*----------------------------------------------------------------------------
  Main Thread 'main': Run Network
 *---------------------------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	/* Se inicializa el Network Driver */
  netInitialize ();
	
	/* Despues ya podemos hacer lo que sea que requiera conexion a internet*/

	// Iniciamos los hilos
  TID_Led     = osThreadNew (BlinkLed, NULL, NULL);
  TID_Display = osThreadNew (Display,  NULL, NULL);
	Init_Thread_Timers();
	
	osDelay(3000); //Se recomienda esperar 5seg antes de la primera sincronizacion con el servidor SNTP
	get_time();

  osThreadExit();
}
