/**
  ******************************************************************************
  * @file    Templates/Src/main.c 
  * @author  MCD Application Team
  * @brief   STM32F4xx HAL API Template project 
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


#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS2                  // when RTE component CMSIS RTOS2 is used
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#endif

#ifdef RTE_CMSIS_RTOS2_RTX5

#include <stdio.h>                      /* Standard I/O .h-file               */
#include <ctype.h>                      /* Character functions                */
#include <string.h>                     /* String and memory functions        */
#include "rl_fs.h"
#include "Board_LED.h"

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

typedef struct {
	char codigoPIN[7]; //< 6 para el codigo + 1 para el \0 que indica fin de array
	char nombre[20];
	char apellido[20];
	char DNI [10];	//< 9 para el DNI + 1 para el \0 que indica fin de array
	bool estaDentro;
} identificacion;

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t statusADD = 10;
uint8_t statusDELETE= 10;
uint8_t statusINSIDE= 10;
uint8_t statusALLOWED = 10;
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
__NO_RETURN void app_main (void *arg);
uint8_t tieneAcceso(char codigoPIN[]); 
uint8_t addVecino(identificacion vecino);
uint8_t deleteVecino(char codigoPIN[], char DNI[]);
uint8_t estaDentro(char DNI[]);
fsStatus estado;
/* Private functions ---------------------------------------------------------*/
__NO_RETURN void app_main (void *arg) {
  (void)arg;
	

  osThreadExit();
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
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
	LED_Initialize();
	printf("Se han inicializado los LEDs.\n");

	//FILE *f; ///< Perteneciente a la libreria stdio.h
	
	// finit ("M:") Initialize the M: drive. Every drive must be initialized separately
	/*	Drive					File System				 Drive Type
		"F:" or "F0:"				EFS					NOR Flash Drive 0
		"F1:" 							EFS					NOR Flash Drive 1
		"R:" or "R0:"				FAT					RAM Drive 
		"M:" or "M0:"				FAT					Memory Card Drive 0 << Selected in congif wizard
		"M1:"								FAT					Memory Card Drive 1
		"U:" or "U0:"				FAT					USB Memory Stick  0
		"U1:"       				FAT					USB Memory Stick  1
		"N:" or "N0:"				FAT					NAND Flash Drive 0
		"N1:" 							FAT					NAND Flash Drive 1
	*/

	
	// fmount ("M:") Mount the M: drive.
	/* Mounting means that it will be checked if the drive's media is attached 
		 and formatted. After a successful mount, drive is ready for file I/O operations*/
	
	// Update a log file on SD card.
	/*
	r: Read mode. Opens a file for reading.
	w: Write mode. Opens an empty file for writing. If the file exists, 
		 the content is destroyed. If the file does not exist, an empty file is opened for writing.
	a: Append mode. Opens a file for appending text. If the file already exists, data is appended. 
		 If the file does not exist, an empty file is opened for writing. Opening a file with append 
		 mode causes all subsequent writes to be placed at the then current end-of-file, regardless 
		 of interfering calls to fseek.
	b: Binary mode. Can be appended to any character above, but has no effect. The character is 
		 allowed for ISO C standard conformance.
	+: Update mode. Can be used with any character above as a second or third character. When a 
		 file is opened with update mode, both reading and writing can be performed. Programmers 
		 must ensure that reading is not directly followed by writing without an interfering call to 
		 fflush or to a file positioning function (fseek or rewind). Also, writing should not be 
		 followed directly by reading without an interfering call to a file positioning function, 
		 unless the writing operation encounters EOF.
	*/
  
	identificacion Volker;
	strcpy(Volker.codigoPIN, "281218");
	strcpy(Volker.nombre, "Volker");
	strcpy(Volker.apellido, "Rivera");
	strcpy(Volker.DNI, "51023293R");
	Volker.estaDentro = false;
	
	printf("---------------------------------------------------------\n");
  statusADD = addVecino(Volker); //LED VERDE ;; ME AÑADE A MI
	HAL_Delay(500);
	printf("---------------------------------------------------------\n");
  statusALLOWED = tieneAcceso("281218"); //LED AZUL // deberia decir qe tengo acceso:
	HAL_Delay(500);
	printf("---------------------------------------------------------\n");
  statusINSIDE = estaDentro("51023293R"); // deberia decir que no estoy dentro: 
	HAL_Delay(500);
	printf("---------------------------------------------------------\n");
  statusDELETE = deleteVecino("999999","12345678A"); //LED ROJO ;; BORRA A WILLY
	HAL_Delay(500);
  printf("End of application.\n");
  


#ifdef RTE_CMSIS_RTOS2
  /* Initialize CMSIS-RTOS2 */
  osKernelInitialize ();

  /* Create thread functions that start executing, */
  osThreadNew(app_main, NULL, NULL);

  /* Start thread execution */
  osKernelStart();
#endif

  /* Infinite loop */
  while (1)
  {
  }
}

