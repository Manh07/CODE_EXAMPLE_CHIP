/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        filterExpire.c
*
* @author    quanvu
*
* @version   1.0
*
* @date
*
* @brief
*
*******************************************************************************
*
* Detailed Description of the file. If not used, remove the separator above.
*
******************************************************************************/


#include "filterExpire.h"
#include "flash_app.h"
#include "user_config.h"
#include <string.h>
#include "timeCheck.h"
#include "updateParam.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/

// update after 200 phut
#define SAVE_TIME_INTERVAL 12000 //S
/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/
static uint32_t s_lifeTimeRemain[FILTER_NUM_MAX];



/******************************************************************************
* Local functions
******************************************************************************/
/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */


void updateToFlash()
{
	flash_app_writeBlock((uint8_t *)s_lifeTimeRemain, FILLTER_EXPIRE_BLOCK, sizeof(s_lifeTimeRemain));
}

/******************************************************************************
* Global functions
******************************************************************************/

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
void filterExpire_init()
{
    bool readOk = flash_app_readData((uint8_t*)s_lifeTimeRemain,FILLTER_EXPIRE_BLOCK,sizeof(s_lifeTimeRemain));

    if(!readOk)
    {
    	memcpy(s_lifeTimeRemain,g_userConfig.s_lifeTimeTotal,sizeof(s_lifeTimeRemain));
    	updateToFlash();
    }

}
void filterExpire_resetToDefault()
{
	memcpy(s_lifeTimeRemain,g_userConfig.s_lifeTimeTotal,sizeof(s_lifeTimeRemain));
	updateToFlash();
}

void filterExpire_process()
{
	static uint32_t lastTimeSave = 0;
	if(elapsedTime(g_sysTimeS,lastTimeSave) >= SAVE_TIME_INTERVAL)
	{
		filterExpire_minusTime(elapsedTime(g_sysTimeS,lastTimeSave));
		lastTimeSave = g_sysTimeS;
		updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
	}
}

void filterExpire_minusTime(uint32_t second)
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		if(s_lifeTimeRemain[i] > second)
		{
			s_lifeTimeRemain[i] -= second;
		}
		else // need new filter
		{
			s_lifeTimeRemain[i] = 0;
		}
	}
	updateToFlash();
}

uint32_t filterExpire_getSecond(uint8_t filIndex)
{
	return (s_lifeTimeRemain[filIndex]);
}

uint32_t filterExpire_getFilterHour(uint8_t filIndex)
{
	if(s_lifeTimeRemain[filIndex]%3600 >0)
	{
		return (uint32_t)(s_lifeTimeRemain[filIndex]/3600 +1);
	}
	else
	{
		return (uint32_t)(s_lifeTimeRemain[filIndex]/3600);
	}
}

void filterExpire_resetTimeAtIndex(uint8_t filIndex)
{

	s_lifeTimeRemain[filIndex] = g_userConfig.s_lifeTimeTotal[filIndex];
	updateToFlash();
}

void filterExpire_resetTimeAll()
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		s_lifeTimeRemain[i] = g_userConfig.s_lifeTimeTotal[i];
	}
	updateToFlash();
}

/**
 * @brief One line documentation
 *
 * A more detailed documentation
 *
 * @param arg1 the first function argument
 * @param arg2 the second function argument
 *
 * @return descrition for the function return value
 */
