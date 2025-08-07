/*
 * thermo.c
 *
 *  Created on: Jul 31, 2021
 *      Author: TranQuangThai
 */
#include "thermo.h"
#include "stdlib.h"
#include "string.h"
#include "config.h"
#include "uart.h"
#include "updateParam.h"
#include "timeCheck.h"
#include "adc.h"
int canValue = 0;
bool readThermoAvailable = false;
uint32_t lastTimeAvailable = 0;
bool old_coldBlockEnable = false;
bool old_hotBlockEnable = false;
uint16_t old_temHotValue = 0;
uint16_t old_temColdValue = 0;
bool needUpTank = false;
bool needUpTankHot = false;
bool needUpTankCold = false;

bool coldBlockEnable = true;
bool hotBlockEnable = true;
uint16_t temHotValue = 0;  //*10
uint16_t temColdValue = 0;  //*10
uint8_t coldSettingValue = 0;
void thermo_readThermoValue()
{
    if(g_sysTime < 5000)
    {
    	return;
    }
    if(elapsedTime(g_sysTime, lastTimeAvailable) > 100)
    {
        lastTimeAvailable = g_sysTime;
        temHotValue = userAdc_getTemNong();
        temColdValue = userAdc_getTemLanh();
    //        if(((temHotValue + 50) < old_temHotValue) || ((old_temHotValue + 50) < temHotValue))
        //        {
        //            updateParam_setNeedUpdate(PARAM_TYPE_TEMPHOT);
        //        }
        //        if(((temColdValue + 2) < old_temColdValue) || ((old_temColdValue + 2) < temColdValue))
        //        {
        //            updateParam_setNeedUpdate(PARAM_TYPE_TEMPCOLD);
        //        }
        temHotValue = temHotValue/10;
        temHotValue = temHotValue - temHotValue%5;      //5,10,15,20...
        temColdValue = temColdValue/10;
        if(temColdValue < 5) temColdValue = 0;
        else if(temColdValue < 7) temColdValue = 5;
        else if(temColdValue < 10) temColdValue = 7;
        else if(temColdValue < 12) temColdValue = 10;
        else if(temColdValue < 15) temColdValue = 12;
        else if(temColdValue < 17) temColdValue = 15;
        else if(temColdValue < 20) temColdValue = 17;
        else temColdValue = 20;

        if(readThermoAvailable == false)
        {
            readThermoAvailable = true;
            updateParam_setNeedUpdate(PARAM_TYPE_TEMPCOLD);
            updateParam_setNeedUpdate(PARAM_TYPE_TEMPHOT);
            updateParam_setNeedUpdate(PARAM_TYPE_CTRLHOT);
            updateParam_setNeedUpdate(PARAM_TYPE_CTRLCOLD);
            old_temHotValue = temHotValue;
            old_temColdValue = temColdValue;
            old_hotBlockEnable = hotBlockEnable;
            old_coldBlockEnable = coldBlockEnable;
        }
        else
        {
            if(old_temHotValue != temHotValue)
            {
                old_temHotValue = temHotValue;
                updateParam_setNeedUpdate(PARAM_TYPE_TEMPHOT);
            }
            if(old_temColdValue != temColdValue)
            {
                old_temColdValue = temColdValue;
                updateParam_setNeedUpdate(PARAM_TYPE_TEMPCOLD);
            }

            if((hotBlockEnable != old_hotBlockEnable) || needUpTankHot)
            {
               needUpTankHot = false;
               old_hotBlockEnable = hotBlockEnable;
               updateParam_setNeedUpdate(PARAM_TYPE_CTRLHOT);
            }
            if((coldBlockEnable != old_coldBlockEnable) || needUpTankCold)
            {
               needUpTankCold = false;
               old_coldBlockEnable = coldBlockEnable;
               updateParam_setNeedUpdate(PARAM_TYPE_CTRLCOLD);
            }
        }
    }
}

uint8_t thermo_getTemHot()
{
    return temHotValue;
}

uint8_t thermo_getTemCold()
{
    return temColdValue;
}


bool thermo_getTankHotIsEnable()
{
    return hotBlockEnable;
}
bool thermo_getTankColdIsEnable()
{
    return coldBlockEnable;
}
extern bool isEnableHot;
extern bool isEnableCold;
void thermo_controlEnableTankHot(bool isEnable)
{
    hotBlockEnable = isEnable;
    isEnableHot = isEnable;
    needUpTankHot = true;
}

void thermo_controlEnableTankCold(bool isEnable)
{
    coldBlockEnable = isEnable;
    isEnableCold = isEnable;
    needUpTankCold = true;
}
void thermo_controlTank(bool isHotEnable, bool isColdEnable)
{
    if(isHotEnable != hotBlockEnable)
    {
        thermo_controlEnableTankHot(isHotEnable);
    }
    else if(isColdEnable != coldBlockEnable)
    {
        thermo_controlEnableTankCold(isColdEnable);
    }
    needUpTank = true;
}
