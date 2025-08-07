
/******************************************************************************
 *
 * Embedded software team.
 * (c) Copyright 2018.
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************/
/**
 *
 * @file         Adc.h
 *
 * @author    	trongkn
 *
 * @version   1.0
 *
 * @date
 *
 * @brief     Brief description of the file
 *
 * Detailed Description of the file. If not used, remove the separator above.
 *
 */

#ifndef ADC_H_
#define ADC_H_

/******************************************************************************
 * Includes
 ******************************************************************************/

#include <config.h>
#include "r_cg_s12ad.h"
#include <queue.h>
#include "r_cg_userdefine.h"
#include "flash_app.h"
#include "uart.h"
#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
/******************************************************************************
 * Constants
 ******************************************************************************/

/******************************************************************************
 * Macros
 ******************************************************************************/

#define ADC_SAMPLE_QUEUE_SIZE (8)
#define TDS_OUT_CHANNEL ADCHANNEL6 // tds 1
#define TDS_IN_CHANNEL ADCHANNEL2  // tds 3
#define TDS_BU_CHANNEL ADCHANNEL5  // tds 2

#define H20_CHANNEL_DETECT_1 ADCHANNEL4
#define TEMP_NONG_CHANNEL ADCHANNEL3
#define ADC_FB_CHANNEL ADCHANNEL17

#define ADC_SAMPLE_CAL_MAX (50)

#define UPDATE_TDS_IN (0)
#define UPDATE_TDS_OUT (1)
#define UPDATE_VALUE (2)
#define TDS_OUT_VALUE_MIN (0)
#define TDS_OUT_MAX_DEFAULT (100)
#define ADC_H2O_DET_DEFAULT (4000)
#define H2O_DET_CNT_MAX (3)
#define TDS_OUT_VALUE_MAX (1000)
#define CALIB_POINT_MAX (16)
#define TDS_THRESHOLD (1)
#define CNT_THRESHOLD_MAX (10)
/******************************************************************************
 * Types
 ******************************************************************************/

/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *
 * Detailed explanation.
 */

typedef struct
{
	uint16_t high_cnt;
	uint16_t low_cnt;
	int32_t sum_adc_high;
	int32_t sum_adc_low;
	int16_t sma_tds_adc;
	int32_t sum_tds_adc;
	uint16_t tds_display;
	uint8_t cnt_down;
	uint8_t cnt_increase;
	QUEUE_NODE_T *adc_sample;

} TDS_T;

typedef struct
{
	int16_t adc_value[CALIB_POINT_MAX];	 //
	uint16_t tds_value[CALIB_POINT_MAX]; //
} TDS_CALIB_PARAM_T;

typedef struct
{
	TDS_CALIB_PARAM_T tds_in;  //
	TDS_CALIB_PARAM_T tds_out; //
	uint16_t tds_out_max;
	uint16_t adc_h2o_det;
} TDS_CONFIG_T;

typedef enum
{
	TDS_IN_VALUE = 0,  //
	TDS_OUT_VALUE = 1, //
	TDS_BU_VALUE = 2,  //
	TDS_VALUE_MAX
} TDS_E;

typedef enum
{
	H2O_1 = 0, //
	H2O_2 = 1, //
} H2O_T;

/******************************************************************************
 * Global variables
 ******************************************************************************/

/******************************************************************************
 * Global functions
 ******************************************************************************/
PUBLIC void ADC_Init(void);

PUBLIC void ADC_UpdateTds(uint8_t state);

PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue, TDS_E channel);

PUBLIC void ADC_UpdateTdsDisplay();

PUBLIC uint16_t ADC_GetTdsValueDisplay(TDS_E channel);

bool ADC_TdsOutIsNewValue();

PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue, TDS_E channel, uint8_t index);

PUBLIC bool ADC_GetH2oDet(H2O_T index);

PUBLIC ERR_E ADC_GetAdcTable(TDS_E channel, char *out);

PUBLIC ERR_E ADC_GetTdsTable(TDS_E channel, char *out);

PUBLIC ERR_E ADC_WriteAdcFromUart(int *adcTable, TDS_E channel);

PUBLIC ERR_E ADC_WriteTdsFromUart(int *tdsTable, TDS_E channel);

PUBLIC bool ADC_SetDisableUpdateTds(bool disable);

PUBLIC void ADC_process();

PUBLIC ERR_E ADC_CalibTdsAutoZero();

PUBLIC ERR_E ADC_GetCalibTdsParam(TDS_E channel, char *out);
int16_t ADC_getAdcTdsOutDebug();
int16_t ADC_getAdcTdsInDebug();
;
void ADC_displayTask();

void userAdc_calculateTemperatureNong();
void userAdc_calculateTemperatureLanh();
uint32_t userAdc_calculateFB();
uint16_t userAdc_getTemNong();
uint16_t userAdc_getTemLanh();
/******************************************************************************
 * Inline functions
 ******************************************************************************/

/*
 * data mach 2 v2.12
[TDS_TABLE,1][TDS_TABLE,0,0,4,11,20,30,40,50,60,71,80,90,105,201,303,402,502,602,706,807,1010,2010]
[TDS_TABLE,2][TDS_TABLE,0,0,4,11,20,30,40,50,60,71,80,90,105,201,303,402,502,602,706,807,1010,2010]
[ADC_TABLE,1][ADC_TABLE,0,2468,2428,2306,1726,1633,1561,1511,1462,1410,1372,1332,1276,1014,813,686,596,508,412,338,250,0]
[ADC_TABLE,2][ADC_TABLE,0,2468,2428,2306,1726,1633,1561,1511,1462,1410,1372,1332,1276,1014,813,686,596,508,412,338,250,0]
 * */

#endif
