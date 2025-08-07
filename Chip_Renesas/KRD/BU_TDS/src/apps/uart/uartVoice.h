/*
 * uartVoice.h
 *
 *  Created on: Oct 27, 2022
 *      Author: ASUS
 */

#ifndef APPS_UART_UARTVOICE_H_
#define APPS_UART_UARTVOICE_H_

#include "stdint.h"
#include "stdbool.h"
typedef enum
{
    MSG_IDX_REQUEST_GET_DATA,
    MSG_IDX_UP_VAN_XA,
    MSG_IDX_UP_BOM ,
    MSG_IDX_UP_BINH_AP,
    MSG_IDX_UP_NUOC_VAO,
    MSG_IDX_UP_HSD_REMAIN,
    MSG_IDX_UP_HSD_DEFAULT,
    MSG_IDX_UP_RUNTIME_DEFAULT,
    MSG_IDX_UP_RUNTIME_REMAIN,
    MSG_IDX_UP_CP_RESET_FITLER,
    MSG_IDX_UP_TDS,
    MSG_IDX_UP_TIME_PUMP,
    MSG_IDX_UP_ERROR,
    MSG_IDX_UP_TEMP_HOT,
    MSG_IDX_UP_TEMP_COLD,
    MSG_IDX_UP_CP_SETTING_FASTBLOCK,
    MSG_IDX_UP_CP_ENABLE_HOTTANK,
    MSG_IDX_UP_CP_ENABLE_COLDTANK,
    MSG_IDX_UP_CP_GET_WATER,
    MSG_IDX_UP_CP_XA_SUC,
    MSG_IDX_UP_CP_BU,       //bu hydrogen
    MSG_IDX_UP_NOTIFY,
    MSG_IDX_MAX
} msgIdx_list_t;

bool uartVoice_msgIdxIsControlling(uint8_t msgIdx);
void uartVoice_setMsgIdxIsControlling(uint8_t msgIdx, bool isControlling);

void uartVoice_sendFrameWithMsgIdx(uint8_t msgIdx, uint8_t* data, uint16_t len);
void uartVoice_processFrameFormat(uint8_t *buffer_rx, uint16_t length);
void uartVoice_sendResetToMcu();

#endif /* APPS_UART_UARTVOICE_H_ */
