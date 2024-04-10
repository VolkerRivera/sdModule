#include <stdio.h>                      /* Standard I/O .h-file               */
#include <ctype.h>                      /* Character functions                */
#include <string.h>                     /* String and memory functions        */
#include "rl_fs.h"
#include "Board_LED.h"                  // CMSIS RTOS header file

#ifndef __SD_H

typedef struct {
	char codigoPIN[7]; //< 6 para el codigo + 1 para el \0 que indica fin de array
	char nombre[20];
	char apellido[20];
	char DNI [10];	//< 9 para el DNI + 1 para el \0 que indica fin de array
	bool estaDentro;
} identificacion;

uint8_t tieneAcceso(char codigoPIN[]); 
uint8_t addVecino(identificacion vecino);
uint8_t deleteVecino(char codigoPIN[], char DNI[]);
uint8_t estaDentro(char timestamp[], char DNI[], bool cambiarEstado); //cambiarEstado = false solo en consulta (pagina web p.ej), cambiarEstado = true cuando la persona accede/sale. Devuelve el estado TRAS la actualizacion
//uint8_t RFID_Used(); //quedaria añadir este numero a la estructura de identificacion
//uint8_t PIN_Used(char DNI[],char codigoPIN[]);
uint8_t registerPerson(char timestamp[], char DNI[], char InOut[]); //hora de entrada o salida, persona que entra o sale, metodo de entrada o salida
	
#endif
