/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        user_config.c
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


#include "user_config.h"
#include "flash_app.h"
#include <string.h>
#include "filter_time.h"
#include "filterExpire.h"
#include "updateParam.h"

/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
UserConfig_t g_userConfig;
/******************************************************************************
* Constants and macros
******************************************************************************/
static const UserConfig_t USER_CONFIG_DEFAULD = {
#ifdef MODEL_LEAF
		// 1000 1000 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_888_TN
		// 1000 1000 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_888_TC
		// 1000 1000 1000 3600 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 12 36 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_IRO3_K92
		// 1000 1000 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_OPTIMUS_I1
		// 1000 1000 1000 3600 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 12 36 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_OPTIMUS_I2
		// 1000 1000 1000 3600 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 12 36 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_AIOTEC_4_3
		// 1200 1200 1200 3600 3600 1200 3600 1200 3600
		.filterRunTimeTotal = {4320000,4320000,4320000,12960000,43200000,4320000,4320000,4320000,43200000},
		// tháng: 12 12 12 36 36 12 36 12 36
		.s_lifeTimeTotal = {31104000,31104000,31104000,93312000,311040000,31104000,31104000,31104000,311040000},
#endif
#ifdef MODEL_IQ_IRO2
		// 1000 1000 1000 3600 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 12 36 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000},
#endif
#ifdef MODEL_TOPBOX
		// 1000 1000 3600 1000 1000 1000 1000 1000 1000
		.filterRunTimeTotal = {4320000,4320000,12960000,4320000,4320000,4320000,4320000,4320000,4320000},
		// tháng: 12 12 36 12 12 12 12 12 12
		.s_lifeTimeTotal = {31104000,31104000,93312000,31104000,31104000,31104000,31104000,31104000,31104000},
#endif
		.tdsLimitIn = 1000,
		.tdsLimitOut = 50,
		.tdsToTimeMode = false,
		.showTdsIn = false,
		.waitTimeUpdateTds = WAIT_TIME_UPDATE_TDS
};


/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/

/******************************************************************************
* Local variables
******************************************************************************/


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


void user_config_updateToFlash()
{
	flash_app_writeBlock((uint8_t *)&g_userConfig, USER_CONFIG_BLOCK, sizeof(g_userConfig));
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
void user_config_init()
{
    bool readOk = flash_app_readData((uint8_t*)&g_userConfig,USER_CONFIG_BLOCK,sizeof(g_userConfig));

    if(!readOk)
    {
    	memcpy(&g_userConfig,&USER_CONFIG_DEFAULD,sizeof(g_userConfig));
    	user_config_updateToFlash();
    }
}

void UserConfig_setFilterLifeTime(uint32_t lifeTime,uint8_t filterIndex)
{
	g_userConfig.filterRunTimeTotal[filterIndex] = lifeTime;
	user_config_updateToFlash();
}

//set giá trị tất cả các lõi từ wifi (khi thay lõi lọc mới, loại lõi lọc mới có thể có thông số khác)
void AppConfig_setAllFilterTimeTotal(uint32_t* value_hour_arr)
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		g_userConfig.filterRunTimeTotal[i] = (uint32_t)value_hour_arr[i] * 3600;  //đổi ra giây
	}
	user_config_updateToFlash();
	//reset het ve thoi gian ban dau
	filter_time_resetTimeAll();
	filterExpire_resetTimeAll();
	//update to server
	updateParam_setNeedUpdate(PARAM_TYPE_FILTERTOTAL);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEFILTER);
}

void AppConfig_setAllFilterExpireTotal(uint32_t* value_hour_arr)
{
	for(uint8_t i = 0; i< FILTER_NUM; i++)
	{
		g_userConfig.s_lifeTimeTotal[i] = (uint32_t)value_hour_arr[i] * 3600;  //đổi ra giây
	}
	user_config_updateToFlash();
	//reset het ve thoi gian ban dau
	filterExpire_resetTimeAll();
	filter_time_resetTimeAll();
	//update to server
	updateParam_setNeedUpdate(PARAM_TYPE_EXPIRETOTAL);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEFILTER);
}
//reset giá trị lõi thứ Index về giá trị mặc định đã set ở hàm AppConfig_setAllFilterTimeTotal va AppConfig_setAllFilterExpireTotal
void AppConfig_resetTimeIndex(uint8_t filterIndex)
{
	filter_time_resetTimeAtIndex(filterIndex);
	filterExpire_resetTimeAtIndex(filterIndex);

	updateParam_setNeedUpdate(PARAM_TYPE_RSFT);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEFILTER);
}

uint16_t userConfig_getFilterTimeTotalHour(uint8_t filIndex)
{
	if(g_userConfig.filterRunTimeTotal[filIndex]%3600 >0)
		{
			return (uint16_t)(g_userConfig.filterRunTimeTotal[filIndex]/3600 +1);
		}
		else
		{
			return (uint16_t)(g_userConfig.filterRunTimeTotal[filIndex]/3600);
		}
}

uint32_t userConfig_getFilterExpireTotalHour(uint8_t filIndex)
{
	if(g_userConfig.s_lifeTimeTotal[filIndex]%3600 >0)
		{
			return (uint32_t)(g_userConfig.s_lifeTimeTotal[filIndex]/3600 +1);
		}
		else
		{
			return (uint32_t)(g_userConfig.s_lifeTimeTotal[filIndex]/3600);
		}
}
void UserConfig_setTdsLimitIn(uint16_t tdsLimit)
{
	g_userConfig.tdsLimitIn = tdsLimit;
	user_config_updateToFlash();
}

void UserConfig_setTdsLimitOut(uint16_t tdsLimit)
{
	g_userConfig.tdsLimitOut = tdsLimit;
	user_config_updateToFlash();
}


uint32_t UserConfig_getDefaultLifeTimeHour(uint8_t filterIndex)
{
	 return (USER_CONFIG_DEFAULD.filterRunTimeTotal[filterIndex] / 3600);
}

uint32_t UserConfig_getWaitTimeUpdateTds()
{
#ifdef KAROFI
	return g_userConfig.waitTimeUpdateTds;
#endif
#ifdef KAROHOME
	return 0;
#endif
}
void UserConfig_setWaitTimeUpdateTds(uint32_t time)
{
	g_userConfig.waitTimeUpdateTds = time;
	user_config_updateToFlash();
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
void UserConfig_resetToDefault()
{
	memcpy(&g_userConfig,&USER_CONFIG_DEFAULD,sizeof(g_userConfig));
	user_config_updateToFlash();
}
