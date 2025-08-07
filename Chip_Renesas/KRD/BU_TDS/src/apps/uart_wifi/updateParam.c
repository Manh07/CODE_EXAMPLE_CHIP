/*
 * updateParam.c
 *
 *  Created on: Dec 11, 2018
 *      Author: hp
 */
#include "updateParam.h"
#include "string.h"
#include "stdlib.h"
#include "uartHandle.h"
#include "filter_time.h"
#include "filterExpire.h"
#include "Config_SCI1.h"
#include "adc.h"
#include "user_config.h"
#include "UIControl.h"
#include "stdbool.h"
#include "errorCheck.h"
#include "tools.h"
#include "timeCheck.h"
#include "thermo.h"
#include "heatControl.h"
#include "uartVoice.h"
#include "getWater.h"
#define MAX_RES_BUF 10
#define MAX_BUF_LEN_FRAME 120

typedef enum checkState
{
	CHECK_STATE_AT = 0,
	CHECK_STATE_AC,
	CHECK_STATE_TDS,
	CHECK_STATE_BOM,
	CHECK_STATE_VX,
	CHECK_STATE_EXPIRETOTAL,
	CHECK_STATE_FILTERTOTAL,
	CHECK_STATE_TIMEEXPIRE,
	CHECK_STATE_TIMEFILTER,
	CHECK_STATE_RSFT,
	CHECK_STATE_ERRS,
	CHECK_STATE_TEMP_HOT,
	CHECK_STATE_TEMP_COLD,
	CHECK_STATE_CTRL_DISP,
	CHECK_STATE_CTRL_HOT,
	CHECK_STATE_CTRL_COLD,
	NUM_OF_CHECK_STATE,
} CheckState_t;
enum
{
	AP_THAP,
	AP_CAO
};

typedef enum
{
	STT_CO_AP, // có áp
	STT_MAT_AP // mất áp
} stt_ap_t;

static char str_update_buf[MAX_RES_BUF][MAX_BUF_LEN_FRAME] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}; // giua cac lan update cach nhau 1 khoang time de tranh esp nhan nhieu lenh vao bo dem
bool buf_isEmpty[MAX_RES_BUF] = {true, true, true, true, true, true, true, true, true, true};
uint8_t num_respone_cur = 0;
uint8_t index_res_cur = 0;
uint32_t time_last_update = 0;
uint32_t time_last_update_current = 0;

bool s_needUpdateParams[NUM_OF_PARAM] = {false, false, false, false, false, false, false, false, false, false};
CheckState_t s_checkState = CHECK_STATE_AT;
stt_ap_t ap_stt[2] = {STT_MAT_AP, STT_MAT_AP};
bool is_err_update_current[ERR_IRO_NUM_MAX] = {false, false, false, false, false};
uint32_t s_bomIsOn = 0;
uint32_t s_vxIsOn = 0;
uint32_t s_recentFilterReset = 0;

iro_struct_att_to_wifi_t att_struct[IRO_MAX_NUM_ATT] =
	{
		{IRO_VAN_XA, "iro_vx"},
		{IRO_BOM, "iro_bom"},
		{IRO_KET_NUOC, "iro_ketn"},
		{IRO_NUOC_VAO, "iro_nuocIn"},
		{IRO_lifeTime, "iro_lTime"},	  //		 timeExpireRemain
		{IRO_HSD, "iro_hsd"},			  // r,w    timeExpireTotal
		{IRO_RUN_TIME_TOTAL, "iro_rtt"},  // r,w   timeFilterTotal
		{IRO_RUN_TIME_REMAIN, "iro_rtr"}, //       timeFilterRemain
		{IRO_RESET_FILTER, "iro_rsft"},	  // w
		{IRO_TDS, "iro_tds"},
		{IRO_pumpTime, "iro_bomTime"},
		{IRO_ERRS, "iro_errs"},
		{IRO_tempHot, "flow_rate"},
		{IRO_tempCold, "akl_current"},
		{IRO_ctrlDisp, "ctrl_disp"},
		{IRO_ctrlHot, "ctrl_hot"},
		{IRO_ctrlCold, "ctrl_cold"},
};

