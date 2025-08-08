/**
 ******************************************************************************
 * @file    getWater.c
 * @author  Makipos Co.,LTD.
 * @version 1.0
 * @date    Jan 25, 2021
 * @brief   
 * @history
 ******************************************************************************/
/*******************************************************************************
 * Include
 ******************************************************************************/
#include "getWater.h"
#include "flowSensor.h"
#include "timeCheck.h"
#include "gpio.h"
#include "heatControl.h"
#include "adc.h"
#include "tools.h"
#include "uartVoice.h"
#include "updateParam.h"
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define MAX_DP_CURRENT 3000
#define MIN_DP_CURRENT  0
#define MIN_DP_PWM      0
#define MAX_DP_PWM      95
#define MIN_DP_ADC      0
#define MAX_DP_ADC      3690
 /*******************************************************************************
 * Variables
 ******************************************************************************/
extern bool needUpdateGetwater;
bool waterIsOuting[WATER_TYPE_MAX] = {false,false,false,false,false,false};
bool van2NeedOff = true;
bool van4NeedOff = true;
bool van6NeedOff = true;
bool pumpNeedOff = true;
extern bool g_ioTestEn;
waterType_t lastWaterTypeOut = WATER_TYPE_MAX;
uint16_t pwm_value = 0;
static bool isTypePh85 = false;
static bool isTypePh = false;
uint16_t PWM_PH85[5][2] =
{
//   {30,15},{40,15},{50,15},{60,15},{64,15},{80,15},
// {30,25},{40,25},{50,25},{60,25},{64,25},{80,25},
    // {30,18},{40,18},{50,18},{60,18},{64,18},{80,18},
		{65,14},{115,16},{165,21},{215,24}, {275,27},
};
uint16_t PWM_PH90[5][2] =
{
//    {30,40},{40,63},{50,67},{60,73},{64,80},{80,90},
//  {30,20},{40,20},{50,20},{60,20},{64,20},{80,20},
// {30,43},{40,43},{50,43},{60,43},{64,43},{80,43},
    // {30,30},{40,30},{50,30},{60,30},{64,30},{80,30},
		{65,27},{115,29},{165,34},{215,44}, {275,49},
};
uint16_t PWM_PH95[5][2] =
{
//    {30,90},{40,100},{50,100},{60,100},{64,100},{80,100},
// {30,45},{40,67},{50,85},{60,88},{64,88},{80,88},
//  {30,60},{40,60},{50,60},{60,60},{64,60},{80,60},
// {30,95},{40,95},{50,95},{60,95},{64,95},{80,95},
    // {30,64},{40,64},{50,64},{60,64},{64,64},{80,64},
		{65,51},{115,53},{165,59},{215,65}, {275,67},
};



uint16_t PWM_PH85_2[5][2] =
{
    // {30,46},{40,46},{50,29},{60,29},{64,29},{80,29},
// {30,25},{40,25},{50,25},{60,25},{64,25},{80,25},
    // {30,18},{40,18},{50,18},{60,18},{64,18},{80,18},
		{65,14},{115,16},{165,21},{215,24}, {275,27},
};
uint16_t PWM_PH90_2[5][2] =
{
//    {30,40},{40,63},{50,67},{60,73},{64,80},{80,90},
//  {30,54},{40,54},{50,54},{60,54},{64,54},{80,54},
// {30,43},{40,43},{50,43},{60,43},{64,43},{80,43},
    // {30,30},{40,30},{50,30},{60,30},{64,30},{80,30},
		{65,27},{115,29},{165,34},{215,44}, {275,49},
};
uint16_t PWM_PH95_2[5][2] =
{
//    {30,90},{40,100},{50,100},{60,100},{64,100},{80,100},
// {30,45},{40,67},{50,85},{60,88},{64,88},{80,88},
//  {30,93},{40,93},{50,93},{60,93},{64,93},{80,93},
// {30,95},{40,95},{50,95},{60,95},{64,95},{80,95},
    // {30,64},{40,64},{50,64},{60,64},{64,64},{80,64},
		{65,51},{115,53},{165,59},{215,65}, {275,67},
};
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
void getWater_initPin()
{
//    GPIO_setAsOutputPin(PORT_WATER_RO, PIN_WATER_RO);
//    GPIO_setAsOutputPin(PORT_WATER_COLD, PIN_WATER_COLD);
    getWater_controlTurnOffAllWater();
//    flowSensor_init();
}

