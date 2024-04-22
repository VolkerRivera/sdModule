/*------------------------------------------------------------------------------
 * MDK Middleware - Component ::Network:Service
 * Copyright (c) 2004-2018 ARM Germany GmbH. All rights reserved.
 *------------------------------------------------------------------------------
 * Name:    HTTP_Server_CGI.c
 * Purpose: HTTP Server CGI Module
 * Rev.:    V6.0.0
 *----------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "cmsis_os2.h"                  // ::CMSIS:RTOS2
#include "rl_net.h"                     // Keil.MDK-Pro::Network:CORE
#include "rtc.h"
#include "Board_LED.h"                  // ::Board Support:LED
#include "SD.h"

#if      defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma  clang diagnostic push
#pragma  clang diagnostic ignored "-Wformat-nonliteral"
#endif

/* RTC related*/
typedef struct{
	
	char time[30];
	char date[30];

}HoraFecha;

HoraFecha horafecha;
extern HoraFecha getHoraFecha (void);

//modulo sd
uint8_t paginaActual = 1;


// http_server.c
extern uint16_t AD_in (uint32_t ch);
extern uint8_t  get_button (void);

extern bool LEDrun;
extern char lcd_text[2][20+1];
extern osThreadId_t TID_Display;

// Local variables.
static uint8_t P2;
static uint8_t ip_addr[NET_ADDR_IP6_LEN];
static char    ip_string[40];


// My structure of CGI status variable.
typedef struct {
  uint8_t idx;
  uint8_t unused[3];
} MY_BUF;
#define MYBUF(p)        ((MY_BUF *)p)

// Process query string received by GET request.
void netCGI_ProcessQuery (const char *qstr) {
  netIF_Option opt = netIF_OptionMAC_Address;
  int16_t      typ = 0;
  char var[40];

  do {
    // Loop through all the parameters
    qstr = netCGI_GetEnvVar (qstr, var, sizeof (var));
    // Check return string, 'qstr' now points to the next parameter

    switch (var[0]) {
      case 'i': // Local IP address
        if (var[1] == '4') { opt = netIF_OptionIP4_Address;       }
        else               { opt = netIF_OptionIP6_StaticAddress; }
        break;

      case 'm': // Local network mask
        if (var[1] == '4') { opt = netIF_OptionIP4_SubnetMask; }
        break;

      case 'g': // Default gateway IP address
        if (var[1] == '4') { opt = netIF_OptionIP6_DefaultGateway; }
        else               { opt = netIF_OptionIP6_DefaultGateway; }
        break;

      case 'p': // Primary DNS server IP address
        if (var[1] == '4') { opt = netIF_OptionIP4_PrimaryDNS; }
        else               { opt = netIF_OptionIP6_PrimaryDNS; }
        break;

      case 's': // Secondary DNS server IP address
        if (var[1] == '4') { opt = netIF_OptionIP4_SecondaryDNS; }
        else               { opt = netIF_OptionIP6_SecondaryDNS; }
        break;
      
      default: var[0] = '\0'; break;
    }

    switch (var[1]) {
      case '4': typ = NET_ADDR_IP4; break;
      case '6': typ = NET_ADDR_IP6; break;

      default: var[0] = '\0'; break;
    }

    if ((var[0] != '\0') && (var[2] == '=')) {
      netIP_aton (&var[3], typ, ip_addr);
      // Set required option
      netIF_SetOption (NET_IF_CLASS_ETH, opt, ip_addr, sizeof(ip_addr));
    }
  } while (qstr);
}

