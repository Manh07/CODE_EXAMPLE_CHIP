
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         errorCheck.h
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

#ifndef APPS_ERRORCHECK_ERRORCHECK_H_
#define APPS_ERRORCHECK_ERRORCHECK_H_

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
typedef enum
{
	ERROR_TYPE_FILTER_0 = 0,
	ERROR_TYPE_FILTER_1,
	ERROR_TYPE_FILTER_2,
	ERROR_TYPE_FILTER_3,
	ERROR_TYPE_FILTER_4,
	ERROR_TYPE_FILTER_5,
	ERROR_TYPE_FILTER_6,
	ERROR_TYPE_FILTER_7,
	ERROR_TYPE_FILTER_8,
	ERROR_TYPE_FILTER_9,
	ERROR_TYPE_INCOME_WATER_LOST,
	ERROR_TYPE_INCOME_WATER_NO_STABILITY ,
	ERROR_TYPE_PUMP_RUN_OVER_TIME ,
	ERROR_TYPE_LEAK_WATER,
	ERROR_TYPE_TDS_IN,
	ERROR_TYPE_TDS_OUT,
	MAX_ERROR_NUM,
}ErrorType_t;

/******************************************************************************
* Global variables
******************************************************************************/


/******************************************************************************
* Global functions
******************************************************************************/
void ErrorCheck_init();
/**
 * @brief process error check module
 *
 * call this funtion in loop.
 * Required cycle is 100ms
 *
 * @return none
 */
void ErrorCheck_process();
bool ErrorCheck_haveError();
ErrorType_t ErrorCheck_getNextError(ErrorType_t currentError);
bool ErrorCheck_getError(ErrorType_t type);
uint32_t Error_getErrorBit();
bool ErrorCheck_haveErrorOtherErrorTds();
//callback
void ErroCheck_newError_cb(ErrorType_t newError);
void ErrorCheck_allErrorAreRemoved_cb();
/******************************************************************************
* Inline functions
******************************************************************************/






#endif /* APPS_ERRORCHECK_ERRORCHECK_H_ */