void getWater_controlTurnOffAllWater()
{
    if(g_ioTestEn) return;
    if(PIN_LEVEL_GET_WATER == 1)
    {
        TURN_OFF_VDT_3;
        if(van2NeedOff)
        {
            TURN_OFF_VDT_2;
        }
        if(van4NeedOff)
        {
            TURN_OFF_VDT_4;
        }
        TURN_OFF_VDT_5;
        if(van6NeedOff)
        {
            TURN_OFF_VDT_6;
        }
        TURN_OFF_VDT_7;
        TURN_OFF_VDT_9;
    }
    else
    {
        TURN_ON_VDT_3;
        if(van2NeedOff)
        {
            TURN_ON_VDT_2;
        }
        if(van4NeedOff)
        {
            TURN_ON_VDT_4;
        }
        TURN_ON_VDT_5;
        if(van6NeedOff)
        {
            TURN_ON_VDT_6;
        }
        TURN_ON_VDT_7;
        TURN_ON_VDT_9;
    }
    userGpio_turnPwm(0);
	waterIsOuting[WATER_TYPE_COLD] = false;
	waterIsOuting[WATER_TYPE_RO] = false;
	waterIsOuting[WATER_TYPE_COOL] = false;
	waterIsOuting[WATER_TYPE_PH85] = false;
    waterIsOuting[WATER_TYPE_PH90] = false;
    waterIsOuting[WATER_TYPE_PH95] = false;
}