// Process data received by POST request.
// Type code: - 0 = www-url-encoded form data.
//            - 1 = filename for file upload (null-terminated string).
//            - 2 = file upload raw data.
//            - 3 = end of file upload (file close requested).
//            - 4 = any XML encoded POST data (single or last stream).
//            - 5 = the same as 4, but with more XML data to follow.
void netCGI_ProcessData (uint8_t code, const char *data, uint32_t len) {
	identificacion vecino; //cada vez que llega un submit creamos un objeto tipo identificacion
  memset(&vecino, 0, sizeof(vecino)); //se inicializa a 0 para que al escribir los valores no salgan caracteres raros
	char var[40],passw[12];

  if (code != 0) {
    // Ignore all other codes
    return;
  }

  P2 = 0;
  LEDrun = true;
  if (len == 0) {
    // No data or all items (radio, checkbox) are off
    LED_SetOut (P2);
    return;
  }
  passw[0] = 1;
  do {
    // Parse all parameters
    data = netCGI_GetEnvVar (data, var, sizeof (var));
		printf("%s\n", var);
		fflush(stdout);
    if (var[0] != 0) {
      // First character is non-null, string exists
      if (strcmp (var, "led0=on") == 0) {
				//Si pulsamos el checkbox de led0
        P2 |= 0x01;
      }
      else if (strcmp (var, "led1=on") == 0) {
        P2 |= 0x02;
      }
      else if (strcmp (var, "led2=on") == 0) {
        P2 |= 0x04;
      }
      else if (strcmp (var, "led3=on") == 0) {
        P2 |= 0x08;
      }
      else if (strcmp (var, "led4=on") == 0) {
        P2 |= 0x10;
      }
      else if (strcmp (var, "led5=on") == 0) {
        P2 |= 0x20;
      }
      else if (strcmp (var, "ctrl=Browser") == 0) {
        LEDrun = false;
      }
      else if ((strncmp (var, "pw0=", 4) == 0) ||
               (strncmp (var, "pw2=", 4) == 0)) {
        // Change password, retyped password
        if (netHTTPs_LoginActive()) {
          if (passw[0] == 1) {
            strcpy (passw, var+4);
          }
          else if (strcmp (passw, var+4) == 0) {
            // Both strings are equal, change the password
            netHTTPs_SetPassword (passw);
          }
        }
      }
      else if (strncmp (var, "lcd1=", 5) == 0) {
        // LCD Module line 1 text
        strcpy (lcd_text[0], var+5);
        osThreadFlagsSet (TID_Display, 0x01);
      }
      else if (strncmp (var, "lcd2=", 5) == 0) {
        // LCD Module line 2 text
        strcpy (lcd_text[1], var+5);
        osThreadFlagsSet (TID_Display, 0x01);
      }
			else if (strncmp(var, "nombre=", 7) == 0) { //comprobamos si var contiene el valor "nombre" del formulario
            strncpy(vecino.nombre, var + 7, sizeof(vecino.nombre) - 1); //Si es asi, copiamos vecino.nombre el valor de var a partir del caracter 7 
      }																																	// y hasta un maximo del sizeof - 1, esto es para dejar siempre espacio para el \0 que finaliza el array de caracteres
			else if (strncmp(var, "apellidos=", 10) == 0) {
            strncpy(vecino.apellido, var + 10, sizeof(vecino.apellido) - 1);
      }
			else if (strncmp(var, "dni=", 4) == 0) {
            strncpy(vecino.DNI, var + 4, sizeof(vecino.DNI) - 1);
      }
			else if (strncmp(var, "password=", 9) == 0) {
            strncpy(vecino.codigoPIN, var + 9, sizeof(vecino.codigoPIN) - 1);
      }
			else if (strncmp(var, "dentro=", 7) == 0) {
				if (strncmp(var, "dentro=0", 8) == 0){
					vecino.estaDentro = false;
				}else{
					vecino.estaDentro = true;
				}
				
				if (vecino.nombre[0] && vecino.apellido[0] && vecino.DNI[0] && vecino.codigoPIN[0]){
					addVecino(vecino);
				}else{
					printf("Error añadiendo: No se han podido asignar todos los valores del formulario\n");
					fflush(stdout);
				}
      }else if (strncmp(var, "dni_delete=", 11) == 0) {
            strncpy(vecino.DNI, var + 11, sizeof(vecino.DNI) - 1);
      }
			else if (strncmp(var, "codigoPIN_delete=", 17) == 0) {
            strncpy(vecino.codigoPIN, var + 17, sizeof(vecino.codigoPIN) - 1);
				if (vecino.DNI[0] && vecino.codigoPIN[0]){
					deleteVecino(vecino.codigoPIN, vecino.DNI);
				}else{
					printf("Error eliminando: No se han podido asignar todos los valores del formulario\n");
					fflush(stdout);
				}
      }
			
    }
  } while (data);
  LED_SetOut (P2);
}