/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 168000000
  *            HCLK(Hz)                       = 168000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 8000000
  *            PLL_M                          = 25
  *            PLL_N                          = 336
  *            PLL_P                          = 2
  *            PLL_Q                          = 7
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 5
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

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

  /* STM32F405x/407x/415x/417x Revision Z devices: prefetch is supported */
  if (HAL_GetREVID() == 0x1001)
  {
    /* Enable the Flash prefetch */
    __HAL_FLASH_PREFETCH_BUFFER_ENABLE();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void)
{
  /* User may add here some code to deal with this error */
  while(1)
  {
  }
}


/* Añade un nuevo vecino si este aun no esta registrado
Devuelve code:

0: Vecino no añadido porque ya existe
1: Vecino añadido correctamente
2: Error al inicializar el disco M:
3: Error al montar el disco M:
4: Error al abrir el fichero RegistroVecinos.txt

*/

uint8_t addVecino(identificacion vecino){
	printf("Se va a añadir a un vecino.\n");
	fflush(stdout);
	const uint8_t size = 128; //Checkear la longitud maxima de cada linea
	char buffer [size];
	bool Existe = false;
	uint8_t code = 0;
	FILE *f; 
	if (finit ("M:") != fsOK) {
		//Error handling
		printf("No se ha podido inicializar la microSD.\n");
		fflush(stdout);
		code = 2;
		return code;
  }
	
  if (fmount ("M:") != fsOK) {
		//Error handling
    printf("No se ha podido montar la microSD.\n");
		fflush(stdout);
		funinit ("M:");
		code = 3;
		return code;
  }
	
	f = fopen ("M:\\REGVECINOS.TXT","a+");
	
  if (f == NULL) {
    //Error handling
    printf("File not found!\n"); 
		fflush(stdout);
		funmount("M:");
    funinit("M:");
		code = 4;
		return code;
  }
  else {//Si se encuentra el archivo
		rewind(f); //En caso de que se vuelva a abrir el archivo, empiece a leerse desde el principio siempre, no solo al crearlo.
		//Check if already exists
		while(fgets(buffer, sizeof(buffer), f) != NULL){
			if ((strstr(buffer, vecino.codigoPIN) != NULL) && (strstr(buffer, vecino.DNI)!= NULL)) {
				Existe = true;
				printf("Ya existe ese vecino.\n");
				fflush(stdout);
				break;
			}
		}
		
    // Write data to file
		if(!Existe){
			fseek(f, 0, SEEK_END); // Para añadir el nuevo vecino en una nueva linea al final y no donde se haya quedado fgets
			fprintf(f, "%s,%s,%s,%s,%d\n", vecino.codigoPIN, vecino.nombre, vecino.apellido, vecino.DNI, vecino.estaDentro);
			fflush(f);
			printf("Se ha añadido a %s %s.\n", vecino.nombre, vecino.apellido);
			fflush(stdout);
			code = 1;
      LED_On(0);
		}
  }
	// Close file
	fclose (f);
	funmount ("M:");
  funinit ("M:");
	
	
	
	
	return code;
}

/* Funcion que se encarga de eliminar un vecino de la base de datos. Primero evalua si dicha
persona existe mediante la asociacion correcta de codigoPIN y DNI. Si la persona que se pretende 
borrar no esta en la lista, no hay nada que borrar.

Para eliminar la linea no se borra directamente sino que se copia linea a linea del txt original
al txt temporal, a excepcion de la linea que contiene el codigoPIN y DNI asociados a la persona que 
se quiere borrar. Una vez se ha hecho esto, se borra el txt original y al txt temporal se le asigna 
el nombre del otro, pasando a ser este el txt original de la proxima vez que se quiera eliminar un
vecino de la BBDD.

return code:

0: Vecino que se quiere eliminar de la BBDD no se encuentra en ella.
1: Vecino se ha eliminado correctamente.
2: Error al inicializar el disco M:
3: Error al montar el disco M:
4: Error al abrir el fichero RegistroVecinos.txt
5: Vecino que se quiere eliminar se encuentra en la BBDD pero ha habido un error al eliminar sus datos
		- Error en fdelete: fdelete borra el txt que contiene el registro de vecinos original
		- Error en frename: frename renombra el txt temporal con el nombre del txt original, quedando 
			actualizado el fichero con todos los datos anteriores a excepcion del vecino que ha sido borrado
		
6: Vecino no se encuentra en la lista y ha habido un problema al eliminar el txt temporal

*/

