#include "stm32f4xx_hal.h"
#include "adc.h"

#define RESOLUTION_12B 4096U
#define VREF 3.3f

/**
  * @brief config the use of analog inputs ADC123_IN10 and ADC123_IN13 and enable ADC1 clock
  * @param None
  * @retval None
  */
void ADC1_pins_F429ZI_config(){
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	__HAL_RCC_ADC1_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	/*PC0     ------> ADC1_IN10 ///
    PC3     ------> ADC1_IN13
    */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*GPIO_InitStruct.Pin = GPIO_PIN_3;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);*/
		
		

  }
/**
  * @brief Initialize the ADC to work with single conversions. 12 bits resolution, software start, 1 conversion
  * @param ADC handle
	* @param ADC instance
  * @retval HAL_StatusTypeDef HAL_ADC_Init
  */
int ADC_Init_Single_Conversion(ADC_HandleTypeDef *hadc, ADC_TypeDef  *ADC_Instance)
{
	
   /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc->Instance = ADC_Instance;
  hadc->Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
  hadc->Init.Resolution = ADC_RESOLUTION_12B;
  hadc->Init.ScanConvMode = DISABLE;
  hadc->Init.ContinuousConvMode = DISABLE;
  hadc->Init.DiscontinuousConvMode = DISABLE;
  hadc->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc->Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc->Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc->Init.NbrOfConversion = 1;
  hadc->Init.DMAContinuousRequests = DISABLE;
	hadc->Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(hadc) != HAL_OK)
  {
    return -1;
  }
 
	
	return 0;

}

/**
  * @brief Configure a specific channels and gets the voltage in float type. This funtion calls to  HAL_ADC_PollForConversion that needs HAL_GetTick()
  * @param ADC_HandleTypeDef
	* @param channel number
	* @retval voltage in float (resolution 12 bits and VRFE 3.3
  */
float ADC_getVoltage(ADC_HandleTypeDef *hadc, uint32_t Channel)
	{
		ADC_ChannelConfTypeDef sConfig = {0};
		HAL_StatusTypeDef status;

		uint32_t raw = 0;
		//float voltage = 0;
		 /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = Channel;
  sConfig.Rank = 1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(hadc, &sConfig) != HAL_OK)
  {
    return -1;
  }
		
		HAL_ADC_Start(hadc);
		
		do {
			status = HAL_ADC_PollForConversion(hadc, 0); //This funtions uses the HAL_GetTick(), then it only can be executed wehn the OS is running
		}while(status != HAL_OK);
		
		raw = HAL_ADC_GetValue(hadc);
		
		//voltage = raw*VREF/RESOLUTION_12B; 
		
		return raw;

}
	
// Example of using this code from a Thread 

/*osThreadId_t tid_ADC;

void Thread_ADC (void *argument);

uint8_t volumen;
float value;*/

/*int Init_Thread_ADC(void){
	
	tid_ADC = osThreadNew(Thread_ADC, NULL, NULL);
	
	if(tid_ADC == NULL){
		return (-1);
	}
	return (0);
}*/

/*void Thread_ADC (void *argument) {
  ADC_HandleTypeDef adchandle; //handler definition
	ADC1_pins_F429ZI_config(); //specific PINS configuration
	
	
	ADC_Init_Single_Conversion(&adchandle , ADC1); //ADC1 configuration
	
	
	uint8_t volumen_anterior;
	
	//MSGQUEUE_OBJ_t msgVolumen;
	
  while (1) {
    
	  value = ADC_getVoltage(&adchandle , 10 ); //get values from channel 10->ADC123_IN10
		//value = ADC_getVoltage(&adchandle , 13 ); ////get values from channel 13
		
		//Tenemos 16 pasos de volumen en el RDA, por tanto dividiremos value en pasos de 0.2V
		
		if(value >= 0 && value <0.2){
			volumen = 0;
			
		}else if(value >= 0.2 && value <0.4){
			volumen = 1;
			
		}else if(value >= 0.4 && value <0.6){
			volumen = 2;
			
		}else if(value >= 0.6 && value <0.8){
			volumen = 3;
			
		}else if(value >= 0.8 && value <1.0){
			volumen = 4;
			
		}else if(value >= 1.0 && value <1.2){
			volumen = 5;
			
		}else if(value >= 1.2 && value <1.4){
			volumen = 6;
			
		}else if(value >= 1.4 && value <1.6){
			volumen = 7;
			
		}else if(value >= 1.6 && value <1.8){
			volumen = 8;
			
		}else if(value >= 1.8 && value <2.0){
			volumen = 9;
			
		}else if(value >= 2.0 && value <2.2){
			volumen = 10;
			
		}else if(value >= 2.2 && value <2.4){
			volumen = 11;
			
		}else if(value >= 2.4 && value <2.6){
			volumen = 12;
			
		}else if(value >= 2.6 && value <2.8){
			volumen = 13;
			
		}else if(value >= 2.8 && value <3.0){
			volumen = 14;
			
		}else if(value >= 3.0 && value <=3.3){
			volumen = 15;
			
		}
		
		//Solo haremos envios cuando se produzcan cambios de volumen, asi que registramos el volumen cada vez que hay un cambio
		
		if(volumen_anterior != volumen){
			//empaquetamos, registramos y enviamos
			msgVolumen.volumen = volumen;
			volumen_anterior = volumen;
			osMessageQueuePut(cola_Volumen, &msgVolumen ,0U,osWaitForever);
			
		}
		
		osDelay(1000);
   
  }
}*/

