#ifndef APPS_FLASH_USER_CONFIG_H_
#define APPS_FLASH_USER_CONFIG_H_


/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         user_config.h
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
#include "r_flash_rx_if.h"
#include "r_flash_rx_config.h"
/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros
******************************************************************************/



/******************************************************************************
* Types
******************************************************************************/
typedef struct UserConfigs {
	uint32_t s_lifeTimeTotal[FILTER_NUM_MAX];	// tuổi thọ lõi lọc tính theo giây
	uint32_t filterRunTimeTotal[FILTER_NUM_MAX];	// tổng thời gian chạy lõi lọc tính theo giây
	uint16_t tdsLimitIn;		// giới hạn tds đầu vào
	uint16_t tdsLimitOut;		// giới hạn tds đầu ra
	bool tdsToTimeMode;		// chế độ thời gian lõi lọc theo tds
	bool showTdsIn;				// có hiển thị tds in
	uint32_t waitTimeUpdateTds;  // thời gian chờ từ lúc bơm chạy đến lúc bắt đầu hiển thị tds
} UserConfig_t;

/******************************************************************************
* Global variables
******************************************************************************/
extern UserConfig_t g_userConfig;
/******************************************************************************
* Global functions
******************************************************************************/
void user_config_init();
void UserConfig_setFilterLifeTime(uint32_t lifeTime,uint8_t filterIndex);
void UserConfig_setTdsLimitIn(uint16_t tdsLimit);
void UserConfig_setTdsLimitOut(uint16_t tdsLimit);
uint32_t UserConfig_getDefaultLifeTimeHour(uint8_t filterIndex);

uint32_t UserConfig_getWaitTimeUpdateTds();
void UserConfig_setWaitTimeUpdateTds(uint32_t time);
void UserConfig_resetToDefault();

void AppConfig_setAllFilterTimeTotal(uint32_t* value_hour_arr);
void AppConfig_setAllFilterExpireTotal(uint32_t* value_hour_arr);
void AppConfig_resetTimeIndex(uint8_t filterIndex);
uint32_t userConfig_getFilterExpireTotalHour(uint8_t filIndex);
uint16_t userConfig_getFilterTimeTotalHour(uint8_t filIndex);

/******************************************************************************
* Inline functions
******************************************************************************/



#endif /* APPS_FLASH_USER_CONFIG_H_ */
