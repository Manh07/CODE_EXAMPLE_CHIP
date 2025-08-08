/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************//*!
*
* @file        pumpControl.c
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

#include "pumpControl.h"
#include "gpio.h"
#include "timeCheck.h"
#include "filter_time.h"
#include "adc.h"
#include "user_config.h"
#include "updateParam.h"
#include "heatControl.h"
#include "getWater.h"
/******************************************************************************
* External objects
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
extern bool g_ioTestEn;
/******************************************************************************
* Constants and macros
******************************************************************************/


/******************************************************************************
* Local types
******************************************************************************/

/******************************************************************************
* Local function prototypes
******************************************************************************/
void processVanXa();
void processPump();

/******************************************************************************
* Local variables
******************************************************************************/
bool s_pumpIsOn = false;
uint32_t s_pumpStartTime = 0;
uint32_t timeOffVanXa;
bool vanXaIsOn = false;
bool turnOffAll = false;


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
void setOutput(Output_t out, bool isON)
{
	if(g_ioTestEn)
		return;
	switch (out) {
		case OUT_PUMP:
//			if(isON)
//				TURN_ON_PUMP;
//			else
//				TURN_OFF_PUMP;
			break;
		case OUT_VAN:
		//tam thoi dung chan van de dieu khien cai khac
//			if(isON)
//				TURN_ON_VAN_XA;
//			else
//				TURN_OFF_VAN_XA;
			break;
		default:
			break;
	}
    updateParam_statusSystem(out,(uint8_t)isON);
}
void processVanXa()
{
//	if(vanXaIsOn && timeIsAfter(g_sysTime , timeOffVanXa) )
//	{
//		setOutput(OUT_VAN, false);
//		vanXaIsOn = false;
//	}
}

//back lai neu co loi thi k tru time filter nua
void processFilterTime()
{
	static bool s_isRealPumpOn = false;
	static uint32_t s_lastUpdateFilterTime = 0;
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO)
	{
		if(!s_isRealPumpOn)
		{
			s_isRealPumpOn = true;
			s_lastUpdateFilterTime = g_sysTimeS;
		}
		// nếu thời gian bơm chạy lớn hơn 600s trừ thời gian chạy của lõi lọc.
		if(elapsedTime(g_sysTimeS, s_lastUpdateFilterTime) > 600)
		{
			uint32_t runTimeToUpdate = elapsedTime(g_sysTimeS , s_lastUpdateFilterTime);
			s_lastUpdateFilterTime = g_sysTimeS;
			filter_time_minusTime(runTimeToUpdate);
		}
	}
	else  // cần tắt bơm
	{
		if(s_isRealPumpOn) // nếu bơm đang bật thì tắt
		{
			s_isRealPumpOn = false;
			//trừ thời gian chạy của lõi lọc
			uint32_t runTimeToUpdate = elapsedTime(g_sysTimeS , s_lastUpdateFilterTime);
			filter_time_minusTime(runTimeToUpdate);
		}
	}
}
void processPump()
{
#ifdef ENABLE_FLOAT
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO && (!ADC_GetH2oDet(H2O_2)))
#else
	if(CHECK_CO_AP_THAP && CHECK_CO_AP_CAO)
#endif
	{
		if(!s_pumpIsOn)
		{
			s_pumpIsOn = true;
			setOutput(OUT_PUMP, true);
			s_pumpStartTime = g_sysTimeS;
			pumpControl_onVanXaInMs(15000);
		}
		if(pumpControl_getTimePumpRun() > UserConfig_getWaitTimeUpdateTds())
		{
			if( ADC_SetDisableUpdateTds(false))
			{
				ADC_UpdateTdsDisplay();
			}
		}
	}
	else  // cần tắt bơm
	{
		if(s_pumpIsOn) // nếu bơm đang bật thì tắt
		{
			s_pumpIsOn = false;
			// tắt bơm
			setOutput(OUT_PUMP, false);
			// bật van xả trong 5s
			pumpControl_onVanXaInMs(5000);
			// disable update TDS
			ADC_SetDisableUpdateTds(true);
		}
	}
}
/******************************************************************************
* Global functions
******************************************************************************/

void pumpControl_onVanXaInMs(uint32_t msTime)
{
//	if(turnOffAll)
//		return;
//	timeOffVanXa = g_sysTime + msTime;
//	setOutput(OUT_VAN, true);
//	vanXaIsOn = true;
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
void pumpControl_process()
{
//    if(heatControl_waterIsOuting() == true)
//    {
////    	if(!g_ioTestEn)
////    		TURN_ON_VAN_XA;
//    }
//    else
//    {
////    	if(!g_ioTestEn)
////    		TURN_OFF_VAN_XA;
//    }
	if(!turnOffAll)
	{
		processFilterTime();		//loi thi k tru time nua
		processPump();
		processVanXa();
	}
}
uint32_t pumpControl_getTimePumpRun()
{
	if(s_pumpIsOn)
	{
		return (elapsedTime(g_sysTimeS, s_pumpStartTime));
	}else{
		return 0;
	}
}

uint32_t pumpControl_getTimePumpRunHour()
{
	uint32_t time_hour = pumpControl_getTimePumpRun();
	if(time_hour > 0)
	{
		time_hour = time_hour/3600000;
	}
	return time_hour;
}


void pumpControl_turnOffAll()
{
//	setOutput(OUT_VAN, false);
//	setOutput(OUT_PUMP, false);
//	s_pumpIsOn = false;
//	vanXaIsOn = false;
//	turnOffAll = true;
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