bool getWater_waterIsOuting()
{
	if(waterIsOuting[WATER_TYPE_COLD] || waterIsOuting[WATER_TYPE_RO] || waterIsOuting[WATER_TYPE_COOL] || waterIsOuting[WATER_TYPE_PH85] || waterIsOuting[WATER_TYPE_PH90]|| waterIsOuting[WATER_TYPE_PH95])
	{
		return true;
	}
	return false;
}
static bool isTable1 = true;
void getWater_controlPh(waterType_t waterType, bool isOn)
{
    if(!isOn)
    {
        pwm_value = 0;
        userGpio_turnPwm(0);
        return;
    }
    uint16_t p[5][2];


    if(flowSensor_getFlowRateCurrent_ll1() < 1)
    {
        if(waterType == WATER_TYPE_PH85) {
          memcpy(p, PWM_PH85, sizeof(PWM_PH85));
        }
        else if(waterType == WATER_TYPE_PH90) {
          memcpy(p, PWM_PH90, sizeof(PWM_PH90));
        }
        else if(waterType == WATER_TYPE_PH95) {
          memcpy(p, PWM_PH95, sizeof(PWM_PH95));
        }
        isTable1 = true;
    }
    else
    {
        if(waterType == WATER_TYPE_PH85) {
           memcpy(p, PWM_PH85_2, sizeof(PWM_PH85_2));
        }
        else if(waterType == WATER_TYPE_PH90) {
           memcpy(p, PWM_PH90_2, sizeof(PWM_PH90_2));
        }
        else if(waterType == WATER_TYPE_PH95) {
           memcpy(p, PWM_PH95_2, sizeof(PWM_PH95_2));
        }
        isTable1 = false;
    }

    if(waterType == WATER_TYPE_PH85 || waterType == WATER_TYPE_PH90 || waterType == WATER_TYPE_PH95) {
        uint8_t j;
        uint16_t tds_in = ADC_GetTdsValueDisplay(TDS_IN_VALUE);
        if(tds_in >= p[4][0])
        {
            pwm_value = p[4][1];
        }
        else if(tds_in <= p[0][0])
        {
            pwm_value = p[0][1];
        }
        else
        {
            for(j=0;j<4;j++)
            {
                // if((tds_in > p[j-1][0])&& (tds_in <= p[j][0]))
                // {
                //     pwm_value = map(tds_in,p[j-1][0],p[j][0],p[j-1][1],p[j][1]);
                //     break;
                // }
                if((tds_in >= p[j][0]) && (tds_in <= p[j+1][0]))
                {
                    pwm_value = p[j][1];
                    break;
                }
            }
        }
        if(isOn)
            userGpio_turnPwm((uint8_t)pwm_value);
    }

}
void getWater_controlGetWater(waterType_t waterType, bool isOn)
{
    if(g_ioTestEn) return;
	getWater_controlTurnOffAllWater(); //should get only type one time
	if(waterType == WATER_TYPE_COOL)
	{
		waterIsOuting[waterType] = isOn;
//		R_GPIO_PinWrite(PIN_WATER_COLD, (PIN_LEVEL_GET_WATER == 1) ? (gpio_level_t)isOn : (gpio_level_t)!isOn);
//		R_GPIO_PinWrite(PIN_WATER_RO, (PIN_LEVEL_GET_WATER == 1) ? (gpio_level_t)isOn : (gpio_level_t)!isOn);
		return;
	}

    gpio_port_pin_t pin;
    if(waterType == WATER_TYPE_RO) {
        pin = PIN_WATER_RO;
        TURN_ON_VDT_5;
        TURN_ON_VDT_8;

    }
    else if(waterType == WATER_TYPE_COLD) {
        pin = PIN_WATER_COLD;
        TURN_ON_VDT_5;
	    TURN_ON_VDT_8;
    }
    else if(waterType == WATER_TYPE_PH85) {
        pin = PIN_WATER_PH;
        isTypePh85 = true;
        isTypePh = true;
        TURN_ON_VDT_3;
		TURN_ON_VDT_5;
        TURN_ON_VDT_7;
        TURN_ON_VDT_9;

    }else if(waterType == WATER_TYPE_PH90) {
        pin = PIN_WATER_PH;
        isTypePh = true;
        TURN_ON_VDT_3;
		TURN_ON_VDT_5;
        TURN_ON_VDT_7;
        TURN_ON_VDT_9;

    }else if(waterType == WATER_TYPE_PH95) {
        pin = PIN_WATER_PH;
        isTypePh = true;
        TURN_ON_VDT_3;
		TURN_ON_VDT_5;
        TURN_ON_VDT_7;
        TURN_ON_VDT_9;
    }
	else return;

    if(PIN_LEVEL_GET_WATER == 1)
    {
        if(isOn)
        {
            if(waterType != WATER_TYPE_PH85)
            	R_GPIO_PinWrite(pin,GPIO_LEVEL_HIGH);
//            GPIO_setOutputHighOnPin(port2, pin2);
//            if(waterType == WATER_TYPE_RO)
//			{
//            	TURN_ON_VDT_5;
//			}
        }
        else
        {
            pwm_value = 0;
            userGpio_turnPwm(0);
            R_GPIO_PinWrite(pin,GPIO_LEVEL_LOW);
//            GPIO_setOutputLowOnPin(port2, pin2);
//            if(waterType == WATER_TYPE_RO)
//			{
//            	TURN_OFF_VDT_5;
//			}
        }
    }
    else
    {
        if(isOn)
        {
            if(waterType != WATER_TYPE_PH85)
            	 R_GPIO_PinWrite(pin,GPIO_LEVEL_LOW);
//            GPIO_setOutputLowOnPin(port2, pin2);
//            if(waterType == WATER_TYPE_RO)
//            {
//            	TURN_OFF_VDT_5;
 //           }
        }
        else
        {
            pwm_value = 0;
            userGpio_turnPwm(0);
            R_GPIO_PinWrite(pin,GPIO_LEVEL_HIGH);
//            GPIO_setOutputHighOnPin(port2, pin2);
//            if(waterType == WATER_TYPE_RO)
//			{
//				TURN_ON_VDT_5;
//			}
        }
    }
	waterIsOuting[waterType] = isOn;
	if(isOn)
	{
	    lastWaterTypeOut = waterType;
	}

}
extern bool needDelayXaSucWhenWaterRunning;
void getWater_needDpAfterStopWater()
{
	if(needDelayXaSucWhenWaterRunning)
	{
		gpio_setNeedXaSucInterval();
		lastWaterTypeOut = WATER_TYPE_MAX;
		return;
	}
    if(lastWaterTypeOut == WATER_TYPE_PH85 || lastWaterTypeOut == WATER_TYPE_PH90 || lastWaterTypeOut == WATER_TYPE_PH95)
    {
        gpio_setNeedXaSucFollowFlowSensor();
    }
    else {
        gpio_setResumePauseXaSucAfterGetWater(true, false); // tiep tuc xa suc not qua trinh truoc do
    }
    lastWaterTypeOut = WATER_TYPE_MAX;
}

