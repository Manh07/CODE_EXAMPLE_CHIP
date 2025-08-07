/**
 ******************************************************************************
 * @file    heatControl.c
 * @author  Makipos Co.,LTD.
 * @version 1.0
 * @date    Jan 8, 2021
 * @brief   
 * @history
 ******************************************************************************/
/*******************************************************************************
 * Include
 ******************************************************************************/
#include "heatControl.h"
#include "timeCheck.h"
#include "tools.h"
#include "uart.h"
#include "UIControl.h"
#include "updateParam.h"
#define TEM_SETTING_DEFAULT TEM_IDX_SETTING_45
#define AMOUNT_SETTING_DEFAULT AMOUNT_IDX_SETTING_900
#define TIME_INTERVAL_SEND_HEAT_MODULE 200

/*******************************************************************************
 * Definitions
 ******************************************************************************/
uint8_t s_temSettingList[TEM_IDX_SETTING_MAX] = {TEM_SETTING_RO,45,75,99};
uint16_t s_amountWaterSettingList[AMOUNT_IDX_SETTING_MAX] = {180, 300,1000};

uint8_t temVirtual = 0;
uint16_t amountVirtual = 0;
 /*******************************************************************************
 * Variables
 ******************************************************************************/
uint8_t s_idxTemSettingDefault = TEM_IDX_SETTING_99;
uint8_t s_idxAmountSettingDefault = AMOUNT_IDX_SETTING_900;

uint8_t s_currentIdxTemSetting = TEM_SETTING_DEFAULT;
uint16_t s_currenIdxAmountSetting = AMOUNT_SETTING_DEFAULT;

uint16_t s_currentAmountWaterOut = 5;
uint8_t s_currentTemWaterOut = 0;

uint32_t s_totalAmountToday = 0;
uint8_t byteControlWater = BYTE_CONTROL_WATER_OFF;

bool s_requestWaterOut = false;
bool s_requestWaterOff = false;

bool s_requestWaterRO = false;

heatControl_state_t s_heatControlState = HEATCONTROL_STATE_NONE;

bool water_isOuting = false;

uint16_t heatModule_error = 0;

uint16_t cntErrUartTimeOut = 5;

bool heatErrList[HEAT_ERR_MAX] = {0};

heatControl_error_t lastestErr = HEAT_ERR_MAX;

heatControl_error_t priorityErr[HEAT_ERR_MAX] = {
		HEAT_ERR_Eu,
		HEAT_ERR_H1,
		HEAT_ERR_H2,
		HEAT_ERR_E1,
		HEAT_ERR_E2,
		HEAT_ERR_E9,
		HEAT_ERR_E5,
		HEAT_ERR_H4,
		HEAT_ERR_H3,
		HEAT_ERR_H6,
		HEAT_ERR_H5,
		HEAT_ERR_E7,
		HEAT_ERR_Eb};

static uint32_t timeLastCheck = 0;
extern volatile uint32_t g_sysTime;
extern bool g_ioTestEn;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/
uint8_t heatControl_getSettingTemDefaultIdx()
{
    if(s_idxTemSettingDefault >= 1) return s_idxTemSettingDefault - 1;
    else return 0;
}
uint8_t heatControl_getSettingAmountDefaultIdx()
{
    return s_idxAmountSettingDefault;
}
void heatControl_setSettingDefaultIdx(uint8_t idxTem, uint8_t idxAmount)
{
    if(idxTem > 2 || idxAmount > 2)
        return;
        //idxTem, idxAMount: 0/1/2
    s_idxTemSettingDefault = idxTem + 1;
    s_idxAmountSettingDefault = idxAmount;
    updateParam_setNeedUpdate(PARAM_TYPE_CTRLDISP);
}
uint8_t heatControl_getTemSetting()
{
	if(temVirtual != s_temSettingList[s_currentIdxTemSetting]) return temVirtual;
	return s_temSettingList[s_currentIdxTemSetting];
}
uint16_t heatControl_getAmountSetting()
{
	if(amountVirtual != s_amountWaterSettingList[s_currenIdxAmountSetting]) return amountVirtual;
	return s_amountWaterSettingList[s_currenIdxAmountSetting];
}

uint16_t heatControl_getCurrentAmountWater()
{
	return s_currentAmountWaterOut;
}
void heatControl_settingTemNext()
{
	if(++s_currentIdxTemSetting == TEM_IDX_SETTING_MAX)
	{
		s_currentIdxTemSetting = TEM_IDX_SETTING_45;
	}
	temVirtual = s_temSettingList[s_currentIdxTemSetting];
}

