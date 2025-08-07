
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         uart.h
 *
 * @author    	trongkn
 * 
 * @version   1.0
 * 
 * @date      
 * 
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */

#ifndef UART_H_
#define UART_H_


/******************************************************************************
* Includes
******************************************************************************/


#include "config.h"
#include <stdio.h>
#include <string.h>
#include "r_cg_macrodriver.h"
#include "Config_SCI6.h"
#include "adc.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/
#define UART_SEND_MAX_LEN (200)
#define MAX_QUEUE_DATA_UART (200)
#define COMMAND_MAX_LEN (15)
#define MSG_FIELD_SEPARATOR (',')
#define MSG_FIELD_SOF ('[')
#define MSG_FIELD_EOF (']')
#define MSG_FIELDS_MAX (5)

#define MSG_FIELD_ID (1)
#define MSG_FIELD_TYPE (0)
#define MSG_FIELD_DATA (1)
/******************************************************************************
* Types
******************************************************************************/


/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */





/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void UART_Init(void);

PUBLIC uint16  UART_ReadData(char * data,uint16 maxlen);

PUBLIC void UART_UartPuts (char *s);

PUBLIC void UART_Debug(char * data);

PUBLIC void UART_Process(void);

void UART_testReply(uint8_t* data, uint16_t len);
void UART_sendFrame(uint8_t* frame, uint16_t len);
/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