void getWater_toggleWater(waterType_t waterType)
{
    if(g_ioTestEn)
    {
        return;
    }
    waterIsOuting[waterType] = !waterIsOuting[waterType];
    getWater_controlGetWater(waterType, waterIsOuting[waterType]);
}

bool getWater_isAlkalineOuting()
{
    if(getWater_getStateWaterIsOuting(WATER_TYPE_PH85) || getWater_getStateWaterIsOuting(WATER_TYPE_PH90) || getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
    {
        return true;
    }
    return false;
}
bool getWater_isOutingButNotAlkalineOuting()
{
    if(getWater_getStateWaterIsOuting(WATER_TYPE_RO) || getWater_getStateWaterIsOuting(WATER_TYPE_COLD) || getWater_getStateWaterIsOuting(WATER_TYPE_COOL) || heatControl_waterIsOuting())
    {
        return true;
    }
    return false;
}
bool getWater_getStateWaterIsOuting(waterType_t waterType)
{
    return waterIsOuting[waterType];
}
extern bool needXaBeforeGetWater;
bool needTurnXa2sAlkaline = false;
bool isCheckForMcuReset = true;
bool checkGetRoWater = false;
void getWater_checVdt5GetPh()
{
	if(g_ioTestEn)
	{
		return;
	}
    static uint32_t lastTimePhOut = 0;
    static uint32_t lastTimePhOff = 0;
    static bool isTurnPh = false;
    static bool isFlowCheck = false;
    static bool isErrTds = false;
    static bool isCurrentCheck = false;

    static uint32_t lastTimeRoOut = 0;

    if(getWater_getStateWaterIsOuting(WATER_TYPE_RO) || getWater_getStateWaterIsOuting(WATER_TYPE_COLD))
    {
    	lastTimeRoOut = g_sysTime;
        checkGetRoWater = true;
        if(getWater_getStateWaterIsOuting(WATER_TYPE_RO)){
            if(van2NeedOff){
                van2NeedOff = false;
            }
        }
        else if(getWater_getStateWaterIsOuting(WATER_TYPE_COLD)){
            if(van4NeedOff){
                van4NeedOff = false;
            }
        }
    }
    else {
    	if((uint32_t)(g_sysTime - lastTimeRoOut) > 420000)
    	{
    		//7 phut
    		if(!getWater_getStateWaterIsOuting(WATER_TYPE_PH85) && !getWater_getStateWaterIsOuting(WATER_TYPE_PH90) && !getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
    		{
    			TURN_OFF_VDT_8;
    		}
    	}
        else if((uint32_t)(g_sysTime - lastTimeRoOut) > 500)
        {
            if(!van2NeedOff)
            {
                van2NeedOff = true;
                TURN_OFF_VDT_2;
            }
            else if(!van4NeedOff)
            {
                van4NeedOff = true;
                TURN_OFF_VDT_4;
            }
        }
    }

    if(!getWater_getStateWaterIsOuting(WATER_TYPE_PH85) && !getWater_getStateWaterIsOuting(WATER_TYPE_PH90) && !getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
    {
        if(g_sysTime < 420000){
            if(!checkGetRoWater)
            {
                TURN_OFF_VDT_8;
            }
        }
        if((uint32_t)(g_sysTime - lastTimePhOff) < 3000)
        {
            if(!CHECK_CO_AP_CAO){
                TURN_ON_VDT_9;
                pumpNeedOff = false;
            }
        }
        else{
            if(!pumpNeedOff && !getWater_getStateWaterIsOuting(WATER_TYPE_RO) && !getWater_getStateWaterIsOuting(WATER_TYPE_COLD))
            {
                pumpNeedOff = true;
                TURN_OFF_VDT_9;
            }
        }
        if((uint32_t)(g_sysTime - lastTimePhOff) > 500)
        {
            if(!van6NeedOff){
                van6NeedOff = true;
                TURN_OFF_VDT_6;
            }
        }
        lastTimePhOut = g_sysTime;
        isTurnPh = false;
        isFlowCheck = false;
        isErrTds = false;
        isCurrentCheck = false;
        isTypePh85 = false;
        isTypePh = false;
    }
    else
    {
        lastTimePhOff = g_sysTime;
        if(van6NeedOff)
        {
            van6NeedOff = false;
        }
        if(!isFlowCheck && !isErrTds){
            getWater_controlPh(lastWaterTypeOut, true);
        }
    	TURN_ON_VDT_8;
        needXaBeforeGetWater = false;
        if(isCheckForMcuReset && g_sysTime < 2000) //do loi mcu dk reset, nen mach display ngay lap tuc yeu cau lay nuoc tiep
        {
            if(lastWaterTypeOut == WATER_TYPE_PH85)
            {
                if(isTypePh85)
                {
                    isTypePh85 = false;
                    R_GPIO_PinWrite(PIN_WATER_PH,GPIO_LEVEL_HIGH);
                }
            }
            lastTimePhOut = (uint32_t)(g_sysTime - (uint32_t)1000);
        }
        isCheckForMcuReset = false;
        if(isTypePh && needTurnXa2sAlkaline)
        {
            // TURN_ON_VDT_6;
            if(elapsedTime(g_sysTime, lastTimePhOut) > 2000)
            {
                needTurnXa2sAlkaline = false;
                TURN_OFF_VDT_2;
                TURN_OFF_VDT_5;
                TURN_OFF_VDT_6;
                // TURN_ON_VDT_4;
                isTypePh = false;
                lastTimePhOut = (uint32_t)(g_sysTime - (uint32_t)1000);
            }
            return;
        }
        if(lastWaterTypeOut == WATER_TYPE_PH85)
        {
            if(isTypePh85)
            {
                // if(elapsedTime(g_sysTime, lastTimePhOut) > 2300)
                // {
                    isTypePh85 = false;
                    R_GPIO_PinWrite(PIN_WATER_PH,GPIO_LEVEL_HIGH);
                    // lastTimePhOut = (uint32_t)(g_sysTime - (uint32_t)1000);
                    // isTurnPh = false;
                // }
                // if(elapsedTime(g_sysTime, lastTimePhOut) > 2000)
                // {
                //     getWater_setCurrent(0);
                //     TURN_OFF_SWAP;
                //     TURN_OFF_VDT_8;
                // }
                // else
                // {
                //     if(!isTurnPh)
                //     {
                //         isTurnPh = true;
                //         getWater_setCurrent(2500);
                //     }
                //     TURN_ON_SWAP;
                // }
                return;
            }

        }
        if(elapsedTime(g_sysTime, lastTimePhOut) > 4000)
        {
            //check flow
            if(flowSensor_getFlowRateCurrent_ll1() < 0.4)
            {
              if(!isFlowCheck)   //tat 1 lan
              {
                 getWater_controlPh(WATER_TYPE_PH85, false);
                  isFlowCheck = true;
                  //canh bao nuoc yeu
                  uint8_t data[2] = {2,0};
                 uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_NOTIFY,data,2);
              }
            }
            // if(pwm_value)
            // {
            //     if(isTable1 && (flowSensor_getFlowRateCurrent_ll1() > 0.75))
            //     {
            //         getWater_controlPh(lastWaterTypeOut, true);
            //     }
            //     else if(!isTable1 && (flowSensor_getFlowRateCurrent_ll1() < 0.75))
            //     {
            //         getWater_controlPh(lastWaterTypeOut, true);
            //     }
            // }
        }
//         else if(elapsedTime(g_sysTime, lastTimePhOut) > 4000)
//         {
//             uint16_t current = getWater_lockupCurrentFromAdc(userAdc_calculateFB());
//             if(!isCurrentCheck)
//             {
//                 isCurrentCheck = true;
//                 if((current < 300) && (pwm_value != 0))
//                 {
//                     uint8_t data[2] = {3,0};
//                     uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_NOTIFY,data,2);
//                     if(getWater_getStateWaterIsOuting(WATER_TYPE_PH85))
//                     {
//                         getWater_controlPh(WATER_TYPE_PH85, false); //chi can off PH
// //                        getWater_controlGetWater(WATER_TYPE_PH85, false);
//                     }
//                     else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH90))
//                     {
//                          getWater_controlPh(WATER_TYPE_PH85, false);
// //                        getWater_controlPh(WATER_TYPE_PH85, true);
// //                        getWater_controlGetWater(WATER_TYPE_PH90, false);
//                     }
//                     else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
//                     {
//                         getWater_controlPh(WATER_TYPE_PH85, false);
// //                        getWater_controlGetWater(WATER_TYPE_PH95, false);
//                     }
//                     needUpdateGetwater = true;

//                 }
//             }
//         }
        else if(elapsedTime(g_sysTime, lastTimePhOut) > 3000)
        {
            if(!isTurnPh)
            {
                if(ADC_GetTdsValueDisplay(TDS_IN_VALUE) > 300)
                {
                    //bao tds cao
                    isErrTds = true;
                    getWater_controlPh(WATER_TYPE_PH85, false);
                    uint8_t data[2] = {1,0};
                    uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_NOTIFY,data,2);
                }
                 else if(ADC_GetTdsValueDisplay(TDS_IN_VALUE) < 30)
                 {
                     //bao tds thap
                     isErrTds = true;
                     getWater_controlPh(WATER_TYPE_PH85, false);
                     uint8_t data[2] = {4,0};
                     uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_NOTIFY,data,2);
                 }
                else
                {
                    isErrTds = false;
                    // if(getWater_getStateWaterIsOuting(WATER_TYPE_PH85))
                    // {
                    //     getWater_controlPh(WATER_TYPE_PH85, true);
                    // }
                    // else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH90))
                    // {
                    //     getWater_controlPh(WATER_TYPE_PH90, true);
                    // }
                    // else if(getWater_getStateWaterIsOuting(WATER_TYPE_PH95))
                    // {
                    //     getWater_controlPh(WATER_TYPE_PH95, true);
                    // }
                }
                isTurnPh = true;
            }
        }
        // else if(elapsedTime(g_sysTime, lastTimePhOut) > 500)
        // {
        //     TURN_ON_VDT_6;
        // }
    }
}