void heatControl_settingAmountNext()
{
	s_currenIdxAmountSetting++;
	if(s_currenIdxAmountSetting == AMOUNT_IDX_SETTING_MAX)
	{
		s_currenIdxAmountSetting = AMOUNT_IDX_SETTING_MIN;
	}
	amountVirtual = s_amountWaterSettingList[s_currenIdxAmountSetting];
}

void heatControl_requestWaterOut()
{
	if(!water_isOuting)
	{
		s_currentAmountWaterOut = 0;
		s_requestWaterOut = true;
	}
	else
		s_requestWaterOff = true;
}
void heatControl_requestWaterRO()
{
	s_currentIdxTemSetting = TEM_IDX_SETTING_RO;
}
void heatControl_requestWaterSettingDefault()
{
	s_currentIdxTemSetting = s_idxTemSettingDefault;
	s_currenIdxAmountSetting = s_idxAmountSettingDefault;
	temVirtual = s_temSettingList[s_currentIdxTemSetting];
	amountVirtual = s_amountWaterSettingList[s_currenIdxAmountSetting];
}

void heatControl_requestWaterSettingVoiceCommand(voiceCmd_waterType_t type)
{
	if(type == VOICE_CMD_WATER_TYPE_HOT)
	{
		s_currentIdxTemSetting = TEM_IDX_SETTING_99;
		temVirtual = s_temSettingList[s_currentIdxTemSetting];
	}
	else if(type == VOICE_CMD_WATER_TYPE_CAFE)
	{
		s_currentIdxTemSetting = TEM_IDX_SETTING_99;
		temVirtual = s_temSettingList[s_currentIdxTemSetting];
	}
	else if(type == VOICE_CMD_WATER_TYPE_TEA) //->nuoc nong
	{
		temVirtual = 70;
	}

	else if(type == VOICE_CMD_WATER_TYPE_MILK)
	{
        s_currentIdxTemSetting = TEM_IDX_SETTING_45;
        temVirtual = s_temSettingList[s_currentIdxTemSetting];
	}
	else
	{
		return;
	}
//	s_currenIdxAmountSetting = AMOUNT_IDX_SETTING_900;
	amountVirtual = 1000;//s_amountWaterSettingList[s_currenIdxAmountSetting];
}
void heatControl_requestWaterSettingVoiceCommandOnline(uint16_t amount, uint8_t tem)
{
	amountVirtual = amount;
	temVirtual = tem;
}
bool heatControl_waterIsOuting()
{
	return water_isOuting;
}
bool heatControl_isROState()
{
	return s_requestWaterRO;
}


void heatControl_getCode7SegFromErr(uint8_t* code1Idx, uint8_t* code2Idx, uint8_t errorShow)
{

}

bool heatControl_haveError()
{
	for(uint8_t i = 0; i < HEAT_ERR_MAX; i++)
	{
		if(heatErrList[i] == true)
		{
			return true;
		}
	}
	return false;
}
void heatControl_decodeError(uint16_t errByte)
{
	uint8_t error_bit_idx_in_byte[HEAT_ERR_MAX - 1] = {0,1,2,4,5,6,7,8,9,12,13,14}; //k tinh loi Eu
	for(uint8_t i = 0; i < HEAT_ERR_MAX - 1; i++)
	{
		if((errByte & ((uint16_t)1 << error_bit_idx_in_byte[i])) != 0)
		{
			if(heatErrList[i] == false)
			{
				heatErrList[i] = true;
				lastestErr = (heatControl_error_t)i;
			}
		}
		else
		{
			heatErrList[i] = false;
		}
	}
}


