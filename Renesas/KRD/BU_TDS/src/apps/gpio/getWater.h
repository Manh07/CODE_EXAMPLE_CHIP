/**
 ******************************************************************************
 * @file    getWater.h
 * @author  Makipos Co.,LTD.
 * @version 1.0
 * @date    Jan 25, 2021
 * @brief   
 * @history
 ******************************************************************************/


#ifndef APPS_GPIO_GETWATER_H_
#define APPS_GPIO_GETWATER_H_

/* Includes ------------------------------------------------------------------*/
#include "stdbool.h"
#include "gpio.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	WATER_TYPE_RO = 0,
	WATER_TYPE_COLD,
	WATER_TYPE_COOL,
	WATER_TYPE_PH85,
    WATER_TYPE_PH90,
    WATER_TYPE_PH95,
	WATER_TYPE_MAX
} waterType_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

#define PIN_LEVEL_GET_WATER 1
/* Exported functions ------------------------------------------------------- */
void getWater_controlTurnOffAllWater();
bool getWater_waterIsOuting();
void getWater_controlGetWater(waterType_t waterType, bool isOn);
void getWater_toggleWater(waterType_t waterType);
void getWater_needDpAfterStopWater();
bool getWater_getStateWaterIsOuting(waterType_t waterType);
void getWater_initPin();
void getWater_checVdt5GetPh();
uint8_t getWater_lockupPwmFromCurrent(uint16_t current);
uint16_t getWater_lockupCurrentFromAdc(uint16_t adc);
void getWater_setCurrent(uint16_t current);
void getWater_controlPh(waterType_t waterType, bool isOn);
bool getWater_isAlkalineOuting();
bool getWater_isOutingButNotAlkalineOuting();
void getWater_checkUpdateValueToEsp();
uint32_t getWater_getValueFlowToEsp();
uint32_t getWater_getValueCurrentToEsp();
#endif /* APPS_GPIO_GETWATER_H_ */
