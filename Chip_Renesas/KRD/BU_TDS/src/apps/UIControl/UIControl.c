#include <config.h>
#include "UIControl.h"
#include "user_config.h"
#include "filter_time.h"
#include "timeCheck.h"
#include "errorCheck.h"
#include "adc.h"
#include "tools.h"
#include "filterExpire.h"
#include "CurentData.h"
#include "updateParam.h"
#include "heatControl.h"
#include "getWater.h"
#include "voiceCmd.h"
#include "flowSensor.h"
#include "uartHandle.h"
#include "gpio.h"
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



/******************************************************************************
* Local types
******************************************************************************/


typedef enum
{
	UI_WATER_STATE_RO = 0,
	UI_WATER_STATE_HOT,
	UI_WATER_STATE_COLD,
	UI_WATER_STATE_COOL,
	UI_WATER_STATE_MAX,
} UI_waterState_t;


typedef enum
{
	UI_HEATCONTROL_TEM,
	UI_HEATCONTROL_AMOUNT,
	UI_HEATCONTROL_WATER_INOUT,
	UI_HEATCONTROL_MAX
} ui_heatControl_t;

#define UI_WATER_STATE_DEFAULT UI_WATER_STATE_RO
#define UI_HEATCONTROL_DEFAULT UI_HEATCONTROL_TEM

/******************************************************************************
* Local function prototypes
******************************************************************************/



/******************************************************************************
* Local variables
******************************************************************************/
UI_waterState_t uiWaterState = UI_WATER_STATE_DEFAULT;
ui_heatControl_t uiHeatControl = UI_HEATCONTROL_DEFAULT;

uint32_t s_lastTimeWaterOut = 0;
bool s_needReset = false;
uint32_t s_timeReset;
bool s_pressToStopWater = false;
extern bool g_ioTestEn;
bool isVoiceCommand = false;
bool isEnableCheckStopWater = false;
uint32_t timeStartWaterOuting = 0;
/******************************************************************************
* Local functions
******************************************************************************/
void saveFilterLifeTime(uint8_t index)
{
	UserConfig_setFilterLifeTime(0,0/*(uint32_t)Led7seg_getNumberInLed4()*3600,index*/);
	filter_time_resetTimeAtIndex(index);
	filterExpire_resetTimeAtIndex(index);
	updateParam_setNeedUpdate(PARAM_TYPE_FILTERTOTAL);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEEXPIRE);
	updateParam_setNeedUpdate(PARAM_TYPE_TIMEFILTER);

}

uint16_t amountCheckStop = 0; uint8_t temCheckStop = 0;
void UIControl_checkStopWater()
{
    if(getWater_waterIsOuting() || heatControl_waterIsOuting())
    {
        s_lastTimeWaterOut = g_sysTime;
    }
    if(isEnableCheckStopWater)
    {
        if(getWater_waterIsOuting())
        {
//          if(flowSensor_getFlowCurrent() >= amountCheckStop)
            if(elapsedTime(g_sysTime, timeStartWaterOuting) > 120000)
            {
                getWater_controlTurnOffAllWater();
                isEnableCheckStopWater = false;
//                voiceCmd_sendStateWater(WATER_STATE_DONE);
            }
        }
        else
        {
            if(heatControl_getCurrentAmountWater() > 0 && !heatControl_waterIsOuting())
            {
                isEnableCheckStopWater = false;
//                voiceCmd_sendStateWater(WATER_STATE_DONE);
            }
        }

    }
}