bool updateParam_transaction(attribute_name_t name, uint32_t *data_in, uint16_t length);

bool updateParam_addDataToBuffer(char *Data)
{
	if (num_respone_cur < MAX_RES_BUF)
	{
		for (uint8_t i = 0; i < MAX_RES_BUF; i++)
		{
			if (buf_isEmpty[i])
			{
				sprintf((char *)str_update_buf[i], "%s", Data);
				buf_isEmpty[i] = false;
				break;
			}
		}
		num_respone_cur++;
		return true;
	}
	else
	{
		return false;
	}
}

#define MAX_CNT_RETRY 3
bool isSendComplete = false;
void updateParam_sendBuffer()
{
	//	static uint8_t currentSend = 0;
	//	uint8_t i;
	//	if(num_respone_cur > 0)
	//	{
	//		if(g_sysTime > time_last_update + TIME_TWO_TIMES_UD)
	//		{
	//			for(i = currentSend; i< MAX_RES_BUF; i++)
	//			{
	//				if(i == (MAX_RES_BUF -1)) currentSend = 0;
	//				else currentSend = i +1;
	//				if(buf_isEmpty[i] == false)
	//				{
	//					R_SCI1_AsyncTransmit(( uint8_t*) str_update_buf[i], strlen(str_update_buf[i]));
	//					time_last_update = g_sysTime;
	//					memset(str_update_buf[i], 0, MAX_BUF_LEN_FRAME);
	//					buf_isEmpty[i] = true;
	//					num_respone_cur--;
	//				    break;					//moi tick chi gui 1 respone
	//				}
	//			}
	//
	//
	//		}
	//
	//	}

	static uint8_t stateData = DATA_STATE_CHECK;
	static bool isHaveDataToSend = false;
	static uint8_t cntRetry = 0;
	static uint8_t currentSend = 0;	   // index chuan bi cho lan check tiep
	static uint8_t currentProcess = 0; // index dang gui
	uint8_t i = 0;
	switch (stateData)
	{
	case DATA_STATE_CHECK:
	{
		if (num_respone_cur > 0)
		{
			isHaveDataToSend = false;
			for (i = currentSend; i < MAX_RES_BUF; i++)
			{
				if (i == (MAX_RES_BUF - 1))
					currentSend = 0;
				else
					currentSend = i + 1;
				if (buf_isEmpty[i] == false)
				{
					isHaveDataToSend = true;
					currentProcess = i;
					stateData = DATA_STATE_INIT;
					break; // moi tick chi gui 1 respone
				}
			}
		}
		break;
	}
	case DATA_STATE_INIT:
	{
		if (isHaveDataToSend)
		{
			isHaveDataToSend = false;
			cntRetry = 0;
			isSendComplete = false;
			stateData = DATA_STATE_SEND;
		}
		break;
	}
	case DATA_STATE_SEND:
	{
		R_SCI1_AsyncTransmit((uint8_t *)str_update_buf[currentProcess], strlen(str_update_buf[currentProcess]));
		time_last_update_current = g_sysTime;
		stateData = DATA_STATE_WAIT_RSP;
		break;
	}
	case DATA_STATE_WAIT_RSP:
	{
		if ((elapsedTime(g_sysTime, time_last_update_current) > 200) && !isSendComplete)
		{
			if (++cntRetry < MAX_CNT_RETRY)
			{
				stateData = DATA_STATE_SEND;
				break;
			}
			//				NotRsp = true;
			stateData = DATA_STATE_END;
			memset(str_update_buf[currentProcess], 0, MAX_BUF_LEN_FRAME);
			buf_isEmpty[currentProcess] = true;
			num_respone_cur--;
			time_last_update = g_sysTime;
			break;
		}
		if (isSendComplete)
		{
			stateData = DATA_STATE_END;
			memset(str_update_buf[currentProcess], 0, MAX_BUF_LEN_FRAME);
			buf_isEmpty[currentProcess] = true;
			num_respone_cur--;
			time_last_update = g_sysTime;
		}
		break;
	}
	case DATA_STATE_END:
	{
		if (elapsedTime(g_sysTime, time_last_update) > TIME_TWO_TIMES_UD) // wait for next frame
		{
			stateData = DATA_STATE_CHECK;
		}
		break;
	}

	default:
		break;
	}
}

