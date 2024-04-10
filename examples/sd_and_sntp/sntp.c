#include "sntp.h"

#define FlagGetTimeFromSNTP 0x00000008U
#define FlagResync            0x00000016U

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
    uint8_t time [] = {10,10,10};
    uint8_t date [] = {1,10,10,10};
    set_time_date(time, date);
    osThreadFlagsSet(tid_Thread_Timers ,FlagResync);//siguintes sincronizaciones sntp
    printf("Proxima sincronizacion SNTP: dentro de 30 segundos.\n");
  }
  else {
    //printf ("%d seconds elapsed since 1.1.1970\n", seconds);
		/* Nos devuelve el epoc, que hay que pasarlo a formato normal*/
		epoch_to_human(seconds);
		
		/* Lanzar flag a hilo de timers para que active los correspondientes al parpadeo del led rojo */	
		osThreadFlagsSet(tid_Thread_Timers ,FlagGetTimeFromSNTP); //Esta flag desbloqueara el hilo
		/* Configuramos la alarma para que suene dentro de n minutos*/
		//set_alarma();
    osThreadFlagsSet(tid_Thread_Timers ,FlagResync);//siguintes sincronizaciones sntp
    printf("Proxima sincronizacion SNTP: dentro de 30 segundos.\n");
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