enum
{
    VALUE_WATER_OUT_NONE = 0,
    VALUE_WATER_OUT_RO,
    VALUE_WATER_OUT_HOT,
    VALUE_WATER_OUT_COOL,
    VALUE_WATER_OUT_COLD,
    VALUE_WATER_OUT_HYDROGEN,
    VALUE_WATER_OUT_PH85,
    VALUE_WATER_OUT_PH90,
    VALUE_WATER_OUT_PH95,
};
uint8_t stateWaterCurrent = 0;
uint8_t xaSucTypeCurrent = 0;
extern bool needUpdateGetwater;
extern bool needUpAll;
extern bool h2Cap_isOn;
bool buStateCurrent = false;
void UIControl_sendStateWaterOut()
{
    uint8_t state = 0;

    if(getWater_getStateWaterIsOuting(WATER_TYPE_RO))
    {
        state = VALUE_WATER_OUT_RO;
    }
    else if(getWater_getStateWaterIsOuting(WATER_TYPE_COOL))
    {
        state = VALUE_WATER_OUT_COOL;
    }
    else if(getWater_getStateWaterIsOuting(WATER_TYPE_COLD))
    {
        state = VALUE_WATER_OUT_COLD;
    }
    else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH85))
   {
       state = VALUE_WATER_OUT_PH85;
   }
    else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH90))
   {
       state = VALUE_WATER_OUT_PH90;
   }
    else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
   {
       state = VALUE_WATER_OUT_PH95;
   }
    else if(heatControl_waterIsOuting())
    {
        state = VALUE_WATER_OUT_HOT;
    }
    else
    {
        state = VALUE_WATER_OUT_NONE;
    }
    char bufWater[25] = "";
    uint8_t dataGetwater[3];
	dataGetwater[0] = 0;
    dataGetwater[1] = 0;
    dataGetwater[2] = state;
	if(needUpdateGetwater)
	{
        uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_GET_WATER, (uint8_t*)dataGetwater, 3);
        needUpdateGetwater = false;
	}


    uint8_t dataXaSuc[2] ={0,0};
    if(needUpAll)
    {
        xaSucTypeCurrent = gpio_xaSucType();
        dataXaSuc[1] = xaSucTypeCurrent;
        uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_XA_SUC, (uint8_t*)dataXaSuc, 2);
        needUpAll = false;
    }
    else
    {
        if(xaSucTypeCurrent != gpio_xaSucType())
        {
            xaSucTypeCurrent = gpio_xaSucType();
            dataXaSuc[1] = xaSucTypeCurrent;
            uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_XA_SUC, (uint8_t*)dataXaSuc, 2);
        }
    }

    uint8_t dataBu[2] ={0,0};
    if(needUpAll)
    {
        buStateCurrent = h2Cap_isOn;
       dataBu[1] = buStateCurrent;
       uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_BU, (uint8_t*)dataBu, 2);
       needUpAll = false;
    }
    else
    {
       if(buStateCurrent != h2Cap_isOn)
       {
           buStateCurrent = h2Cap_isOn;
           dataBu[1] = buStateCurrent;
           uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_BU, (uint8_t*)dataBu, 2);
       }
    }

    static uint32_t timeGetWater = 0;
    if(stateWaterCurrent == VALUE_WATER_OUT_NONE)
    {
    	timeGetWater = g_sysTime;
    }
    if(state != stateWaterCurrent)
    {
    	uint8_t lastState = stateWaterCurrent;
    	uint32_t timeWaterOut = 0;
    	if(state == VALUE_WATER_OUT_NONE)
    	{
    		timeWaterOut = (uint32_t)(g_sysTime - timeGetWater);
    	}
        stateWaterCurrent = state;
        sprintf(bufWater,"[WATER_STATE,%d,%d,%d,%d]",stateWaterCurrent, isVoiceCommand, lastState, timeWaterOut);
        uartHandle_sendString(bufWater);
    }
    flowSensor_checkFlowRunning();
}

void UIControl_process()
{
    UIControl_sendStateWaterOut();
}