uint8_t deleteVecino(char codigoPIN[], char DNI[]){
	printf("Se va a eliminar a un vecino.\n");
	fflush(stdout);
	char buffer [128];
	bool Existe = false;
	uint8_t code = 0;
	FILE *f, *fileTemporal;
	
	if (finit ("M:") != fsOK) {
		//Error handling
		printf("No se ha podido inicializar la microSD.\n");
		fflush(stdout);
		code = 2;
		return code;
  }
	
  if (fmount ("M:") != fsOK) {
		//Error handling
    printf("No se ha podido montar la microSD.\n");
		fflush(stdout);
		funinit("M:");
		code = 3;
		return code;
  }
	//a+ leo/escribo desde el final ;;; r+ escribo/leo desde el principio
	f = fopen ("M:\\REGVECINOS.TXT","a+");// si se pone "no permitira borrarlo luego", permite lectura desde cualquier parte del archivo
	fileTemporal = fopen("M:\\TEMP.TXT", "a+"); 
  
	if (f == NULL || fileTemporal == NULL) { // Si no se encuentra el archivo
    //Error handling
    printf("File not found!\n"); 
		fflush(stdout);
		code = 4;
		return code;
		
  }else { //Si se encuentra el archivo
    rewind(f);
		//Checkeamos si existe dicho vecino
		while(fgets(buffer, sizeof(buffer), f) != NULL){
			if ((strstr(buffer, codigoPIN) != NULL) && (strstr(buffer, DNI) != NULL)) {
				Existe = true;
				
				printf("Existe ese vecino.\n"); //print ok
				fflush(stdout);
				rewind(f); //Vuelve a poner el puntero al inicio del archivo
				break;
			}
		}
		
		//Hasta aqui todo correcto porque el archivo REGVECINOS.TXT se crea, se escribe y se lee correctamente
		
		if(Existe){ //si existe lo podemos borrar
			// Actualizamos el fichero copiando todas las lineas a otro y renombrandolo, a excepcion de la linea correspondiente al vecino a borrar
			while (fgets(buffer, sizeof(buffer), f) != NULL) {
        // Si la línea no contiene el DNI y el PIN a borrar, se escribe en el archivo temporal
				printf("Buffer leido: %s, DNI: %s, PIN: %s \n",buffer, DNI, codigoPIN);
				fflush(stdout);
        if ((strstr(buffer, codigoPIN) == NULL) && (strstr(buffer, DNI) == NULL)) { // strstr  a null pointer if the sequence is not present in haystack
            //fputs(buffer, fileTemporal);
						fseek(fileTemporal, 0, SEEK_END); // Para añadir el nuevo vecino en una nueva linea al final y no donde se haya quedado fgets
						fprintf(fileTemporal, "%s", buffer);
						fflush(fileTemporal);
						printf("Buffer escrito en temp: %s\n", buffer);
						fflush(stdout);
        }
			}
			//Una vez copiado cerramos los archivos
			fflush(f);
			fflush(fileTemporal);
			fclose(f);
			fclose(fileTemporal);
			//Eliminamos el original
      estado = fdelete("M:\\REGVECINOS.TXT", NULL);
			if(estado != fsOK){
				code = 5;
				funmount ("M:");
				funinit ("M:");
        printf("Error 5: no se pudo eliminar REGVECINOS.TXT\n");
				fflush(stdout);
				return code;
			}
			if (frename ("M:\\TEMP.TXT", "REGVECINOS.TXT") != fsOK){
				code = 5;
				funmount ("M:");
				funinit ("M:");
        printf("Error 5: no se pudo renombrar TEMP.TXT a REGVECINOS.TXT\n");
				fflush(stdout);
				return code;
			}
			printf("Vecino eliminado correctamente.\n");
			fflush(stdout);
			code = 1;
      LED_On(2);
		}else{// Si el vecino no existe
			printf("Dicho vecino no está registrado.\n");
			/* Se cierran ambos archivos */
			fclose(f);
			fclose(fileTemporal);
			printf("Ficheros cerrados, borrando TEMP.TXT...\n");
			fflush(stdout);
			/* Eliminamos el archivo temporal*/
			if(fdelete("M:\\TEMP.TXT", NULL) != fsOK){
				code = 6;
				funmount ("M:");
				funinit ("M:");
				return code;
			}
			
			code = 0;
		}
	
	}
	funmount ("M:");
  funinit ("M:");
	
	return code;
}

