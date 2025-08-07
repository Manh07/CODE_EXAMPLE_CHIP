/*
 * updateParam.h
 *
 *  Created on: Dec 11, 2018
 *      Author: hp
 */

#ifndef APPS_UART_WIFI_UPDATEPARAM_H_
#define APPS_UART_WIFI_UPDATEPARAM_H_
#include "config.h"
#include "pumpControl.h"
#include "gpio.h"
#include "errorCheck.h"


#define UD_TIME_PUMP_AFTER_MS 5000		//ms
#define TIME_TWO_TIMES_UD 500	//ms
typedef enum
{
	IRO_VAN_XA = 0,
	IRO_BOM,
	IRO_KET_NUOC,
	IRO_NUOC_VAO,
	IRO_lifeTime,
	IRO_HSD,
	IRO_RUN_TIME_TOTAL,
	IRO_RUN_TIME_REMAIN,
	IRO_RESET_FILTER,
	IRO_TDS,
	IRO_pumpTime,
	IRO_ERRS,
    IRO_tempHot,
    IRO_tempCold,
    IRO_ctrlDisp,    //for khoi nong nhanh
    IRO_ctrlHot,    //for co dieu khien TS01
	IRO_ctrlCold,    //for co dieu khien TS01
	IRO_MAX_ATT
} attribute_name_t;

#define IRO_MAX_NUM_ATT IRO_MAX_ATT

enum paramType{
	PARAM_TYPE_AT = 0,
	PARAM_TYPE_AC,
	PARAM_TYPE_TDS,
	PARAM_TYPE_BOM,
	PARAM_TYPE_VX,
	PARAM_TYPE_EXPIRETOTAL,
	PARAM_TYPE_FILTERTOTAL,
	PARAM_TYPE_TIMEEXPIRE,
	PARAM_TYPE_TIMEFILTER,
	PARAM_TYPE_RSFT,
	PARAM_TYPE_ERRS,
    PARAM_TYPE_TEMPHOT,
    PARAM_TYPE_TEMPCOLD,
    PARAM_TYPE_CTRLDISP,        //for khoi nong nhanh
    PARAM_TYPE_CTRLHOT,        //for co dieu khien TS01
	PARAM_TYPE_CTRLCOLD,        //for co dieu khien TS01
	NUM_OF_PARAM
};
enum
{
	VALUE_ERR_WT_LOST = 4,
	VALUE_ERR_WT_WEAK = 2,
	VALUE_ERR_PUMP_OVER = 1,
	VALUE_ERR_WT_LEAK = 5,
	VALUE_ERR_TDS_OVER = 100
};
enum
{
	ERR_WT_LOST = 0,
	ERR_WT_WEAK,
	ERR_PUMP_OVER,
	ERR_WT_LEAK,
	ERR_TDS_OVER,
	ERR_IRO_NUM_MAX
};

typedef struct
{
	attribute_name_t att;
	const char property_code[20];
} iro_struct_att_to_wifi_t;


enum
{
	INDEX_UP,
	INDEX_PRO_CODE,
	INDEX_LEN_DATA
};

enum
{
	DATA_STATE_CHECK = 0,
	DATA_STATE_INIT,
	DATA_STATE_SEND,
	DATA_STATE_WAIT_RSP,
	DATA_STATE_END
};

void updateParam_statusSystem(Output_t out, uint8_t isOn);

void updateParam_tdsOut();  //1000ms
void updateParam_timePumpRun();		//call if 1000ms
void updateParam_timeFilter();
void updateParam_expireFilter();
void updateParam_FilterTimeTotal();
void updateParam_FilterExpireTotal();
void updateParam_updateError();
void updateParam_checkError();
void updateParam_handleMsgFromUart(uint8_t* data_in, uint8_t* data_out);
void updateParam_updateRsft();
void updateParam_updateIo();
void updateParam_sendConfigWifiCmd();

void updateParam_updateTempHot();
void updateParam_updateTempCold();
void updateParam_updateCtrlHot();
void updateParam_updateCtrlCold();
void updateParam_updateCtrlDisp();

void updateParam_setNeedUpdate(uint8_t typeParam);

void updateParam_tick();		//10ms
#endif /* APPS_UART_WIFI_UPDATEPARAM_H_ */