void UIControl_setUIVoiceCommandOnline(uint16_t cmd, uint16_t amount, uint8_t tem)
{
    s_lastTimeWaterOut = g_sysTime;
    isVoiceCommand = true;
        switch(cmd)
        {
            case VOICE_ONLINE_WATER_TYPE_HOT:
                if(!heatControl_waterIsOuting())
                {
                    getWater_controlTurnOffAllWater();
                    heatControl_requestWaterSettingVoiceCommandOnline(amount, tem);
                    heatControl_requestWaterOut();  //run
                }

                if(uiWaterState != UI_WATER_STATE_HOT)
                {
                    uiWaterState = UI_WATER_STATE_HOT;
                }
                uiHeatControl = UI_HEATCONTROL_WATER_INOUT;
                break;
            case VOICE_ONLINE_WATER_TYPE_COOL:
                if(heatControl_waterIsOuting())
                {
                    heatControl_requestWaterOut();  //STOP
                }
                getWater_controlGetWater(WATER_TYPE_COOL,true);
                if(uiWaterState != UI_WATER_STATE_COOL)
                {
                    uiWaterState = UI_WATER_STATE_COOL;
                }

                break;
            case VOICE_ONLINE_WATER_TYPE_COLD:
                if(heatControl_waterIsOuting())
                {
                    heatControl_requestWaterOut();  //STOP
                }
                getWater_controlGetWater(WATER_TYPE_COLD,true);
                if(uiWaterState != UI_WATER_STATE_COLD)
                {
                    uiWaterState = UI_WATER_STATE_COLD;
                }
                break;

            case VOICE_ONLINE_WATER_TYPE_RO:
                if(heatControl_waterIsOuting())
                {
                    heatControl_requestWaterOut();  //STOP
                }
                getWater_controlGetWater(WATER_TYPE_RO,true);
                if(uiWaterState != UI_WATER_STATE_RO)
                {
                    uiWaterState = UI_WATER_STATE_RO;
                }
                break;
        }
//      if(cmd != VOICE_ONLINE_WATER_TYPE_HOT )
//      {
            if(amount > 0 && tem > 0)
            {
                isEnableCheckStopWater = true;
                timeStartWaterOuting = g_sysTime;
                amountCheckStop = amount;
                temCheckStop = tem;
            }
            else if(amount == 0)
            {
                isEnableCheckStopWater = true;
                timeStartWaterOuting = g_sysTime;
                amountCheckStop = 1000;
            }
//      }

}

void UIControl_setUiVoiceCommandCancelWater()
{
    isEnableCheckStopWater = false;
    getWater_controlTurnOffAllWater();
    if(heatControl_waterIsOuting())
    {
        heatControl_requestWaterOut();  //stop
    }
}

