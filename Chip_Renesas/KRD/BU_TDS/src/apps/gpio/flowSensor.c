/**
 ******************************************************************************
 * @file    flowSensor.c
 * @author  Makipos Co.,LTD.
 * @version 1.0
 * @date    Mar 13, 2021
 * @brief   
 * @history
 ******************************************************************************/
/*******************************************************************************
 * Include
 ******************************************************************************/
#include "flowSensor.h"
#include "timeCheck.h"
#include "Config_ICU.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/

 /*******************************************************************************
 * Variables
 ******************************************************************************/
#define VOL_CALCULATE 1.0
static uint32_t cntPulse = 0;
uint32_t timeHavePulse = 0;
uint32_t flowCurrent = 0;
uint32_t lastestFlow = 0;
volatile extern uint32_t g_sysTime;
bool flowIsRunning = false;
static uint32_t cntPulse_ll1 = 0;
float flowRate_ll1 = 0;
float vol = 0;
float prev_vol = 0;
float save_vol = VOL_CALCULATE;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
float custom_round(float num) {
    int decimal_part = (int)(num * 10);  // Lấy phần thập phân
    float scale = num - (decimal_part * 0.1);
    if (scale >= 0.05) {
        return (decimal_part * 0.1) + 0.1;              // Làm tròn lên
    } else {
        return num;     // Giữ nguyên
    }
}

void flowSensor_checkPulseInInterrupt()
{
	cntPulse_ll1++;
	timeHavePulse = g_sysTime;
	cntPulse++;
    flowIsRunning = true;
}
void flowSensor_checkFlowRunning()
{
    if(flowIsRunning && elapsedTime(g_sysTime, timeHavePulse) >= 3000)
	{
        flowIsRunning = false;
		cntPulse = 0;
	}
}
bool flowSensor_isRunning()
{
    return flowIsRunning;
}
void flowSensor_resetFlow()
{
    cntPulse = 0;
}
void flowSensor_updateFlowRate()
{
    static uint32_t timeUpdate = 0 ;
    if(elapsedTime(g_sysTime, timeUpdate) > 1000)
    {
        flowRate_ll1 = (float)cntPulse_ll1*1000/elapsedTime(g_sysTime, timeUpdate)/38; //L/min
        timeUpdate = g_sysTime;
        cntPulse_ll1 = 0;
    }
}
float flowSensor_getFlowRateCurrent_ll1()
{
    return flowRate_ll1;
}
bool flowSensor_checkRemain_ll1()
{
    if (flowIsRunning)
	{
		vol = custom_round((float)cntPulse/(38 * 60));
		if(vol - prev_vol >= 0.1)
		{
			prev_vol = vol;
			save_vol = save_vol - 0.1;
		}
		if(save_vol <= 0.1)
		{
			save_vol = VOL_CALCULATE;
            return true;
		}
	}
	else
	{
		vol = 0;
		prev_vol = 0;
	}
    return false;
}
uint16_t flowSensor_getFlowCurrent()
{
	flowCurrent = cntPulse * 44 /100;
	return flowCurrent;
}

void flowSensor_init()
{
	R_Config_ICU_IRQ7_Start();
}
/***********************************************/
