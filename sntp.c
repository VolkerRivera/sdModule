#include "sntp.h"

extern osThreadId_t tid_Thread_Timers;

//const NET_ADDR4 ntp_server = { NET_ADDR_IP4, 0, 193, 147, 107, 33 }; // Solo necesario si queremos meter esta IP manualmente
static void time_callback (uint32_t seconds, uint32_t seconds_fraction);

 
void get_time (void) {

  //if ( netSNTPc_GetTime ((NET_ADDR *)&ntp_server, time_callback) == netOK) { // Para meter la IP del sntp server manualmente
	if (netSNTPc_GetTime (NULL, time_callback) == netOK) { // Si ponemos NULL se pone la que hemos configurado en el Wizard por defecto
    //printf ("SNTP request sent.\n"); Meter printf hace que se pare la ejecucion del programa "193.147.107.33"
  }
  else {
    //printf ("SNTP not ready or bad parameters.\n");
  }
}
 
static void time_callback (uint32_t seconds, uint32_t seconds_fraction) {
  if (seconds == 0) {
    //printf ("Server not responding or bad response.\n");
		/* Significaria que han pasado 0 segundos desde 1/1/1970, lo cual no tiene sentido*/
  }
  else {
    //printf ("%d seconds elapsed since 1.1.1970\n", seconds);
		/* Nos devuelve el epoc, que hay que pasarlo a formato normal*/
		epoch_to_human(seconds);
		
		/* Lanzar flag a hilo de timers para que active los correspondientes al parpadeo del led rojo */	
		osThreadFlagsSet(tid_Thread_Timers ,0x00000002U); //Esta flag desbloqueara el hilo
		/* Configuramos la alarma para que suene dentro de n minutos*/
		set_alarma();
  }
}

void epoch_to_human (uint32_t seconds){
	time_t now; // Tiempo en segundos
	struct tm ts; // Estructura con el tiempo ya separado en HH MM SS ¿ZZ?
	char buf[80];
	uint8_t hora [3];
	uint8_t fecha [4];
	
	// Get current time
	//En este caso son los segundos que nos devuelve el servidor sntp
	now = seconds; //mi servidor es de madrid asi que no hay que sumarle ni restarle horas
	
	//Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
	ts = *localtime(&now); // convertimos los segundos a la estructura tm
	strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
	hora[0] = ts.tm_hour + 1; //El mas 1 de aqui es para no andar gestionando nada +1 o -1 en la alarma y establecimiento de hora del rtc. 
	hora[1] = ts.tm_min;
	hora[2] = ts.tm_sec;
	
	fecha[0] = ts.tm_wday + 1;
	fecha[1] = ts.tm_mday + 1;
	fecha[2] = ts.tm_mon + 1;
	fecha[3] = (ts.tm_year + 1900) - 2000;
	
	set_time_date(hora,fecha);
	
}

/* CREACION DE TIMERS */
/* Se crea el timeout5sec que es el tiempo que el LED parpadeara, 
	 y el timerToogle que indica cuanto dura cada parpadeo */

/* TIMER 2 SEGUNDOS*/

osTimerId_t timeout2seg;
uint32_t exec1; 


int Init_Timeout2seg (void) {
  //osStatus_t status;                            // function return status
  // Create periodic timer
  exec1 = 1U;
  timeout2seg = osTimerNew((osTimerFunc_t)&Timeout2seg_Callback, osTimerOnce, &exec1, NULL); 
	
  return NULL;
}


/*TIMER TOOGLE 250 ms*/

osTimerId_t timer250ms;
uint32_t exec2; 

                  
 
int Init_timer250ms (void) {
  //osStatus_t status;                            // function return status
  // Create periodic timer
  exec2 = 2U;
  timer250ms = osTimerNew((osTimerFunc_t)&timer250ms_Callback, osTimerPeriodic, &exec2, NULL);
	
  return NULL;
}

void Timeout2seg_Callback  (void const *arg){                   // prototypes for timer callback function
	/* Una vez pasan los 5 segundos para de parpadear */
	osTimerStop(timer250ms);
}

void timer250ms_Callback  (void const *arg){
/* Cada 250 ms hace toogle*/
	//HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0);// LED VERDE
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_14);// LED ROJO
} 