void heatControl_decodeResponeInquire(uint8_t* frame, uint16_t len)
{
	uint16_t cks = tools_calCheckSum2Byte(frame, len - 2);
	if((cks & 0x00FF) != frame[len-1]) return;
	if((cks>>8) != frame[len-2]) return;

	if(frame[1] == 1) water_isOuting = true;
	else water_isOuting = false;

	if(water_isOuting == true)
	{
		s_currentTemWaterOut = frame[2];
		s_currentAmountWaterOut = (uint16_t)frame[3]<<8 | frame[4];
	}
	s_totalAmountToday = (uint32_t)frame[5]<<24 | (uint32_t)frame[6]<<16 |  (uint32_t)frame[7]<<8 | (uint32_t)frame[8];

	//error: byte 9 10
	heatModule_error = (uint16_t)frame[9]<<8 | frame[10];
	heatControl_decodeError(heatModule_error);
	cntErrUartTimeOut = 5;
	heatErrList[HEAT_ERR_Eu] = false;
}
void heatControl_sendMeassageControl()
{
	uint8_t frame[19];
	memset((void*)frame,0,19);
	frame[0] = 0xAA;
	frame[2] = byteControlWater;
	frame[3] = heatControl_getTemSetting();
    if(frame[3] == 75)
    {
        frame[3] = 80;
    }
	frame[4] = heatControl_getAmountSetting() >> 8;
	frame[5] = heatControl_getAmountSetting() & 0x00FF;
	if(frame[3] == TEM_SETTING_RO)
	{
		frame[4] = AMOUNT_SETTING_RO >> 8;
		frame[5] = AMOUNT_SETTING_RO & 0x00FF;
		s_requestWaterRO = true;
	}
	else{
		s_requestWaterRO = false;
	}
	uint16_t cks = tools_calCheckSum2Byte(&frame[1],16);
	frame[17] = cks >> 8;
	frame[18] = cks & 0x00FF;
    if(!g_ioTestEn)
    	UART_sendFrame(frame,19);
}


void heatControl_sendMeassageInquire()
{
	if(cntErrUartTimeOut)
		cntErrUartTimeOut--;
	uint8_t frame[1] = {0xA5};
    if(!g_ioTestEn)
    	UART_sendFrame(frame,1);
}

heatControl_error_t heatControl_getErrorNeedShow()
{
	if(heatErrList[HEAT_ERR_Eu])
	{
		lastestErr = HEAT_ERR_Eu;
	}
	else
	{
		if(!heatControl_haveError())
		{
			lastestErr = HEAT_ERR_MAX;
		}
		else if(heatErrList[lastestErr] == false)
		{
			for(uint8_t j = 0; j < HEAT_ERR_MAX; j++)
			{
				if(heatErrList[j] == true)
				{
					lastestErr = j;
				}
			}
		}
	}
	return lastestErr;
//	for(uint8_t i = 0; i < HEAT_ERR_MAX; i++)
//	{
//		if(heatErrList[i] == true)
//		{
//			return i;
//		}
//	}
//	return HEAT_ERR_MAX;
}

void heatControl_task()
{
	if(cntErrUartTimeOut == 0)
	{
		heatErrList[HEAT_ERR_Eu] = true;
		water_isOuting = false;
	}
	switch(s_heatControlState)
	{
		case HEATCONTROL_STATE_NONE:
			if(elapsedTime(g_sysTime, timeLastCheck) > TIME_INTERVAL_SEND_HEAT_MODULE)
			{
				s_heatControlState = HEATCONTROL_STATE_CONTROL;
				timeLastCheck = g_sysTime;
			}
			break;
		case HEATCONTROL_STATE_CONTROL:
			if(elapsedTime(g_sysTime, timeLastCheck) > TIME_INTERVAL_SEND_HEAT_MODULE)
			{
				heatControl_sendMeassageInquire();
				s_heatControlState = HEATCONTROL_STATE_INQUIRE;
				timeLastCheck = g_sysTime;
			}
			break;

		case HEATCONTROL_STATE_INQUIRE:
			if(elapsedTime(g_sysTime, timeLastCheck) > TIME_INTERVAL_SEND_HEAT_MODULE)
			{
				if(!s_requestWaterOut && !s_requestWaterOff)
				{
					if(water_isOuting)
					{
						byteControlWater = BYTE_CONTROL_WATER_OUTING;
					}
					else
					{
						byteControlWater = BYTE_CONTROL_WATER_OFF;
					}
				}
				if(s_requestWaterOut)
				{
					byteControlWater = BYTE_CONTROL_WATER_OUT;
					s_requestWaterOut = false;
				}
				else if(s_requestWaterOff)
				{
					byteControlWater = BYTE_CONTROL_WATER_OFF;
					s_requestWaterOff = false;
				}
				heatControl_sendMeassageControl();
				s_heatControlState = HEATCONTROL_STATE_CONTROL;
				timeLastCheck = g_sysTime;
			}
			break;

		case HEATCONTROL_STATE_MAX:
			break;

	}
}

/***********************************************/