// Generate dynamic web data from a script line.
uint32_t netCGI_Script (const char *env, char *buf, uint32_t buflen, uint32_t *pcgi) {
  int32_t socket;
  netTCP_State state;
  NET_ADDR r_client;
  const char *lang;
  uint32_t len = 0U;
  uint8_t id;
  static uint32_t adv;
	/* Nuevo para la pagina RTC */
  netIF_Option opt = netIF_OptionMAC_Address;
  int16_t      typ = 0;
	
	//char line_one [128];
	//strcpy(line_one, read_this_line(1));
	
	

  switch (env[0]) {//Se evalua env[0], que es un caracter. Segun la letra que sea, se evaluan otros indices de env[]
    // Analyze a 'c' script line starting position 2
    case 'a' :
      // Network parameters from 'network.cgi'
      switch (env[3]) {
        case '4': typ = NET_ADDR_IP4; break;
        case '6': typ = NET_ADDR_IP6; break;

        default: return (0);
      }
      
      switch (env[2]) {
        case 'l':
          // Link-local address
          if (env[3] == '4') { return (0);                             }
          else               { opt = netIF_OptionIP6_LinkLocalAddress; }
          break;

        case 'i':
          // Write local IP address (IPv4 or IPv6)
          if (env[3] == '4') { opt = netIF_OptionIP4_Address;       }
          else               { opt = netIF_OptionIP6_StaticAddress; }
          break;

        case 'm':
          // Write local network mask
          if (env[3] == '4') { opt = netIF_OptionIP4_SubnetMask; }
          else               { return (0);                       }
          break;

        case 'g':
          // Write default gateway IP address
          if (env[3] == '4') { opt = netIF_OptionIP4_DefaultGateway; }
          else               { opt = netIF_OptionIP6_DefaultGateway; }
          break;

        case 'p':
          // Write primary DNS server IP address
          if (env[3] == '4') { opt = netIF_OptionIP4_PrimaryDNS; }
          else               { opt = netIF_OptionIP6_PrimaryDNS; }
          break;

        case 's':
          // Write secondary DNS server IP address
          if (env[3] == '4') { opt = netIF_OptionIP4_SecondaryDNS; }
          else               { opt = netIF_OptionIP6_SecondaryDNS; }
          break;
      }

      netIF_GetOption (NET_IF_CLASS_ETH, opt, ip_addr, sizeof(ip_addr));
      netIP_ntoa (typ, ip_addr, ip_string, sizeof(ip_string));
      len = (uint32_t)sprintf (buf, &env[5], ip_string);
      break;

    case 'b':
      // LED control from 'led.cgi'
      if (env[2] == 'c') {
        // Select Control
        len = (uint32_t)sprintf (buf, &env[4], LEDrun ?     ""     : "selected",
                                               LEDrun ? "selected" :    ""     );
        break;
      }
      // LED CheckBoxes
      id = env[2] - '0';
      //if (id > 7) {
			if (id > 5) {
        id = 0;
      }
      id = (uint8_t)(1U << id);
      len = (uint32_t)sprintf (buf, &env[4], (P2 & id) ? "checked" : "");
      break;

    case 'c':
      // TCP status from 'tcp.cgi'
      while ((uint32_t)(len + 150) < buflen) {
        socket = ++MYBUF(pcgi)->idx;
        state  = netTCP_GetState (socket);

        if (state == netTCP_StateINVALID) {
          /* Invalid socket, we are done */
          return ((uint32_t)len);
        }

        // 'sprintf' format string is defined here
        len += (uint32_t)sprintf (buf+len,   "<tr align=\"center\">");
        if (state <= netTCP_StateCLOSED) {
          len += (uint32_t)sprintf (buf+len, "<td>%d</td><td>%d</td><td>-</td><td>-</td>"
                                             "<td>-</td><td>-</td></tr>\r\n",
                                             socket,
                                             netTCP_StateCLOSED);
        }
        else if (state == netTCP_StateLISTEN) {
          len += (uint32_t)sprintf (buf+len, "<td>%d</td><td>%d</td><td>%d</td><td>-</td>"
                                             "<td>-</td><td>-</td></tr>\r\n",
                                             socket,
                                             netTCP_StateLISTEN,
                                             netTCP_GetLocalPort(socket));
        }
        else {
          netTCP_GetPeer (socket, &r_client, sizeof(r_client));

          netIP_ntoa (r_client.addr_type, r_client.addr, ip_string, sizeof (ip_string));
          
          len += (uint32_t)sprintf (buf+len, "<td>%d</td><td>%d</td><td>%d</td>"
                                             "<td>%d</td><td>%s</td><td>%d</td></tr>\r\n",
                                             socket, netTCP_StateLISTEN, netTCP_GetLocalPort(socket),
                                             netTCP_GetTimer(socket), ip_string, r_client.port);
        }
      }
      /* More sockets to go, set a repeat flag */
      len |= (1u << 31);
      break;

    case 'd':
      // System password from 'system.cgi'
      switch (env[2]) {
        case '1':
          len = (uint32_t)sprintf (buf, &env[4], netHTTPs_LoginActive() ? "Enabled" : "Disabled");
          break;
        case '2':
          len = (uint32_t)sprintf (buf, &env[4], netHTTPs_GetPassword());
          break;
      }
      break;

    case 'e':
      // Browser Language from 'language.cgi'
      lang = netHTTPs_GetLanguage();
      if      (strncmp (lang, "en", 2) == 0) {
        lang = "English";
      }
      else if (strncmp (lang, "de", 2) == 0) {
        lang = "German";
      }
      else if (strncmp (lang, "fr", 2) == 0) {
        lang = "French";
      }
      else if (strncmp (lang, "sl", 2) == 0) {
        lang = "Slovene";
      }
      else {
        lang = "Unknown";
      }
      len = (uint32_t)sprintf (buf, &env[2], lang, netHTTPs_GetLanguage());
      break;

    case 'f':
      // LCD Module control from 'lcd.cgi'
      switch (env[2]) {
        case '1':
          len = (uint32_t)sprintf (buf, &env[4], lcd_text[0]);
          break;
        case '2':
          len = (uint32_t)sprintf (buf, &env[4], lcd_text[1]);
          break;
      }
      break;

		/* ESTE CASO ES DURANTE LA ACTUALIZACION CONTINUA DE LA PAGINA AD*/
    case 'g': // primer parametro que sigue al comando "c" en CGI, correspondiente a env[0]
      // AD Input from 'ad.cgi'
      switch (env[2]) { 
        case '1':	//parametro que sigue en este caso a "c g ", correspondiente a env[2]
          adv = AD_in (10);
          len = (uint32_t)sprintf (buf, &env[4], adv);
          break;
        case '2':
          len = (uint32_t)sprintf (buf, &env[4], (double)((float)adv*3.3f)/4096);
          break;
        case '3':
          adv = (adv * 100) / 4096;
          len = (uint32_t)sprintf (buf, &env[4], adv);
          break;
				}
      break;

		/* ESTO ES SOLO PARA EL CGX*/
    case 'x': // Se ejecuta cuando el navegador web envia una solicitud con la variable 'x' en la URL
      // AD Input from 'ad.cgx'
      adv = AD_in (10); // adv sirve para leer el valor del potenciometro
      len = (uint32_t)sprintf (buf, &env[1], adv); //sprintf escribe "&env[1] + adv" en buf. La longitud de la
																									// cadena se alacena en la variable len
      break;
		
		//////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		case 'z':
			
			switch (env[2]) {
				//uint8_t numGente = cuanta_gente(); //retorna el numero de gente que tiene acceso
				//uint8_t numPaginas = numGente%9 == 0 ? numGente/9 : numGente/9 + 1; //num paginas que contendran gente
				
				
				case '0':	//parametro que sigue en este caso a "c g ", correspondiente a env[2]. Index
          
          break;
        case '1':
					
          len = (uint32_t)sprintf (buf, &env[1], read_this_line(1));
          break;
        case '2':
          
          break;
        case '3':
          
          break;
				case '4':
          
          break;
        case '5':
          
          break;
				case '6':
          
          break;
        case '7':
          
          break;
				case '8':
          
          break;
        case '9':
          
          break;
				}
		break;
		
		/////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
				/* CASO AÑADIDO PARA EL RTC*/
		case 'r': //pertenenciente a env[0]
			
			horafecha = getHoraFecha(); //Si no se pone esta llamada a funcion da error el char de la linea de abajo por expression expected
			char time_date [60];
			// Concatenamos hora y fecha y los copiamos a time_date
			strcpy(time_date,(strcat(horafecha.time, horafecha.date)));
		
			len = (uint32_t)sprintf (buf, &env[1], time_date);
		
		break;

    case 'y':
      // Button state from 'button.cgx'
      len = (uint32_t)sprintf (buf, "<checkbox><id>button%c</id><on>%s</on></checkbox>",
                               env[1], (get_button () & (1 << (env[1]-'0'))) ? "true" : "false");
      break;
		
  }
  return (len); // devuelve la longitud del mensaje
}

#if      defined (__ARMCC_VERSION) && (__ARMCC_VERSION >= 6010050)
#pragma  clang diagnostic pop
#endif
