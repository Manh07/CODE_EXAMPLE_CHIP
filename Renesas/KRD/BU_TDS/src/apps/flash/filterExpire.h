/*
 * filterExpire.h
 *
 *  Created on: Oct 16, 2018
 *      Author: Admin
 */

#ifndef APPS_FLASH_FILTEREXPIRE_H_
#define APPS_FLASH_FILTEREXPIRE_H_


/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         filterExpire.h
 *
 * @author    	quanvu
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


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/




/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void filterExpire_init();
void filterExpire_process();
void filterExpire_minusTime(uint32_t second);
void filterExpire_resetTimeAtIndex(uint8_t filIndex);
uint32_t filterExpire_getSecond(uint8_t filIndex);
void filterExpire_resetToDefault();

uint32_t filterExpire_getFilterHour(uint8_t filIndex);
void filterExpire_resetTimeAll();

/******************************************************************************
* Inline functions
******************************************************************************/




#endif /* APPS_FLASH_FILTEREXPIRE_H_ */
