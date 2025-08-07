/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        errorCheck.c
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


#include "errorCheck.h"
#include "gpio.h"
#include "timeCheck.h"
#include "user_config.h"
#include "adc.h"
#include "filter_time.h"
#include "pumpControl.h"
#include "adc.h"
#include "filterExpire.h"
#include "getWater.h"
#include "heatControl.h"
#include "uartVoice.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/

/******************************************************************************
* Constants and macros
******************************************************************************/
#define ENABLE_TDS_LIMIT_CHECK

#define MIN_TIME_WATER_STABILITY 	5000
#define MIN_TIME_WATER_HAVE 		10000
#define PUMP_RUN_OVER_TIME 			18000
#define TDS_LIMIT_DELAY				10000
#define E4_SHOW_INTERVAL 			3600000 // 1H
/******************************************************************************
* Local types
******************************************************************************/



typedef enum
{
	WATER_IN_ERROR_NOMAL = 0,
	WATER_IN_ERROR_LOST,
	WATER_IN_ERROR_NO_STABILITY,
}WaterInError_t;
/******************************************************************************
* Local function prototypes
******************************************************************************/
void checkFilter();
void checkWaterIn();
void checkPumpRunTime();
void checkTdsLimit();
void checkH2ODet();

void newErrorOccur(ErrorType_t error);

/******************************************************************************
* Local variables
******************************************************************************/
bool currentErrors[MAX_ERROR_NUM];
ErrorType_t currentDisplay;
uint32_t lastTimeShow;

bool s_oldWaterInState = true;
WaterInError_t s_waterInError = WATER_IN_ERROR_NOMAL;
uint32_t s_timeHaveWater = 0;
uint32_t s_timeLostWater = 0;
uint8_t s_waterInBlinkCnt = 0;
bool lastError = false;
ErrorType_t s_recentNewError;

uint32_t s_lastTimeShowE4 = 0;
bool needShowErrorOccur = false;
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
void newErrorOccur(ErrorType_t error)
{
	if(error == ERROR_TYPE_INCOME_WATER_LOST)
	{
		s_lastTimeShowE4 = g_sysTime;
	}
	currentErrors[error] = true;
    needShowErrorOccur = true;
	s_recentNewError = error;
}

void checkFilter()
{
    static bool isreset = true;
	for(uint8_t i = 0; i<FILTER_NUM ; i++)
	{
		if((filter_time_getFilterSecond(i) == 0) || (filterExpire_getSecond(i) == 0))
		{
            if(currentErrors[i] == false)
            {
                newErrorOccur((ErrorType_t)i);
                if(currentErrors[i])
                {
                    uint32_t errorBit = Error_getErrorBit();
                    uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_ERROR, (uint8_t*)&errorBit, 4);
                }
            }
		}
		else
		{
            if(currentErrors[i])
            {
                currentErrors[i] = false;
                uint32_t errorBit = Error_getErrorBit();
                uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_ERROR, (uint8_t*)&errorBit, 4);
            }
		}
	}

    if(isreset)
    {
        if(g_sysTime > 10000)
        {
            isreset = false;
            uint32_t errorBit = Error_getErrorBit();
            uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_ERROR, (uint8_t*)&errorBit, 4);
        }
    }
}
void checkWaterIn()
{
	if(CHECK_CO_AP_THAP )
	{
		if(!s_oldWaterInState)
		{
			s_oldWaterInState = true;
			s_timeHaveWater = g_sysTime;
		}
		else if(elapsedTime(g_sysTime,s_timeHaveWater) > MIN_TIME_WATER_HAVE)
		{
			s_waterInBlinkCnt = 0;
			if(s_waterInError != WATER_IN_ERROR_NOMAL)
			{
				s_waterInError = WATER_IN_ERROR_NOMAL;
				currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
				currentErrors[ERROR_TYPE_INCOME_WATER_NO_STABILITY] = false;
			}
		}
		if(s_waterInError == WATER_IN_ERROR_LOST)
		{
			s_waterInError = WATER_IN_ERROR_NOMAL;
			currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
			if(s_recentNewError == ERROR_TYPE_INCOME_WATER_LOST)
			{

			}
		}

	}
	else if((!CHECK_CO_AP_THAP)&& s_oldWaterInState)
	{
		s_oldWaterInState = false;
		s_timeLostWater = g_sysTime;

		if((elapsedTime(g_sysTime,s_timeHaveWater) < MIN_TIME_WATER_HAVE))
		{
			if((++s_waterInBlinkCnt >= 10) && (s_waterInError != WATER_IN_ERROR_NO_STABILITY))
			{
				s_waterInError = WATER_IN_ERROR_NO_STABILITY;
				currentErrors[ERROR_TYPE_INCOME_WATER_LOST] = false;
				newErrorOccur(ERROR_TYPE_INCOME_WATER_NO_STABILITY);
				s_waterInBlinkCnt = 0;
			}
		}
	}
	if((!CHECK_CO_AP_THAP)&& (!s_oldWaterInState)&&
			(elapsedTime(g_sysTime, s_timeLostWater) > MIN_TIME_WATER_STABILITY) &&
			(s_waterInError != WATER_IN_ERROR_LOST))
	{
		s_waterInError = WATER_IN_ERROR_LOST;
		currentErrors[ERROR_TYPE_INCOME_WATER_NO_STABILITY] = false;
		newErrorOccur(ERROR_TYPE_INCOME_WATER_LOST);
	}
}
void checkPumpRunTime()
{
	uint32_t timeRun = pumpControl_getTimePumpRun();
	if((timeRun > PUMP_RUN_OVER_TIME) && (!currentErrors[ERROR_TYPE_PUMP_RUN_OVER_TIME]))
	{
		pumpControl_turnOffAll();
		newErrorOccur(ERROR_TYPE_PUMP_RUN_OVER_TIME);
	}
}
void checkTdsLimit()
{
#ifdef ENABLE_TDS_LIMIT_CHECK
	static uint32_t timeTdsLimitIn = 0, timeTdsLimitOut = 0;
	if((ADC_GetTdsValueDisplay(TDS_IN_VALUE) > g_userConfig.tdsLimitIn) && (!currentErrors[ERROR_TYPE_TDS_IN]))
	{
		if(timeTdsLimitIn == 0)
		{
			timeTdsLimitIn = g_sysTime;
		}else if(elapsedTime(g_sysTime,timeTdsLimitIn) > TDS_LIMIT_DELAY)
		{
			newErrorOccur(ERROR_TYPE_TDS_IN);
		}
	}
	if((ADC_GetTdsValueDisplay(TDS_OUT_VALUE) > g_userConfig.tdsLimitOut)&& (!currentErrors[ERROR_TYPE_TDS_OUT]))
	{
		if(timeTdsLimitOut == 0)
		{
			timeTdsLimitOut = g_sysTime;
		}else if(elapsedTime(g_sysTime,timeTdsLimitOut) > TDS_LIMIT_DELAY)
		{
			newErrorOccur(ERROR_TYPE_TDS_OUT);
		}
	}
	if(ADC_GetTdsValueDisplay(TDS_IN_VALUE) < g_userConfig.tdsLimitIn)
	{
		if(currentErrors[ERROR_TYPE_TDS_IN])
		{
			currentErrors[ERROR_TYPE_TDS_IN] = false;
		}
		timeTdsLimitIn = 0;
	}
	if(ADC_GetTdsValueDisplay(TDS_OUT_VALUE) < g_userConfig.tdsLimitOut)
	{
		if(currentErrors[ERROR_TYPE_TDS_OUT])
		{
			currentErrors[ERROR_TYPE_TDS_OUT] = false;
		}
		timeTdsLimitOut = 0;
	}
#endif
}
/*
 * check dò nước cổng dò 1.
 * nếu nước rò, 10s sau kiểm tra lại, vẫn rò thì báo lỗi.
 *
 * */
