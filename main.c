/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   Main program body
  *
  * @note    modified by ARM
  *          The modifications allow to use this file as User Code Template
  *          within the Device Family Pack.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2017 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "lcd.h"
#include "Board_LED.h"
#include "sntp.h"
#include "rtc.h"

#ifdef RTE_CMSIS_RTOS2_RTX5 // define lo que va a hacer el RTOS de CMSIS
/**
  * Override default HAL_GetTick function
  */
uint32_t HAL_GetTick (void) {
  static uint32_t ticks = 0U;
         uint32_t i;

  if (osKernelGetState () == osKernelRunning) {
    return ((uint32_t)osKernelGetTickCount ());
  }

  /* If Kernel is not running wait approximately 1 ms then increment 
     and return auxiliary tick counter value */
  for (i = (SystemCoreClock >> 14U); i > 0U; i--) {
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
    __NOP(); __NOP(); __NOP(); __NOP(); __NOP(); __NOP();
  }
  return ++ticks;
}

#endif

/** @addtogroup STM32F4xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
	ADC_HandleTypeDef adchandle;
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
	uint8_t hora [] = {19,10,30};
	uint8_t fecha [] = {05,01,03,24};
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
	
void Init_BlueButton(void){
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); // Las interrupciones del boton azul corresponden a las EXTI15-10
	
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Se habilita el reloj del GPIO C dado que el boton esta asociado al GPIOC13
	
	static GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING; // Detecta los flancos de subida
	GPIO_InitStruct.Pull = GPIO_NOPULL; // No hay R pullup ni pulldown
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); // Se inicia el pin
	
}

int main(void)
{

  /* STM32F4xx HAL library initialization:
       - Configure the Flash prefetch, Flash preread and Buffer caches
       - Systick timer is configured by default as source of time base, but user 
             can eventually implement his proper time base source (a general purpose 
             timer for example or other time source), keeping in mind that Time base 
             duration should be kept 1ms since PPP_TIMEOUT_VALUEs are defined and 
             handled in milliseconds basis.
       - Low Level Initialization
     */
  HAL_Init();

  /* Configure the system clock to 168 MHz */
  SystemClock_Config();
  SystemCoreClockUpdate();

  /* Add your application code here
     */
	/* Iniciar el RTC de forma local se puede hacer hacer aqui dado que no necesita del SO, solo de la capa HAL.
		 Sin embargo, si queremos sincronizar con el server tendra que ser obligatoriamente despues de iniciar el SO.*/
	RTC_init(hora, fecha); // En apartado 4 se iniciara desde la callback del sntp
	//get_time();
	LED_Initialize();
	for (int i = 0; i < 6; i++){
		LED_Off(i); // Inicialmente apagamos todos los LEDs
	}
	LCD_reset();
	LCD_init();
	LCD_update();
	ADC1_pins_F429ZI_config(); //specific PINS configuration
	ADC_Init_Single_Conversion(&adchandle , ADC1); //ADC1 configuration
	Init_BlueButton(); // Este boton tendra asociada una IRQ_Handler en la que nos atendremos a las interrupciones externas

#ifdef RTE_CMSIS_RTOS2
  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize ();


  /* Create application main thread */
  osThreadNew(app_main, NULL, &app_main_attr);
	//Init_Thread_ADC();

  /* Start thread execution */
  osKernelStart();
#endif

  /* Infinite loop */
  while (1)
  {
  }
}

void EXTI15_10_IRQHandler (void){
	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13); //Manejador del botón azul
	
}

void HAL_GPIO_EXTI_Callback( uint16_t GPIO_Pin){ //Interrupción del pulsador azul
	if(GPIO_Pin==GPIO_PIN_13){
		uint8_t time_reset [] = {0,0,0};
		uint8_t date_reset [] = {1,1,1,0};
		set_time_date(time_reset,date_reset);
	}
 }
#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}

#endif

/**
  * @}
  */ 

/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