void UIControl_setUIVoiceCommand(uint16_t cmd)
{
    static uint16_t lastestCmdWater = 0;
    static uint32_t lastestTimeCmdWater = 0;
    if(cmd == 0xFFFF) return;
    if(getWater_waterIsOuting() || heatControl_waterIsOuting())
    {
        if(!isVoiceCommand) return;
    }
    if(cmd == VOICE_CMD_WATER_TYPE_WAKEUP_CMD)
    {
        return;
    }

    if(cmd == VOICE_CMD_WATER_TYPE_STOP || cmd == VOICE_CMD_WATER_TYPE_STOP_2 || cmd == VOICE_CMD_WATER_TYPE_RESUME)
    {

    }
    else isVoiceCommand = true;
    switch (cmd) {
        case VOICE_CMD_WATER_TYPE_HOT:
        case VOICE_CMD_WATER_TYPE_CAFE:
        case VOICE_CMD_WATER_TYPE_TEA:
        case VOICE_CMD_WATER_TYPE_MILK:
            //
            if(!heatControl_waterIsOuting() && !getWater_waterIsOuting())
            {
//              getWater_controlTurnOffAllWater();
                heatControl_requestWaterSettingVoiceCommand((voiceCmd_waterType_t)cmd);
                heatControl_requestWaterOut();  //run

                if(uiWaterState != UI_WATER_STATE_HOT)
                {
                    uiWaterState = UI_WATER_STATE_HOT;
                }
                uiHeatControl = UI_HEATCONTROL_WATER_INOUT;
                lastestCmdWater = cmd;
            }

            break;
        case VOICE_CMD_WATER_TYPE_COLD:
//          if(heatControl_waterIsOuting())
//          {
//              heatControl_requestWaterOut();  //STOP
//          }
            if(!heatControl_waterIsOuting() && !getWater_waterIsOuting())
            {
                getWater_controlGetWater(WATER_TYPE_COLD,true);

                if(uiWaterState != UI_WATER_STATE_COLD)
                {
                    uiWaterState = UI_WATER_STATE_COLD;
                }
                lastestCmdWater = cmd;
            }
            break;
        case VOICE_CMD_WATER_TYPE_COOL:
//          if(heatControl_waterIsOuting())
//          {
//              heatControl_requestWaterOut();  //STOP
//          }
            if(!heatControl_waterIsOuting() && !getWater_waterIsOuting())
            {
                getWater_controlGetWater(WATER_TYPE_COOL,true);

                if(uiWaterState != UI_WATER_STATE_COOL)
                {
                    uiWaterState = UI_WATER_STATE_COOL;
                }
                lastestCmdWater = cmd;
            }
            break;
        case VOICE_CMD_WATER_TYPE_RO:
        case VOICE_CMD_WATER_TYPE_RO_200:
//          if(heatControl_waterIsOuting())
//          {
//              heatControl_requestWaterOut();  //STOP
//          }
            if(!heatControl_waterIsOuting() && !getWater_waterIsOuting())
            {
                getWater_controlGetWater(WATER_TYPE_RO,true);
                if(uiWaterState != UI_WATER_STATE_RO)
                {
                    uiWaterState = UI_WATER_STATE_RO;
                }
               lastestCmdWater = cmd;
            }
            break;
        case VOICE_CMD_WATER_TYPE_STOP_2:
        case VOICE_CMD_WATER_TYPE_STOP:
            if(!isVoiceCommand) break;
            if(getWater_waterIsOuting())
            {
                lastestTimeCmdWater = g_sysTime;
                getWater_controlTurnOffAllWater();
            }
            if(heatControl_waterIsOuting())
            {
                lastestTimeCmdWater = g_sysTime;
                heatControl_requestWaterOut();  //stop
            }

            break;
        case VOICE_CMD_WATER_TYPE_RESUME:
            if(!isVoiceCommand) break;
            if(elapsedTime(g_sysTime, lastestTimeCmdWater) < 5000)
            {
                if(lastestCmdWater == VOICE_CMD_WATER_TYPE_HOT || lastestCmdWater == VOICE_CMD_WATER_TYPE_MILK
                        || lastestCmdWater == VOICE_CMD_WATER_TYPE_CAFE || lastestCmdWater == VOICE_CMD_WATER_TYPE_TEA)
                {
                    if(!heatControl_waterIsOuting())
                    {
                        heatControl_requestWaterOut();  //run
                    }
                }
                else if(lastestCmdWater == VOICE_CMD_WATER_TYPE_RO ||
                        lastestCmdWater == VOICE_CMD_WATER_TYPE_RO_200)
                {
                    if(!getWater_waterIsOuting())
                    {
                        getWater_controlGetWater(WATER_TYPE_RO,true);
                    }
                }
                else if(lastestCmdWater == VOICE_CMD_WATER_TYPE_COLD)
                {
                    if(!getWater_waterIsOuting())
                    {
                        getWater_controlGetWater(WATER_TYPE_COLD,true);
                    }
                }
                else if(lastestCmdWater == VOICE_CMD_WATER_TYPE_COOL)
                {
                    if(!getWater_waterIsOuting())
                    {
                        getWater_controlGetWater(WATER_TYPE_COOL,true);
                    }
                }
            }
            break;
        default:
            isVoiceCommand = false;
            break;
    }
    if(cmd != VOICE_CMD_WATER_TYPE_STOP && cmd != VOICE_CMD_WATER_TYPE_STOP_2)
    {
        isEnableCheckStopWater = true;
        timeStartWaterOuting = g_sysTime;
        amountCheckStop = 1000;
        if(cmd == VOICE_CMD_WATER_TYPE_RO_200)
        {
            amountCheckStop = 200;
        }
        else if(cmd == VOICE_CMD_WATER_TYPE_RESUME)
        {
            if(lastestCmdWater == VOICE_CMD_WATER_TYPE_RO_200)
            {
                amountCheckStop = 200;
            }
        }
    }

}
// callback
void ErroCheck_newError_cb(ErrorType_t newError)
{
    if(newError == ERROR_TYPE_LEAK_WATER)
    {
        if(heatControl_waterIsOuting())
        {
            heatControl_requestWaterOut();  //stop
        }
        if(getWater_waterIsOuting() && heatControl_waterIsOuting())
        {
            getWater_controlTurnOffAllWater();
        }
    }
}

void ErrorCheck_allErrorAreRemoved_cb()
{

}