void updateParam_checkNeedUpdate()
{
	uint32_t data_in[1];
	switch (s_checkState)
	{
	case CHECK_STATE_AT:
	{
		if (s_needUpdateParams[PARAM_TYPE_AT]) // có áp thấp, có nước , ghi 1
		{
			if (ap_stt[AP_THAP] == STT_CO_AP)
			{
				data_in[0] = 1;
			}
			else
			{
				data_in[0] = 0;
			}
			if (updateParam_transaction(IRO_NUOC_VAO, (uint32_t *)data_in, 1) == 0) // chi khi da gui vao buf ok thi dung
			{
				uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_NUOC_VAO, (uint8_t *)data_in, 1); // send voice 1 tam thoi k can retry
				s_needUpdateParams[PARAM_TYPE_AT] = false;
			}
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_AC:
	{
		if (s_needUpdateParams[PARAM_TYPE_AC]) // có áp cao, chưa đầy bình, ghi 1
		{
			if (ap_stt[AP_CAO] == STT_CO_AP)
			{
				data_in[0] = 1;
			}
			else
			{
				data_in[0] = 0;
			}
			if (updateParam_transaction(IRO_KET_NUOC, (uint32_t *)data_in, 1) == 0)
			{
				uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_BINH_AP, (uint8_t *)data_in, 1);
				s_needUpdateParams[PARAM_TYPE_AC] = false;
			}
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_TDS:
	{
		if (s_needUpdateParams[PARAM_TYPE_TDS])
		{
			uint32_t data_in[1];
			data_in[0] = ADC_GetTdsValueDisplay(TDS_OUT_VALUE);
			if (updateParam_transaction(IRO_TDS, data_in, 1) == 0)
			{
				uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_TDS, (uint8_t *)data_in, 2); // 2byte
				s_needUpdateParams[PARAM_TYPE_TDS] = false;
			}
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_BOM:
	{
		if (s_needUpdateParams[PARAM_TYPE_BOM])
		{
			if (updateParam_transaction(IRO_BOM, &s_bomIsOn, 1) == 0)
			{
				uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_BOM, (uint8_t *)&s_bomIsOn, 1);
				s_needUpdateParams[PARAM_TYPE_BOM] = false;
			}
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_VX:
	{
		if (s_needUpdateParams[PARAM_TYPE_VX])
		{
			if (updateParam_transaction(IRO_VAN_XA, &s_vxIsOn, 1) == 0)
			{
				uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_VAN_XA, (uint8_t *)&s_vxIsOn, 1);
				s_needUpdateParams[PARAM_TYPE_VX] = false;
			}
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_EXPIRETOTAL:
	{
		if (s_needUpdateParams[PARAM_TYPE_EXPIRETOTAL])
		{
			updateParam_FilterExpireTotal();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_FILTERTOTAL:
	{
		if (s_needUpdateParams[PARAM_TYPE_FILTERTOTAL])
		{
			updateParam_FilterTimeTotal();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_TIMEEXPIRE:
	{
		if (s_needUpdateParams[PARAM_TYPE_TIMEEXPIRE])
		{
			updateParam_expireFilter();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_TIMEFILTER:
	{
		if (s_needUpdateParams[PARAM_TYPE_TIMEFILTER])
		{
			updateParam_timeFilter();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_RSFT:
	{
		if (s_needUpdateParams[PARAM_TYPE_RSFT])
		{
			updateParam_updateRsft();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_ERRS:
	{
		if (s_needUpdateParams[PARAM_TYPE_ERRS])
		{
			updateParam_updateError();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_TEMP_HOT:
	{
		if (s_needUpdateParams[PARAM_TYPE_TEMPHOT])
		{
			updateParam_updateTempHot();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_TEMP_COLD:
	{
		if (s_needUpdateParams[PARAM_TYPE_TEMPCOLD])
		{
			updateParam_updateTempCold();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_CTRL_DISP:
	{
		if (s_needUpdateParams[PARAM_TYPE_CTRLDISP])
		{
			updateParam_updateCtrlDisp();
		}
		s_checkState++;
		break;
	}
	case CHECK_STATE_CTRL_HOT:
	{
		//            if(s_needUpdateParams[PARAM_TYPE_CTRLHOT])
		//            {
		//                updateParam_updateCtrlHot();
		//            }
		s_checkState++;
		break;
	}
	case CHECK_STATE_CTRL_COLD:
	{
		//			if(s_needUpdateParams[PARAM_TYPE_CTRLCOLD])
		//			{
		//				updateParam_updateCtrlCold();
		//			}
		s_checkState++;
		break;
	}
	case NUM_OF_CHECK_STATE:
		s_checkState = CHECK_STATE_AT;
		break;
	default:
		break;
	}
}
void updateParam_checkConditons()
{
	static uint32_t lastTimeApCaoStable = 0;
	// ap thap
	if (!ErrorCheck_getError(ERROR_TYPE_INCOME_WATER_LOST) &&
		(ap_stt[AP_THAP] == STT_MAT_AP)) // có áp thấp, có nước , ghi 1
	{
		ap_stt[AP_THAP] = STT_CO_AP;
		s_needUpdateParams[PARAM_TYPE_AT] = true;
	}
	if (ErrorCheck_getError(ERROR_TYPE_INCOME_WATER_LOST) &&
		(ap_stt[AP_THAP] == STT_CO_AP)) // mất áp thấp, mất nước , ghi 0
	{
		ap_stt[AP_THAP] = STT_MAT_AP;
		s_needUpdateParams[PARAM_TYPE_AT] = true;
	}

	// ap cao
	if (CHECK_CO_AP_CAO && (ap_stt[AP_CAO] == STT_MAT_AP)) // có áp cao, chưa đầy bình, ghi 1
	{
		if ((uint32_t)(g_sysTime - lastTimeApCaoStable) > 1000)
		{
			ap_stt[AP_CAO] = STT_CO_AP;
			s_needUpdateParams[PARAM_TYPE_AC] = true;
			lastTimeApCaoStable = g_sysTime;
		}
		else
		{
		}
	}

	else if (!CHECK_CO_AP_CAO && (ap_stt[AP_CAO] == STT_CO_AP)) // mất áp cao, đầy bình, ghi 0
	{
		if ((uint32_t)(g_sysTime - lastTimeApCaoStable) > 1000)
		{
			ap_stt[AP_CAO] = STT_MAT_AP;
			s_needUpdateParams[PARAM_TYPE_AC] = true;
			lastTimeApCaoStable = g_sysTime;
		}
		else
		{
		}
	}
	else
	{
		lastTimeApCaoStable = g_sysTime;
	}
	// TDS
	if (ADC_TdsOutIsNewValue())
	{
		s_needUpdateParams[PARAM_TYPE_TDS] = true;
	}

	updateParam_checkError();
}
void updateParam_tick()
{
	updateParam_sendBuffer();
	updateParam_checkConditons();
	updateParam_checkNeedUpdate();
}
bool updateParam_transaction(attribute_name_t name, uint32_t *data_in, uint16_t length)
{
	char buf[MAX_BUF_LEN_FRAME] = "";
	uint16_t length_buf = 0;

	// index1 "UP"
	strcat((char *)buf, "[UP,");

	// index2: property_code
	for (uint8_t i = 0; i < IRO_MAX_NUM_ATT; i++)
	{
		if (att_struct[i].att == name)
		{
			strcat((char *)buf, (const char *)att_struct[i].property_code);
			break;
		}
	}

	char buf_sub[20] = "";
	// index3: datalength
	sprintf((char *)buf_sub, ",%d,", length);
	strcat((char *)buf, (char *)buf_sub);

	// data
	if (length > 0)
	{
		for (uint8_t i = 0; i < length; i++)
		{
			sprintf((char *)buf_sub, "%d,", *(data_in + i));
			strcat((char *)buf, (char *)buf_sub);
		}
	}
	// end of buf
	length_buf = strlen((char *)buf);
	buf[length_buf - 1] = ']';
	// Ex: iro_errs: [UP,iro_errs,0]

	// add 2 byte cks to trail
	uint8_t cks_H, cks_L, cks;
	cks = tools_calculateChecksum((uint8_t *)buf, length_buf);
	tools_encodeCheckSum(cks, (char *)&cks_H, (char *)&cks_L);
	buf[length_buf] = cks_H;
	buf[length_buf + 1] = cks_L;
	buf[length_buf + 2] = '\0';

	if (updateParam_addDataToBuffer(buf))
		return 0;
	else
		return 1;
}

void updateParam_sendConfigWifiCmd()
{
	char buf[20] = "[Wifi_conf,1]";
	updateParam_addDataToBuffer(buf);
}

/*hàm gửi trạng thái hệ thống

 * */
void updateParam_statusSystem(Output_t out, uint8_t isOn) // called in setOutput(Output_t out, bool isOn)
{
	switch (out)
	{
	case OUT_PUMP:
		s_bomIsOn = isOn;
		s_needUpdateParams[PARAM_TYPE_BOM] = true;
		break;
	case OUT_VAN:
		s_vxIsOn = isOn;
		s_needUpdateParams[PARAM_TYPE_VX] = true;
		break;
	default:
		break;
	}
}

// called in function processPump()
void updateParam_timePumpRun()
{

	uint32_t data_in[1];

	data_in[0] = pumpControl_getTimePumpRunHour();
	if (updateParam_transaction(IRO_pumpTime, (uint32_t *)data_in, 1) == 0)
	{
	}
}

uint16_t filter_hourToDay(uint32_t hour)
{
	if (hour % 24 > 0)
	{
		return (uint16_t)(hour / 24 + 1);
	}
	else
	{
		return (uint16_t)(hour / 24);
	}
}
// thoi gian con lai cua loi loc, giam khi bom hoat dong
void updateParam_timeFilter() // called in function processPump()
{
	uint32_t data_in[FILTER_NUM];
	uint16_t data_2[FILTER_NUM];
	for (uint8_t i = 0; i < FILTER_NUM; i++)
	{
		data_in[i] = (uint32_t)filter_time_getFilterHour(i);
		data_2[i] = data_in[i];
	}
	if (updateParam_transaction(IRO_RUN_TIME_REMAIN, (uint32_t *)data_in, FILTER_NUM) == 0)
	{
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_RUNTIME_REMAIN, (uint8_t *)data_2, FILTER_NUM * 2);
		s_needUpdateParams[PARAM_TYPE_TIMEFILTER] = false;
	}
}
// tuoi tho con lai cua loi
void updateParam_expireFilter() // called in filterExpire_process()
{
	uint32_t data_in[FILTER_NUM];
	uint16_t data_2[FILTER_NUM];
	for (uint8_t i = 0; i < FILTER_NUM; i++)
	{
		data_in[i] = (uint32_t)filterExpire_getFilterHour(i);
		data_2[i] = filter_hourToDay(data_in[i]);
	}
	if (updateParam_transaction(IRO_lifeTime, (uint32_t *)data_in, FILTER_NUM) == 0)
	{
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_HSD_REMAIN, (uint8_t *)data_2, FILTER_NUM * 2);
		s_needUpdateParams[PARAM_TYPE_TIMEEXPIRE] = false;
	}
}
// tong thoi gian hoat dong cua loi
void updateParam_FilterTimeTotal()
{
	uint32_t data_in[FILTER_NUM];
	uint16_t data_2[FILTER_NUM];
	for (uint8_t i = 0; i < FILTER_NUM; i++)
	{
		data_in[i] = (uint32_t)userConfig_getFilterTimeTotalHour(i);
		data_2[i] = data_in[i];
	}
	if (updateParam_transaction(IRO_RUN_TIME_TOTAL, (uint32_t *)data_in, FILTER_NUM) == 0)
	{
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_RUNTIME_DEFAULT, (uint8_t *)data_2, FILTER_NUM * 2);
		s_needUpdateParams[PARAM_TYPE_FILTERTOTAL] = false;
	}
}
// tong tuoi tho cua loi
void updateParam_FilterExpireTotal()
{
	uint32_t data_in[FILTER_NUM];
	uint16_t data_2[FILTER_NUM];
	for (uint8_t i = 0; i < FILTER_NUM; i++)
	{
		data_in[i] = (uint32_t)userConfig_getFilterExpireTotalHour(i);
		data_2[i] = filter_hourToDay(data_in[i]);
	}
	if (updateParam_transaction(IRO_HSD, (uint32_t *)data_in, FILTER_NUM) == 0)
	{
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_HSD_DEFAULT, (uint8_t *)data_2, FILTER_NUM * 2);
		s_needUpdateParams[PARAM_TYPE_EXPIRETOTAL] = false;
	}
}

void updateParam_updateRsft()
{
	if (updateParam_transaction(IRO_RESET_FILTER, (uint32_t *)&s_recentFilterReset, 1) == 0)
	{
		uint8_t dataUart[2];
		dataUart[0] = uartVoice_msgIdxIsControlling(MSG_IDX_UP_CP_RESET_FITLER);
		dataUart[1] = s_recentFilterReset;
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_RESET_FITLER, (uint8_t *)dataUart, 2);
		s_needUpdateParams[PARAM_TYPE_RSFT] = false;
	}
}
void updateParam_checkError()
{
	uint8_t errIdx, errCur;
	for (errIdx = ERROR_TYPE_INCOME_WATER_LOST, errCur = ERR_WT_LOST;
		 errIdx <= ERROR_TYPE_TDS_OUT && errCur <= ERR_TDS_OVER; errIdx++)
	{
		if (errIdx == ERROR_TYPE_TDS_IN)
			continue; // tam thoi khong check loi TDS_IN
		if (ErrorCheck_getError((ErrorType_t)errIdx) != is_err_update_current[errCur])
		{
			is_err_update_current[errCur] = ErrorCheck_getError((ErrorType_t)errIdx);
			s_needUpdateParams[PARAM_TYPE_ERRS] = true;
		}
		errCur++;
	}
}

void updateParam_updateError()
{
	uint32_t err_value[ERR_IRO_NUM_MAX];
	uint8_t len = 0;
	for (uint8_t i = ERR_WT_LOST; i <= ERR_TDS_OVER; i++)
	{
		uint8_t value;
		if (is_err_update_current[i] == true)
		{
			if (i == ERR_WT_LOST)
				value = VALUE_ERR_WT_LOST;
			if (i == ERR_WT_WEAK)
				value = VALUE_ERR_WT_WEAK;
			if (i == ERR_PUMP_OVER)
				value = VALUE_ERR_PUMP_OVER;
			if (i == ERR_WT_LEAK)
				value = VALUE_ERR_WT_LEAK;
			if (i == ERR_TDS_OVER)
				value = VALUE_ERR_TDS_OVER;
			err_value[len++] = value;
		}
	}
	if (updateParam_transaction(IRO_ERRS, (uint32_t *)err_value, len) == 0)
	{
		uint32_t errorBit = Error_getErrorBit();
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_ERROR, (uint8_t *)&errorBit, 4);
		s_needUpdateParams[PARAM_TYPE_ERRS] = false;
	}
}

void updateParam_updateTempHot()
{
	uint32_t tem = getWater_getValueFlowToEsp();
	if (updateParam_transaction(IRO_tempHot, (uint32_t *)&tem, 1) == 0)
	{
		//    	uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_TEMP_HOT, (uint8_t*)&tem, 2);
		s_needUpdateParams[PARAM_TYPE_TEMPHOT] = false;
	}
}
void updateParam_updateTempCold()
{
	uint32_t tem = getWater_getValueCurrentToEsp();
	if (updateParam_transaction(IRO_tempCold, (uint32_t *)&tem, 1) == 0)
	{
		//    	uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_TEMP_COLD, (float*)&tem, 2);
		s_needUpdateParams[PARAM_TYPE_TEMPCOLD] = false;
	}
}
void updateParam_updateCtrlHot()
{
	uint32_t hotEn;
	hotEn = thermo_getTankHotIsEnable() ? 1 : 0;
	if (updateParam_transaction(IRO_ctrlHot, (uint32_t *)&hotEn, 1) == 0)
	{
		uint8_t dataUart[2];
		dataUart[0] = uartVoice_msgIdxIsControlling(MSG_IDX_UP_CP_ENABLE_HOTTANK);
		dataUart[1] = hotEn;
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_ENABLE_HOTTANK, (uint8_t *)dataUart, 2);
		s_needUpdateParams[PARAM_TYPE_CTRLHOT] = false;
	}
}
void updateParam_updateCtrlCold()
{
	uint32_t coldEn;
	coldEn = thermo_getTankColdIsEnable() ? 1 : 0;
	if (updateParam_transaction(IRO_ctrlCold, (uint32_t *)&coldEn, 1) == 0)
	{
		uint8_t dataUart[2];
		dataUart[0] = uartVoice_msgIdxIsControlling(MSG_IDX_UP_CP_ENABLE_COLDTANK);
		dataUart[1] = coldEn;
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_ENABLE_COLDTANK, (uint8_t *)dataUart, 2);
		s_needUpdateParams[PARAM_TYPE_CTRLCOLD] = false;
	}
}
void updateParam_updateCtrlDisp()
{
	// for only function fast hot block
	uint32_t disp[2];
	disp[0] = heatControl_getSettingTemDefaultIdx();
	disp[1] = heatControl_getSettingAmountDefaultIdx();
	uint8_t disp_2[3];
	disp_2[0] = uartVoice_msgIdxIsControlling(MSG_IDX_UP_CP_SETTING_FASTBLOCK);
	disp_2[1] = disp[0];
	disp_2[2] = disp[1];
	if (updateParam_transaction(IRO_ctrlDisp, (uint32_t *)disp, 2) == 0)
	{
		uartVoice_sendFrameWithMsgIdx(MSG_IDX_UP_CP_SETTING_FASTBLOCK, (uint8_t *)disp_2, 3);
		s_needUpdateParams[PARAM_TYPE_CTRLDISP] = false;
	}
}

// ex: //data_in: iro_rtt,9,300,300,300,3600,500,300,300,300,400
void updateParam_handleMsgFromUart(uint8_t *data_in, uint8_t *data_out)
{
	uint8_t index = 0;
	bool isValid = false;
	uint8_t *ptr = NULL;

	// find property_code
	char *ptr_token = NULL;
	ptr_token = strtok((char *)data_in, ",");
	for (index = 0; index < IRO_MAX_NUM_ATT; index++)
	{
		if ((const char *)strcmp(att_struct[index].property_code, (const char *)ptr_token) == 0)
		{
			isValid = true;
			break;
		}
	}
	if (isValid == false)
	{
		strcpy((char *)data_out, "error_code");
		return;
	}

	ptr = data_in + strlen(ptr_token) + 1; // ex: -->'9'
	uint32_t data_wifi[20];				   // byte đầu là số filter, còn lại la giá trị cacs filter
	uint16_t len = 0;					   // số phần tử tách được từ chuỗi
	len = convParamStringToNumber(ptr, data_wifi);
	if (len != (data_wifi[0] + 1))
	{
		strcpy((char *)data_out, "error_len");
		return;
	}

	// data ok
	uint32_t *value_fil = &data_wifi[1];
	switch (att_struct[index].att)
	{
	case IRO_HSD:
	{
		strcat((char *)data_out, "iro_hsd");

		if (data_wifi[0] != FILTER_NUM)
		{
			strcpy((char *)data_out, "error_num_filter");
			return;
		}
		AppConfig_setAllFilterExpireTotal(value_fil);
		// for(uint8_t i = 0; i< len; i++)
		// {
		// 	char buf_sub[20] = "";
		// 	sprintf((char*)buf_sub, ",%d",data_wifi[i]);
		// 	strcat((char*)data_out,(char*)buf_sub);
		// }
		break;
	}
	case IRO_RUN_TIME_TOTAL:
	{
		strcat((char *)data_out, "iro_rtt");

		if (data_wifi[0] != FILTER_NUM)
		{
			strcpy((char *)data_out, "error_num_filter");
			return;
		}
		AppConfig_setAllFilterTimeTotal(value_fil);
		// for(uint8_t i = 0; i< len; i++)
		// {
		// 	char buf_sub[20] = "";
		// 	sprintf((char*)buf_sub, ",%d",data_wifi[i]);
		// 	strcat((char*)data_out,(char*)buf_sub);
		// }
		break;
	}
	case IRO_RESET_FILTER:
	{
		//			if((data_wifi[1] >=0) &&(data_wifi[1] <FILTER_NUM))
		if (data_wifi[1] < FILTER_NUM)
		{
			s_recentFilterReset = data_wifi[1];
			AppConfig_resetTimeIndex((uint8_t)data_wifi[1]);
			if (data_wifi[1] == 4)
			{
				// loi C
				gpio_setNeedXaSucWhenResetFilterC(); //->update UI
			}
		}
		else
		{
			if (data_wifi[1] == 0xFF) // dung cai nay cho xa suc 24h
			{
				gpio_setNeedXaSucInterval();
			}
		}
		break;
	}
	case IRO_ctrlDisp:
	{
		// for only func use fast hot block
		if (data_wifi[0] == 2)
		{
			heatControl_setSettingDefaultIdx(data_wifi[1], data_wifi[2]);
		}
		break;
	}
	case IRO_ctrlHot:
	{
		if (data_wifi[0] == 1)
		{
			thermo_controlEnableTankHot(data_wifi[1]);
		}
		break;
	}
	case IRO_ctrlCold:
	{
		if (data_wifi[0] == 1)
		{
			thermo_controlEnableTankCold(data_wifi[1]);
		}
		break;
	}
	}
}

void updateParam_updateIo()
{
	uint8_t i;
	for (i = 0; i < NUM_OF_PARAM; ++i)
	{
		if ((i == PARAM_TYPE_RSFT) || (i == PARAM_TYPE_TEMPCOLD) || (i == PARAM_TYPE_TEMPHOT))
		{
			s_needUpdateParams[i] = false;
		}
		else
		{
			s_needUpdateParams[i] = true;
		}
	}
}

void updateParam_setNeedUpdate(uint8_t typeParam)
{
	s_needUpdateParams[typeParam] = true;
}