uint8_t getWater_lockupPwmFromCurrent(uint16_t current)
{
    uint8_t pwm = 0;
    if(current >= MAX_DP_CURRENT)
    {
        return MAX_DP_PWM;
    }
    if(current == 0)
    {
        return 0;
    }
    pwm = map(current, MIN_DP_CURRENT, MAX_DP_CURRENT, MIN_DP_PWM, MAX_DP_PWM);
    return pwm;
}
uint16_t getWater_lockupCurrentFromAdc(uint16_t adc)
{
    uint16_t current = 0;
    if(adc >= MAX_DP_ADC)
    {
       return MAX_DP_CURRENT;
    }
    if(adc == 0)
    {
        return 0;
    }
    current = map(adc, MIN_DP_ADC, MAX_DP_ADC, MIN_DP_CURRENT, MAX_DP_CURRENT);
    return current;
}

void getWater_setCurrent(uint16_t current)
{
    uint8_t pwm = getWater_lockupPwmFromCurrent(current);
    userGpio_turnPwm(pwm);
}


uint32_t flowToEsp = 0;
uint32_t currentToEsp = 0;
void getWater_checkUpdateValueToEsp()
{
	static uint32_t lastTimeUpdateFlow = 0;
	static uint32_t lastTimeUpdateCurrent = 0;
	if(flowSensor_getFlowRateCurrent_ll1() > 0.35)
	{
		if(elapsedTime(g_sysTime, lastTimeUpdateFlow) > 10000)
		{
			flowToEsp = (uint32_t)(flowSensor_getFlowRateCurrent_ll1()*1000);
			updateParam_setNeedUpdate(PARAM_TYPE_TEMPHOT);//tam thoi update vao property nay
			lastTimeUpdateFlow = g_sysTime;
		}
	}
	else
	{
		lastTimeUpdateFlow = g_sysTime - 10000;
	}

	 uint16_t current = getWater_lockupCurrentFromAdc(userAdc_calculateFB());
	if(current > 300)
	{
		if(elapsedTime(g_sysTime, lastTimeUpdateCurrent) > 10000)
		{

			currentToEsp = current;
			updateParam_setNeedUpdate(PARAM_TYPE_TEMPCOLD);	//tam thoi update vao property nay
			lastTimeUpdateCurrent = g_sysTime;
		}
	}
	else
	{
		lastTimeUpdateCurrent = g_sysTime - 10000;
	}

}
uint32_t getWater_getValueFlowToEsp()
{
	return flowToEsp;
}
uint32_t getWater_getValueCurrentToEsp()
{
	 return (uint32_t)currentToEsp;
}
/***********************************************/