/* Evalua el permiso de acceso comprobando si el codigoPIN se encuentra en el Registro de Vecinos.
Devuelve code:

0: Acceso no permitido
1: Acceso permitido
2: Error al inicializar el disco M:
3: Error al montar el disco M:
4: Error al abrir el fichero RegistroVecinos.txt

*/

uint8_t tieneAcceso(char codigoPIN[]){ //codigoPIN es unico para cada persona
	printf("Comprobando permiso de acceso...\n");
	fflush(stdout);
	FILE *f;
	uint8_t code = 0;
	char buffer[128]; //< Buffer donde se almacenan los 128 caracteres leidos
	if (finit ("M:") != fsOK) {
		//Error handling
		printf("No se ha podido inicializar la microSD.\n");
		fflush(stdout);
		code = 2;
		return code;
  }
	
	if (fmount ("M:") != fsOK) {
		//Error handling
    printf("No se ha podido montar la microSD.\n");
		fflush(stdout);
		funinit("M:");
		code = 3;
		return code;
  }
	
	f = fopen ("M:\\REGVECINOS.TXT","r"); //< Puesto que unicamente vamos a leer este archivo
	
  if (f == NULL) {
    //Error handling
    printf("File not found!\n"); 
		fflush(stdout);
		funmount("M:"); // Desmontar antes de retornar si no se encuentra el archivo
    funinit("M:");
		code = 4;
		return code;
  }
  // Comprobacion validez del Codigo PIN
	/* fgets lee hasta que se hayan leido los 128 caracteres, hasta que se cambie de linea o 
	hasta final del archivo => En ningun momento el codigoPIN de f se quedara a medias.*/
	while(fgets(buffer, sizeof(buffer), f) != NULL){ //Mientras no se llegue al final del archivo se leen 128 caracteres de f y se almacenan en buffer
	//Cada vez que se realiza cada lectura:
	if(strstr(buffer, codigoPIN) != NULL){ //Si se encuentra en buffer dicha cadena
		code = 1;
    LED_On(1);
		break; // Sale del bucle en cuanto lo encuentra.
		}
	}
	// Close file
  fclose (f);
  funmount("M:");
  funinit("M:");
	
	if(code == 1) printf("Tiene acceso.\n");
	fflush(stdout);
	return code;
}

uint8_t estaDentro(char DNI[]){
	printf("Comprobando si dicha persona se encuentra dentro del edificio...\n");
	fflush(stdout);
	FILE *f;
	uint8_t code = 0;
	char isInside;
	char buffer[128]; //< Buffer donde se almacenan los 128 caracteres leidos
	
	if (finit ("M:") != fsOK) {
		//Error handling
		printf("No se ha podido inicializar la microSD.\n");
		fflush(stdout);
		code = 2;
		return code;
  }
	
	if (fmount ("M:") != fsOK) {
		//Error handling
    printf("No se ha podido montar la microSD.\n");
		fflush(stdout);
		funinit("M:");
		code = 3;
		return code;
  }
	
	f = fopen ("M:\\REGVECINOS.TXT","r"); //< Puesto que unicamente vamos a leer este archivo
	
  if (f == NULL) {
    //Error handling
    printf("File not found!\n"); 
		fflush(stdout);
		funmount("M:"); // Desmontar antes de retornar si no se encuentra el archivo
    funinit("M:");
		code = 4;
		return code;
  }
	while(fgets(buffer, sizeof(buffer), f) != NULL){
		if(strstr(buffer, DNI) != NULL){ //Si se encuentra el DNI en esa linea
			size_t length = strlen(buffer);
			isInside = buffer[length - 2]; // El índice length - 2 es el último carácter antes del '\n'
			if(isInside == '1'){ //Si se lee 1 quiere decir que la persona esta dentro, retornamos 1
				printf("Esta dentro del edificio.\n");
				fflush(stdout);
				code = 1;
			}else{ // La persona no esta dentro, retornamos 0
				printf("No esta dentro del edificio.\n");
				fflush(stdout);
				code = 0; 
			}
		}
	}
	funmount("M:"); // Desmontar antes de retornar si no se encuentra el archivo
  funinit("M:");
  return code;
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
