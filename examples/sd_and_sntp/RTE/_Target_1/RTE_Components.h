
/*
 * Auto generated Run-Time-Environment Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'testSD' 
 * Target:  'Target 1' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "stm32f4xx.h"

/* ARM::CMSIS:RTOS2:Keil RTX5:Library:5.5.3 */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_RTX5            /* CMSIS-RTOS2 Keil RTX5 */
/* Keil.ARM Compiler::Compiler:Event Recorder:DAP:1.4.0 */
#define RTE_Compiler_EventRecorder
          #define RTE_Compiler_EventRecorder_DAP
/* Keil.ARM Compiler::Compiler:I/O:File:File System:1.2.0 */
#define RTE_Compiler_IO_File            /* Compiler I/O: File */
          #define RTE_Compiler_IO_File_FS         /* Compiler I/O: File (File System) */
/* Keil.ARM Compiler::Compiler:I/O:STDOUT:ITM:1.2.0 */
#define RTE_Compiler_IO_STDOUT          /* Compiler I/O: STDOUT */
          #define RTE_Compiler_IO_STDOUT_ITM      /* Compiler I/O: STDOUT ITM */
/* Keil.MDK-Plus::File System:CORE:LFN Debug:6.14.1 */
#define RTE_FileSystem_Core             /* File System Core */
          #define RTE_FileSystem_LFN              /* File System with Long Filename support */
          #define RTE_FileSystem_Debug            /* File System Debug Version */
/* Keil.MDK-Plus::File System:Drive:Memory Card:6.14.1 */
#define RTE_FileSystem_Drive_MC_0       /* File System Memory Card Drive 0 */

/* Keil.MDK-Pro::Network:CORE:IPv4/IPv6 Debug:7.15.0 */
#define RTE_Network_Core                /* Network Core */
          #define RTE_Network_IPv4                /* Network IPv4 Stack */
          #define RTE_Network_IPv6                /* Network IPv6 Stack */
          #define RTE_Network_Debug               /* Network Debug Version */
/* Keil.MDK-Pro::Network:Interface:ETH:7.15.0 */
#define RTE_Network_Interface_ETH_0     /* Network Interface ETH 0 */

/* Keil.MDK-Pro::Network:Legacy API:7.15.0 */
#define RTE_Network_Legacy              /* Network Legacy */
/* Keil.MDK-Pro::Network:Service:SNTP Client:7.15.0 */
#define RTE_Network_SNTP_Client         /* Network SNTP Client */
/* Keil.MDK-Pro::Network:Service:Web Server Compact:HTTP:7.15.0 */
#define RTE_Network_Web_Server_RO       /* Network Web Server with Read-only Web Resources */
/* Keil.MDK-Pro::Network:Socket:TCP:7.15.0 */
#define RTE_Network_Socket_TCP          /* Network Socket TCP */
/* Keil.MDK-Pro::Network:Socket:UDP:7.15.0 */
#define RTE_Network_Socket_UDP          /* Network Socket UDP */
/* Keil::CMSIS Driver:Ethernet MAC:2.9 */
#define RTE_Drivers_ETH_MAC0            /* Driver ETH_MAC0 */
/* Keil::CMSIS Driver:Ethernet PHY:LAN8742A:1.3.0 */
#define RTE_Drivers_PHY_LAN8742A        /* Driver PHY LAN8742A */
/* Keil::CMSIS Driver:SPI:2.15 */
#define RTE_Drivers_SPI1                /* Driver SPI1 */
        #define RTE_Drivers_SPI2                /* Driver SPI2 */
        #define RTE_Drivers_SPI3                /* Driver SPI3 */
        #define RTE_Drivers_SPI4                /* Driver SPI4 */
        #define RTE_Drivers_SPI5                /* Driver SPI5 */
        #define RTE_Drivers_SPI6                /* Driver SPI6 */
/* Keil::Device:STM32Cube Framework:Classic:1.7.9 */
#define RTE_DEVICE_FRAMEWORK_CLASSIC
/* Keil::Device:STM32Cube HAL:Common:1.7.9 */
#define RTE_DEVICE_HAL_COMMON
/* Keil::Device:STM32Cube HAL:Cortex:1.7.9 */
#define RTE_DEVICE_HAL_CORTEX
/* Keil::Device:STM32Cube HAL:DMA:1.7.9 */
#define RTE_DEVICE_HAL_DMA
/* Keil::Device:STM32Cube HAL:ETH:1.7.9 */
#define RTE_DEVICE_HAL_ETH
/* Keil::Device:STM32Cube HAL:GPIO:1.7.9 */
#define RTE_DEVICE_HAL_GPIO
/* Keil::Device:STM32Cube HAL:PWR:1.7.9 */
#define RTE_DEVICE_HAL_PWR
/* Keil::Device:STM32Cube HAL:RCC:1.7.9 */
#define RTE_DEVICE_HAL_RCC
/* Keil::Device:STM32Cube HAL:RTC:1.7.9 */
#define RTE_DEVICE_HAL_RTC
/* Keil::Device:STM32Cube HAL:SPI:1.7.9 */
#define RTE_DEVICE_HAL_SPI
/* Keil::Device:STM32Cube HAL:SRAM:1.7.9 */
#define RTE_DEVICE_HAL_SRAM
/* Keil::Device:STM32Cube HAL:TIM:1.7.9 */
#define RTE_DEVICE_HAL_TIM
/* Keil::Device:Startup:2.6.3 */
#define RTE_DEVICE_STARTUP_STM32F4XX    /* Device Startup for STM32F4 */


#endif /* RTE_COMPONENTS_H */