void checkH2ODet()
{
	static bool waterIsLeak = false;
	static uint32_t timeStartLeak = 0;
	if(ADC_GetH2oDet(H2O_1) && (!currentErrors[ERROR_TYPE_LEAK_WATER]))
	{
		if(!waterIsLeak)
		{
			waterIsLeak = true;
			timeStartLeak = g_sysTime;
		}
		if(waterIsLeak && (elapsedTime(g_sysTime, timeStartLeak) >= 10000))
		{
			pumpControl_turnOffAll();
			newErrorOccur(ERROR_TYPE_LEAK_WATER);
		}
	}
	if(waterIsLeak && (elapsedTime(g_sysTime, timeStartLeak) >= 10000) && (!ADC_GetH2oDet(H2O_1)))
	{
		waterIsLeak = false;
	}
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
void ErrorCheck_init()
{
	for(int8_t i = 0; i<MAX_ERROR_NUM ; i++)
	{
		currentErrors[i] = false;
	}
}
void ErrorCheck_process()
{
//	checkFilter();
	checkWaterIn();
//	checkPumpRunTime();
#ifdef ENABLE_TDS_OUT
//	checkTdsLimit();
#endif

#ifndef DISABLE_H2O_DET
//	checkH2ODet();
#endif
	if((lastError == true) && (ErrorCheck_haveError()== false))
	{
		ErrorCheck_allErrorAreRemoved_cb();
	}
	lastError = ErrorCheck_haveError();

	if((currentErrors[ERROR_TYPE_INCOME_WATER_LOST] == true) && (elapsedTime(g_sysTime,s_lastTimeShowE4)> E4_SHOW_INTERVAL))
	{
		newErrorOccur(ERROR_TYPE_INCOME_WATER_LOST);
	}
    if(needShowErrorOccur && (!getWater_waterIsOuting() && !heatControl_waterIsOuting()))
    {
        if(ErrorCheck_haveError())
            ErroCheck_newError_cb(s_recentNewError);
        needShowErrorOccur = false;
    }
}
bool ErrorCheck_haveError()
{
	for(uint8_t i=0; i< MAX_ERROR_NUM; i++)
	{
		if(currentErrors[i] == true)
			return true;
	}
	return false;
}

ErrorType_t ErrorCheck_getNextError(ErrorType_t currentError)
{
	ErrorType_t nextError = currentError;
	while(1)
	{
		if(++nextError == MAX_ERROR_NUM)
			nextError = ERROR_TYPE_FILTER_0;
		if(currentErrors[nextError])
		{
			return nextError;
		}
	}
	return currentError;
}

bool ErrorCheck_getError(ErrorType_t type)
{
	return currentErrors[type];
}

bool ErrorCheck_haveErrorOtherErrorTds()
{
	for(uint8_t i=0; i< MAX_ERROR_NUM; i++)
	{
		if(i == ERROR_TYPE_TDS_OUT) continue;
		if(currentErrors[i] == true)
			return true;
	}
	return false;
}


uint32_t Error_getErrorBit()
{
    uint32_t ret = 0; uint8_t i;
    for(i=0; i< MAX_ERROR_NUM; i++)
    {
        if(currentErrors[i])
        {
            ret |= ((uint32_t)1<<i);
        }
    }
    return ret;
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


