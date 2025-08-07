/*
 * uartHandle.h
 *
 *  Created on: Dec 10, 2018
 *      Author: hp
 */

#ifndef APPS_UART_WIFI_UARTHANDLE_H_
#define APPS_UART_WIFI_UARTHANDLE_H_

#include "r_cg_macrodriver.h"
#include "config.h"

#define MAX_SIZE_BUFFER 100

typedef enum
{
	UART_STT_IDLE,
	UART_STT_DATA_COMING,
	UART_STT_DATA_DONE
} uart_data_status_t;


typedef struct
{
	uint8_t buffer_rx[100];
	uint8_t buffer_index;
	volatile uint8_t state_cnt;
	uart_data_status_t uart_stt;
} uart_handle_t;


void uartHandle_sendString(char* data);
void uartHandle_addDataFromItr(uint8_t u8Data);
void uartHandle_tick(void);		//1ms
uint16_t convParamStringToNumber(uint8_t* dataString_in, uint32_t* dataNumber_out);
void uartHandle_Init(void);
void uartHandle_setId(char* id, char* pw);
void uartHandle_getId();
void uartHandle_getHwFwEsp();
void uartHandle_getBleWifi();
void uartHandle_setRegStore();
void uartHandle_getRegStt();
void uartHandle_requestReset();
void uartHandle_getPassWord();
void uartHandle_sendResetEsp();
void uartHandle_requestBootLoader();
void uartHandle_requestCertificate();
#endif /* APPS_UART_WIFI_UARTHANDLE_H_ */
