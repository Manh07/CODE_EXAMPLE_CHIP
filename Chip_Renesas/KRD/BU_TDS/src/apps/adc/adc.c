/******************************************************************************
 *
 * Embedded software team.
 * (c) Copyright 2018.
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************/
/*!
 *
 * @file        adc.c
 *
 * @author    trongkn
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

#include <adc.h>
#include <Config_S12AD0.h>
#include <rx130/r_flash_rx130.h>

#include "stdbool.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "user_config.h"
#include "CurentData.h"
#include "timeCheck.h"

// #include "Config_S12AD0.h"
/******************************************************************************
 * External objects
 ******************************************************************************/
extern volatile uint8_t g_adc_flag;
extern volatile uint8_t g_pwm_value;
/******************************************************************************
 * Global variables
 ******************************************************************************/
int16_t current_adc_tds_in = 0;
int16_t current_adc_tds_out = 0;
int16_t current_adc_tds_bu = 0;
PUBLIC bool g_disableTdsOut = false;
PUBLIC bool g_disableTdsBu = false;
PUBLIC bool g_disableTdsIn = false;

uint16_t adc_result_tds_bu = 0;
uint16_t adc_result_tds_in = 0;
uint16_t adc_result_tds_out = 0;
uint16_t adc_result_nong = 0;
uint16_t adc_result_lanh = 0;
uint16_t s_adcTempNong = 0, s_adcTempLanh = 0;
uint16_t temNong = 0; //*10
uint16_t temLanh = 0; //*10
uint16_t adc_result_fb = 0;
uint16_t s_adcFb = 0;

volatile uint8_t lastLogicPwm = 0;
/* Global flag to indicate A/D conversion operation is completed */

/******************************************************************************
 * Constants and macros
 ******************************************************************************/
#define TIME_OUT_MAX (10)

#define ADC_LOW_VALUE (0)

#define ADC_MAX_SAMPLE (200)

#define ADC_HIGH_VALUE (500)

#define ZERO_CALIB 15

/******************************************************************************
 * Local types
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes
 ******************************************************************************/
void userAdc_getNewTableAdcFromTemp(int16_t *tableAdc, uint16_t temp);

/******************************************************************************
 * Local variables
 ******************************************************************************/

LOCAL TDS_CALIB_PARAM_T TDS_BU_CONFIG_DEFAULD =
	{
		.adc_value = {0, 1431, 2203, 2365, 2495, 2504, 2549, 2625, 2665, 2711, 2748, 2789, 2789, 2789, 2789, 2789}, // 1k
		.tds_value = {0, 500, 1500, 2000, 2500, 2800, 3000, 3200, 3600, 4000, 5000, 6400, 6400, 6400, 6400, 6400}};
// tds in giống bảng tds out 10k
LOCAL const TDS_CALIB_PARAM_T TDS_IN_CONFIG_DEFAULD =
	{
		// .adc_value = {0, 127,  271, 379,   594,  958, 1266, 1441, 1771,  1903,  2237, 2713, 3061, 3364,3485, 3608},
		// .tds_value = {0,   1,    6,   10,  20,   33,   47,   59,   75,   97,   137,  242, 386,  622, 782,  1000}
		// .adc_value = {0, 100,  239, 300,   750,  1140, 1350, 1700, 1920,  2200,  2260, 2500, 2800, 2900,3100, 3200},
		// .tds_value = {0,   1,    5,   12,  22,   40,   50,   67,   96,   117,   132,  172, 250,  300, 350,  400}
		.adc_value = {0, 150, 300, 450, 800, 120, 1600, 1900, 2100, 2300, 2500, 2700, 2980, 3090, 3300, 3500},
		.tds_value = {0, 1, 5, 12, 22, 40, 50, 67, 96, 117, 132, 172, 250, 300, 350, 400}};

LOCAL const TDS_CALIB_PARAM_T TDS_OUT_CONFIG_DEFAULD =
	{
		//		.adc_value = {3850,3725,3575,3130,2770,2496,2302,2157,2014,1880,1766,1672,1241,1045,951,867,816,784,758,728,718},
		//		.tds_value = {0,7,11,20,30,40,50,60,70,80,90,100,200,300,400,500,600,700,800,900,1000}
		//		 .adc_value = {0, 100,  239, 392,   623,  934, 1343, 1538, 1730,  1963,  2347, 2820, 3151, 3385,3525, 3608},
		//		 .tds_value = {0,   1,    5,   10,  20,   30,   48,   61,   76,   95,   140,  250, 400,  600, 800,  1000}
		.adc_value = {0, 100, 239, 750, 1050, 1500, 1800, 2100, 2400, 2600, 2700, 3000, 3000, 3000, 3000, 3000},
		.tds_value = {0, 1, 5, 12, 22, 40, 50, 67, 96, 117, 132, 172, 172, 172, 172, 172}

};

LOCAL TDS_CONFIG_T s_tds_calib_param;

TDS_T s_tds_in;

TDS_T s_tds_out;

TDS_T s_tds_bu;

int16_t queueAdcSma_tdsOut[ADC_SAMPLE_QUEUE_SIZE];
int16_t queueAdcSma_tdsIn[ADC_SAMPLE_QUEUE_SIZE];
int16_t queueAdcSma_tdsBu[ADC_SAMPLE_QUEUE_SIZE];
bool queueIsEmpty_tdsOut = true;
bool queueIsEmpty_tdsIn = true;
bool queueIsEmpty_tdsBu = true;
LOCAL uint16_t s_200ms_cnt;

LOCAL uint16_t s_adc_h2o_det1 = 0;

LOCAL uint16_t s_cnt_h2o_det1 = 0;

LOCAL bool s_is_h2O_det1 = FALSE;

LOCAL uint16_t s_adc_h2o_det2 = 0;

LOCAL uint16_t s_cnt_h2o_det2 = 0;

LOCAL bool s_is_h2O_det2 = FALSE;

LOCAL bool s_disableUpdateTds = true;

// LOCAL uint16_t s_adc_low_value = 0;

/******************************************************************************
 * Local functions
 ******************************************************************************/
uint16_t ADC_GetTdsValue(TDS_E channel);

LOCAL void ADC_PushDataToQueue(int16_t data, TDS_T *tds, TDS_E channel)
{
	if (channel == TDS_OUT_VALUE)
	{
		uint8_t i;
		if (queueIsEmpty_tdsOut)
		{
			queueIsEmpty_tdsOut = false;
			for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE; i++)
			{
				queueAdcSma_tdsOut[i] = data;
				s_tds_out.sum_tds_adc += data;
				s_tds_out.sma_tds_adc = data;
			}
			return;
		}

		s_tds_out.sum_tds_adc = s_tds_out.sum_tds_adc + data - queueAdcSma_tdsOut[0];
		for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE - 1; i++)
		{
			queueAdcSma_tdsOut[i] = queueAdcSma_tdsOut[i + 1];
		}
		queueAdcSma_tdsOut[ADC_SAMPLE_QUEUE_SIZE - 1] = data;
		s_tds_out.sma_tds_adc = (int16_t)(s_tds_out.sum_tds_adc / ADC_SAMPLE_QUEUE_SIZE);
	}
	else if (channel == TDS_IN_VALUE)
	{
		uint8_t i;
		if (queueIsEmpty_tdsIn)
		{
			queueIsEmpty_tdsIn = false;
			for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE; i++)
			{
				queueAdcSma_tdsIn[i] = data;
				s_tds_in.sum_tds_adc += data;
				s_tds_in.sma_tds_adc = data;
			}
			return;
		}

		s_tds_in.sum_tds_adc = s_tds_in.sum_tds_adc + data - queueAdcSma_tdsIn[0];
		for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE - 1; i++)
		{
			queueAdcSma_tdsIn[i] = queueAdcSma_tdsIn[i + 1];
		}
		queueAdcSma_tdsIn[ADC_SAMPLE_QUEUE_SIZE - 1] = data;
		s_tds_in.sma_tds_adc = (int16_t)(s_tds_in.sum_tds_adc / ADC_SAMPLE_QUEUE_SIZE);
	}
	else
	{
		uint8_t i;
		if (queueIsEmpty_tdsBu)
		{
			queueIsEmpty_tdsBu = false;
			for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE; i++)
			{
				queueAdcSma_tdsBu[i] = data;
				s_tds_bu.sum_tds_adc += data;
				s_tds_bu.sma_tds_adc = data;
			}
			return;
		}

		s_tds_bu.sum_tds_adc = s_tds_bu.sum_tds_adc + data - queueAdcSma_tdsBu[0];
		for (i = 0; i < ADC_SAMPLE_QUEUE_SIZE - 1; i++)
		{
			queueAdcSma_tdsBu[i] = queueAdcSma_tdsBu[i + 1];
		}
		queueAdcSma_tdsBu[ADC_SAMPLE_QUEUE_SIZE - 1] = data;
		s_tds_bu.sma_tds_adc = (int16_t)(s_tds_bu.sum_tds_adc / ADC_SAMPLE_QUEUE_SIZE);
	}
}

LOCAL void ADC_InitConfigFlash()
{
	bool readOk = flash_app_readData((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));

	if (!readOk)
	{
		memcpy(&(s_tds_calib_param.tds_in), &TDS_IN_CONFIG_DEFAULD, sizeof(s_tds_calib_param.tds_in));
		memcpy(&(s_tds_calib_param.tds_out), &TDS_OUT_CONFIG_DEFAULD, sizeof(s_tds_calib_param.tds_out));
		s_tds_calib_param.tds_out_max = TDS_OUT_MAX_DEFAULT;
		s_tds_calib_param.adc_h2o_det = ADC_H2O_DET_DEFAULT;
		//    	flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
	}
}

LOCAL bool ADC_GetIndexCalibFromTds(TDS_E channel, uint16_t tds_value, uint8_t *index_ret)
{

	uint8_t ret_index = 0;
	uint8_t index_level = 0;
	TDS_CALIB_PARAM_T *calib_param;
	if (channel == TDS_IN_VALUE)
	{
		calib_param = &(s_tds_calib_param.tds_in);
	}
	else if (channel == TDS_OUT_VALUE)
	{
		calib_param = &(s_tds_calib_param.tds_out);
	}
	else
	{
		calib_param = &(TDS_BU_CONFIG_DEFAULD);
	}
	if (tds_value > calib_param->tds_value[CALIB_POINT_MAX - 1])
		return false;
	for (index_level = 0; index_level < (CALIB_POINT_MAX - 1); index_level++)
	{
		if ((tds_value >= calib_param->tds_value[index_level]) && (tds_value < calib_param->tds_value[index_level + 1]))
		{
			if (((2 * tds_value) > (calib_param->tds_value[index_level] + calib_param->tds_value[index_level + 1])) || ((calib_param->tds_value[index_level] == 0) && (tds_value != 0)))
				ret_index = index_level + 1;
			else
				ret_index = index_level;
			break;
		}
	}
	*index_ret = ret_index;
	return true;
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
PUBLIC void ADC_Init()
{
	//	R_Config_S12AD0_Create();
	s_tds_in.adc_sample = NULL;	 // QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	s_tds_out.adc_sample = NULL; // QUEUE_InitQueue(ADC_SAMPLE_QUEUE_SIZE,sizeof(int16_t));
	s_tds_bu.adc_sample = NULL;
	ADC_InitConfigFlash();
	s_200ms_cnt = 0;
	//	PWM = 0;
	g_adc_flag = 0U;
#ifdef ENABLE_TDS_OUT

	//	R_Config_S12AD0_Start();
	while (queueIsEmpty_tdsOut || queueIsEmpty_tdsIn || queueIsEmpty_tdsBu)
	{
		if (g_adc_flag)
		{
			ADC_UpdateTds(0);
		}
	}
	s_tds_out.tds_display = curentData_getLastTdsOut();
#else
	s_tds_out.tds_display = 0;
#endif
}

PUBLIC uint16_t ADC_GetTdsValue(TDS_E channel)
{
#ifndef ENABLE_TDS_OUT
	return 0;
#endif

	//	float calculate_value = 0;
	//	float slope;
	uint16_t tds_return = 0;
	uint32_t index_level = 0;
	TDS_CALIB_PARAM_T *calib_param;
	if (channel == TDS_IN_VALUE)
	{
		calib_param = &(s_tds_calib_param.tds_in);
	}
	else if (channel == TDS_OUT_VALUE)
	{
		calib_param = &(s_tds_calib_param.tds_out);
	}
	else
	{
		// tds calib theo nhiet do nua nen bang ADC se tinh lai theo nhiet do hien tai
		userAdc_getNewTableAdcFromTemp(&(TDS_BU_CONFIG_DEFAULD.adc_value[0]), userAdc_getTemNong());
		calib_param = &TDS_BU_CONFIG_DEFAULD;
	}
	int16_t adc0_value;
	if (channel == TDS_IN_VALUE)
	{
		if (g_disableTdsIn)
		{
			return 0;
		}
		if (queueIsEmpty_tdsIn)
			return 0;
		adc0_value = s_tds_in.sma_tds_adc;
	}
	else if (channel == TDS_OUT_VALUE)
	{
		if (g_disableTdsOut)
		{
			return 0;
		}
		if (queueIsEmpty_tdsOut)
			return 0;
		adc0_value = s_tds_out.sma_tds_adc;
	}
	else
	{
		if (g_disableTdsBu)
		{
			return 0;
		}
		if (queueIsEmpty_tdsBu)
			return 0;
		adc0_value = s_tds_bu.sma_tds_adc;
	}
	if (adc0_value < calib_param->adc_value[0])
	{
		tds_return = 1;
		return tds_return;
	}

	if (adc0_value >= calib_param->adc_value[CALIB_POINT_MAX - 1])
	{
		tds_return = calib_param->tds_value[CALIB_POINT_MAX - 1];
		if (tds_return == 0)
			tds_return = 1;
		return tds_return;
	}

	int32_t y1, y2;
	int16_t x1, x2;
	int32_t y;
	for (index_level = 0; index_level < (CALIB_POINT_MAX - 1); index_level++)
	{

		if ((adc0_value >= calib_param->adc_value[index_level]) && (adc0_value < calib_param->adc_value[index_level + 1]))
		{
			x1 = calib_param->adc_value[index_level];
			y1 = calib_param->tds_value[index_level];
			x2 = calib_param->adc_value[index_level + 1];
			y2 = calib_param->tds_value[index_level + 1];
			if (x2 != x1)
				y = ((int32_t)(y2 - y1)) * (adc0_value - x1) / (x2 - x1) + y1;
			tds_return = (y < 1) ? 1 : (uint16_t)(y);
			//			slope = ((float)(calib_param->tds_value[index_level+1] - calib_param->tds_value[index_level]))/((float)(calib_param->adc_value[index_level] - calib_param->adc_value[index_level+1]));
			//			calculate_value = calib_param->tds_value[index_level] + slope * ( calib_param->adc_value[index_level] - adc0_value);
			//			tds_return= (calculate_value <= 1)?1:(uint16_t) (calculate_value+0.5);
			break;
		}
	}

	return tds_return;
}

int32_t util_interpolateInt(int32_t y1, int32_t x1, int32_t y2, int32_t x2, int32_t x)
{
	if (x2 == x1)
		return 0;
	return ((y2 - y1) * (x - x1) / (x2 - x1) + y1); // return y
}

uint16_t tableTempRes[100][2] =
	{
		{0, 32740},
		{1, 31110},
		{2, 29580},
		{3, 28120},
		{4, 26750},
		{5, 25450},
		{6, 24220},
		{7, 23060},
		{8, 21960},
		{9, 20920},
		{10, 19940},
		{11, 19000},
		{12, 18120},
		{13, 17280},
		{14, 16490},
		{15, 15730},
		{16, 15020},
		{17, 14340},
		{18, 13690},
		{19, 13080},
		{20, 12500},
		{21, 11950},
		{22, 11420},
		{23, 10930},
		{24, 10450},
		{25, 10000},
		{26, 9570},
		{27, 9160},
		{28, 8770},
		{29, 8400},
		{30, 8050},
		{31, 7720},
		{32, 7400},
		{33, 7090},
		{34, 6800},
		{35, 6520},
		{36, 6260},
		{37, 6010},
		{38, 5760},
		{39, 5530},
		{40, 5320},
		{41, 5110},
		{42, 4910},
		{43, 4710},
		{44, 4530},
		{45, 4360},
		{46, 4190},
		{47, 4030},
		{48, 3880},
		{49, 3730},
		{50, 3590},
		{51, 3460},
		{52, 3330},
		{53, 3200},
		{54, 3090},
		{55, 2970},
		{56, 2870},
		{57, 2760},
		{58, 2660},
		{59, 2570},
		{60, 2480},
		{61, 2390},
		{62, 2300},
		{63, 2220},
		{64, 2150},
		{65, 2070},
		{66, 2000},
		{67, 1930},
		{68, 1870},
		{69, 1800},
		{70, 1740},
		{71, 1680},
		{72, 1630},
		{73, 1570},
		{74, 1520},
		{75, 1470},
		{76, 1420},
		{77, 1380},
		{78, 1330},
		{79, 1290},
		{80, 1250},
		{81, 1210},
		{82, 1170},
		{83, 1130},
		{84, 1100},
		{85, 1060},
		{86, 1030},
		{87, 1000},
		{88, 970},
		{89, 940},
		{90, 910},
		{91, 880},
		{92, 850},
		{93, 830},
		{94, 800},
		{95, 780},
		{96, 760},
		{97, 740},
		{98, 710},
		{99, 690},
};
// neu dung cong thuc
// log(r/r25)
//       |
// 2     |
// 1     |____25_____________________________
// 0     |                         |
//-1    |------------------------100
//-2    |

uint16_t userAdc_calculateTemFromTable(uint16_t r_ntc_Ohm)
{
	//    float temRet = 0;
	//    float tableLogResFollowTem[3][2] = {{0.2,0},{0,25},{-1,100}};
	//    float logRes = 1;//log10(r_ntc_kOhm/10);
	//    if(logRes >= tableLogResFollowTem[2][0] && logRes < tableLogResFollowTem[1][0])
	//    {
	//        temRet = util_interpolate(tableLogResFollowTem[2][1], tableLogResFollowTem[2][0], tableLogResFollowTem[1][1], tableLogResFollowTem[1][0], logRes);
	//    }
	//    else if(logRes >= tableLogResFollowTem[1][0] && logRes < tableLogResFollowTem[0][0])
	//    {
	//        temRet = util_interpolate(tableLogResFollowTem[1][1], tableLogResFollowTem[1][0], tableLogResFollowTem[0][1], tableLogResFollowTem[0][0], logRes);
	//    }
	//    else{
	//        temRet = 0;
	//    }
	//    return temRet;

	if (r_ntc_Ohm >= tableTempRes[0][1])
		return tableTempRes[0][0] * 10;
	else if (r_ntc_Ohm <= tableTempRes[99][1])
		return tableTempRes[99][0] * 10;
	else
	{
		uint8_t i = 0;
		for (i = 0; i < 99; i++)
		{
			if ((tableTempRes[i][1] >= r_ntc_Ohm) && (tableTempRes[i + 1][1] < r_ntc_Ohm))
			{
				//                if((tableTempRes[i][1] - r_ntc_Ohm) < (r_ntc_Ohm - tableTempRes[i+1][1]))
				//                {
				//                    return tableTempRes[i][0];
				//                }
				//                else
				//                {
				//                    return tableTempRes[i+1][0];
				//                }
				return (uint16_t)util_interpolateInt((int32_t)tableTempRes[i][0] * 10, (int32_t)tableTempRes[i][1], (int32_t)tableTempRes[i + 1][0] * 10, (int32_t)tableTempRes[i + 1][1], (int32_t)r_ntc_Ohm);
			}
		}
	}
	return 1;
}
void userAdc_calculateTemperatureNong()
{
	const uint32_t R_serial_pullup = 10000; // Ohm
	if (s_adcTempNong > 4090 || s_adcTempNong < 5)
	{
		temNong = 250; // neu k noi cam bien coi  = default
		return;
	}
	uint32_t ntc_nong = (uint32_t)s_adcTempNong * 3300 / 4095;
	uint32_t ntc_res_nong = ((uint32_t)(3300 - ntc_nong)) * 1000 / R_serial_pullup;
	if (ntc_res_nong == 0)
	{
		return;
	}
	ntc_res_nong = (uint32_t)3300 * 1000 / ntc_res_nong - R_serial_pullup;
	if (ntc_res_nong > 65535)
	{
		ntc_res_nong = 65535;
	}
	uint16_t result = userAdc_calculateTemFromTable(ntc_res_nong);
	if (result > 1000)
		temNong = 1000;
	else
		temNong = result;
}
void userAdc_calculateTemperatureLanh()
{
	const uint16_t R_serial_pulldown = 10000; // Ohm
	uint32_t ntc_lanh = (uint32_t)s_adcTempLanh * 3300 / 4095;
	if (ntc_lanh == 0)
		return;
	if (ntc_lanh > 3300)
		return;
	uint32_t ntc_res_lanh = ((uint32_t)(3300 - ntc_lanh)) * R_serial_pulldown / (ntc_lanh);
	if (ntc_res_lanh == 0)
		return;
	if (ntc_res_lanh > 65535)
	{
		ntc_res_lanh = 65535;
	}
	uint16_t result = userAdc_calculateTemFromTable((uint16_t)ntc_res_lanh);
	if (result > 1000)
		temLanh = 1000;
	else
		temLanh = result;
}

uint32_t userAdc_calculateFB()
{
	return s_adcFb;
}

uint16_t userAdc_getTemNong()
{
	return temNong;
}
uint16_t userAdc_getTemLanh()
{
	return temLanh;
}

#define MAX_POINT_TEMP 10
uint16_t tempList[MAX_POINT_TEMP] = { //*10
	100,
	120,
	140,
	160,
	180,
	200,
	220,
	250,
	280,
	300};
int16_t tableadcWithTempMap[MAX_POINT_TEMP][CALIB_POINT_MAX] =
	{
		{0, 1219, 1938, 2062, 2201, 2306, 2240, 2284, 2349, 2387, 2571, 2571, 2571, 2571, 2571, 2571},
		{0, 1258, 1982, 2103, 2219, 2316, 2276, 2315, 2385, 2421, 2598, 2598, 2598, 2598, 2598, 2598},
		{0, 1304, 2028, 2144, 2253, 2308, 2312, 2353, 2420, 2466, 2622, 2622, 2622, 2622, 2622, 2622},
		{0, 1331, 2064, 2150, 2298, 2360, 2375, 2386, 2446, 2503, 2654, 2654, 2654, 2654, 2654, 2654},
		{0, 1343, 2070, 2199, 2315, 2360, 2396, 2438, 2576, 2580, 2685, 2685, 2685, 2685, 2685, 2685},
		{0, 1373, 2112, 2263, 2371, 2471, 2480, 2488, 2583, 2587, 2705, 2705, 2705, 2705, 2705, 2705},
		{0, 1390, 2146, 2300, 2465, 2470, 2530, 2563, 2613, 2663, 2725, 2725, 2725, 2725, 2725, 2725},
		{0, 1431, 2203, 2365, 2495, 2504, 2549, 2625, 2665, 2711, 2748, 2748, 2748, 2748, 2748, 2748},
		{0, 1541, 2295, 2450, 2525, 2630, 2647, 2663, 2705, 2725, 2773, 2773, 2773, 2773, 2773, 2773},
		{0, 1677, 2337, 2487, 2562, 2689, 2659, 2684, 2718, 2752, 2795, 2795, 2795, 2795, 2795, 2795},

};
void userAdc_getNewTableAdcFromTemp(int16_t *tableAdc, uint16_t temp)
{
	uint8_t i;
	uint8_t idx = 0;
	if (temp <= tempList[0])
	{
		idx = 0;
		memcpy(tableAdc, tableadcWithTempMap[idx], CALIB_POINT_MAX * sizeof(int16_t));
		return;
	}
	else if (temp >= tempList[MAX_POINT_TEMP - 1])
	{
		idx = MAX_POINT_TEMP - 1;
		memcpy(tableAdc, tableadcWithTempMap[idx], CALIB_POINT_MAX * sizeof(int16_t));
		return;
	}
	for (i = 0; i < MAX_POINT_TEMP; i++)
	{
		if (temp > tempList[i] && temp <= tempList[i + 1])
		{
			uint16_t deltaLow = temp - tempList[i];
			uint16_t deltaHigh = tempList[i + 1] - temp;
			if (deltaLow < deltaHigh)
			{
				memcpy(tableAdc, tableadcWithTempMap[i], CALIB_POINT_MAX * sizeof(int16_t));
			}
			else
			{
				memcpy(tableAdc, tableadcWithTempMap[i + 1], CALIB_POINT_MAX);
			}
			return;
		}
	}
}

PUBLIC uint16_t ADC_GetTdsValueDisplay(TDS_E channel)
{
#ifndef ENABLE_TDS_OUT
	return 0;
#endif
	if (channel == TDS_IN_VALUE)
	{
		return s_tds_in.tds_display;
	}
	else if (channel == TDS_OUT_VALUE)
	{
		return s_tds_out.tds_display;
	}
	else
	{
		return s_tds_bu.tds_display;
	}
}

bool ADC_TdsOutIsNewValue()
{
	static uint16_t oldTdsValue = 0;
	if (oldTdsValue != s_tds_out.tds_display)
	{
		oldTdsValue = s_tds_out.tds_display;
		return true;
	}
	return false;
}

void debugADC(int16_t adcIn, int16_t adcOut)
{
	//	char dbg[UART_SEND_MAX_LEN];
	//	sprintf(dbg,"[%d,%d]",adcIn,adcOut);
	////	sprintf(dbg,"%d,%d\r\n",adcIn,adcOut);
	//	UART_Debug (dbg);
}
int16_t ADC_getAdcTdsOutDebug()
{
	return current_adc_tds_out;
}
int16_t ADC_getAdcTdsInDebug()
{
	return current_adc_tds_in;
}
PUBLIC void ADC_UpdateTds(uint8_t state)
{
	s_200ms_cnt = s_200ms_cnt + 1;
	if (s_200ms_cnt >= ADC_SAMPLE_CAL_MAX)
	{
		s_200ms_cnt = 0;
		// update tds bu
		current_adc_tds_bu = ((s_tds_bu.high_cnt == 0) | (s_tds_bu.low_cnt == 0)) ? 0 : ((s_tds_bu.sum_adc_high / s_tds_bu.high_cnt) - (s_tds_bu.sum_adc_low / s_tds_bu.low_cnt));
		ADC_PushDataToQueue(current_adc_tds_bu, &s_tds_bu, TDS_BU_VALUE);

		// update tds in
		current_adc_tds_in = ((s_tds_in.high_cnt == 0) | (s_tds_in.low_cnt == 0)) ? 0 : ((s_tds_in.sum_adc_high / s_tds_in.high_cnt) - (s_tds_in.sum_adc_low / s_tds_in.low_cnt));
		ADC_PushDataToQueue(current_adc_tds_in, &s_tds_in, TDS_IN_VALUE);

		//	update tds out
		current_adc_tds_out = ((s_tds_out.high_cnt == 0) | (s_tds_out.low_cnt == 0)) ? 0 : ((s_tds_out.sum_adc_high / s_tds_out.high_cnt) - (s_tds_out.sum_adc_low / s_tds_out.low_cnt));
		ADC_PushDataToQueue(current_adc_tds_out, &s_tds_out, TDS_OUT_VALUE);

		//		debugADC(s_tds_out.sma_tds_adc,adc_tds_out);
		debugADC(current_adc_tds_in, current_adc_tds_out);
		s_tds_bu.high_cnt = 0;
		s_tds_bu.low_cnt = 0;
		s_tds_bu.sum_adc_high = 0;
		s_tds_bu.sum_adc_low = 0;
		s_tds_in.high_cnt = 0;
		s_tds_in.low_cnt = 0;
		s_tds_in.sum_adc_high = 0;
		s_tds_in.sum_adc_low = 0;
		s_tds_out.high_cnt = 0;
		s_tds_out.low_cnt = 0;
		s_tds_out.sum_adc_high = 0;
		s_tds_out.sum_adc_low = 0;
		//	    check h2o det
		R_Config_S12AD0_Get_ValueResult(H20_CHANNEL_DETECT_1, &s_adc_h2o_det1);
		if (s_adc_h2o_det1 < s_tds_calib_param.adc_h2o_det)
		{
			s_cnt_h2o_det1++;
			if (s_cnt_h2o_det1 > H2O_DET_CNT_MAX)
			{
				s_is_h2O_det1 = TRUE;
				s_cnt_h2o_det1 = 0;
			}
		}
		else
		{
			s_is_h2O_det1 = FALSE;
			s_cnt_h2o_det1 = 0;
		}

		goto end_function;
	}
	R_Config_S12AD0_Get_ValueResult(TDS_IN_CHANNEL, &adc_result_tds_in);
	R_Config_S12AD0_Get_ValueResult(TDS_OUT_CHANNEL, &adc_result_tds_out);
	R_Config_S12AD0_Get_ValueResult(TDS_BU_CHANNEL, &adc_result_tds_bu);

	uint16_t valueAdc;
	R_Config_S12AD0_Get_ValueResult(TEMP_NONG_CHANNEL, &valueAdc);
	static uint8_t cntAvg_nong = 0;
	cntAvg_nong++;
	adc_result_nong += (uint16_t)valueAdc;
	if (cntAvg_nong >= 5)
	{
		s_adcTempNong = adc_result_nong / cntAvg_nong;
		adc_result_nong = 0;
		cntAvg_nong = 0;
	}
	R_Config_S12AD0_Get_ValueResult(ADC_FB_CHANNEL, &valueAdc);
	static uint8_t cntAvg_fb = 0;
	cntAvg_fb++;
	adc_result_fb += (uint16_t)valueAdc;
	if (cntAvg_fb >= 5)
	{
		s_adcFb = adc_result_fb / cntAvg_fb;
		adc_result_fb = 0;
		cntAvg_fb = 0;
	}

	if (g_pwm_value == 0)
	{
		lastLogicPwm = 0;
		s_tds_out.low_cnt++;
		s_tds_out.sum_adc_low += adc_result_tds_out;
		s_tds_in.low_cnt++;
		s_tds_in.sum_adc_low += adc_result_tds_in;
		s_tds_bu.low_cnt++;
		s_tds_bu.sum_adc_low += adc_result_tds_bu;
	}
	else
	{
		lastLogicPwm = 1;
		s_tds_out.high_cnt++;
		s_tds_out.sum_adc_high += adc_result_tds_out;
		s_tds_in.high_cnt++;
		s_tds_in.sum_adc_high += adc_result_tds_in;
		s_tds_bu.high_cnt++;
		s_tds_bu.sum_adc_high += adc_result_tds_bu;
	}

end_function:

	g_adc_flag = 0U;
}

PUBLIC ERR_E ADC_CalibTdsValue(uint16_t tdsvalue, TDS_E channel)
{
	uint8_t index = 0;
	ERR_E f_ret = OK;
	bool ret = false;
	if (ADC_GetIndexCalibFromTds(channel, tdsvalue, &index) == true)
	{
		if (channel == TDS_IN_VALUE)
		{
			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
			//			s_tds_calib_param.tds_in.adc_value[index] = current_adc_tds_in;
			s_tds_calib_param.tds_in.adc_value[index] = s_tds_in.sma_tds_adc;
		}
		else if (channel == TDS_OUT_VALUE)
		{
			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
			//			s_tds_calib_param.tds_out.adc_value[index] = current_adc_tds_out;
			s_tds_calib_param.tds_out.adc_value[index] = s_tds_out.sma_tds_adc;
		}
		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
		f_ret = (ret == true) ? OK : ERR;
		return f_ret;
	}
	else
		return ERR;
}

PUBLIC void ADC_ClearH2oDet()
{
	s_is_h2O_det1 = 0;
	s_is_h2O_det2 = 0;
}

PUBLIC bool ADC_GetH2oDet(H2O_T index)
{
	return false;
	if (index == H2O_1)
		return s_is_h2O_det1;
	if (index == H2O_2)
		return s_is_h2O_det2;
	return true;
}

PUBLIC void ADC_UpdateTdsDisplay()
{
	s_tds_in.tds_display = ADC_GetTdsValue(TDS_IN_VALUE);
	s_tds_bu.tds_display = ADC_GetTdsValue(TDS_BU_VALUE);
	if (s_disableUpdateTds)
	{
		return;
	}
	// nếu tds vượt ngưỡng, 2 lần mới hiển thị
	static uint8_t tdsLimitCnt = 0;
	if (ADC_GetTdsValue(TDS_OUT_VALUE) > g_userConfig.tdsLimitOut)
	{
		if (tdsLimitCnt < 1)
		{
			tdsLimitCnt++;
			return;
		}
	}
	else
	{
		tdsLimitCnt = 0;
	}
	// ----------------------------------------
	s_tds_out.tds_display = ADC_GetTdsValue(TDS_OUT_VALUE);
	curentData_updateTdsStore(s_tds_out.tds_display);
}

PUBLIC ERR_E ADC_GetCalibTdsParam(TDS_E channel, char *out)
{
	//	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	//	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	//	{
	//		char calibStr[20]= "";
	//		sprintf(calibStr,"(%d,%d)",calib_param->tds_value[i],calib_param->adc_value[i]);
	//		strcat((char*)out,calibStr);
	//	}
	return OK;
}

PUBLIC ERR_E ADC_GetAdcTable(TDS_E channel, char *out)
{
	//	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	//	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	//	{
	//		char calibStr[20]= "";
	//		if(i == (CALIB_POINT_MAX - 1))
	//			sprintf(calibStr,"%d",calib_param->adc_value[i]);
	//		else
	//			sprintf(calibStr,"%d,",calib_param->adc_value[i]);
	//		strcat((char*)out,calibStr);
	//	}
	return OK;
}

PUBLIC ERR_E ADC_GetTdsTable(TDS_E channel, char *out)
{
	//	TDS_CALIB_PARAM_T   *calib_param = (channel  == TDS_IN_VALUE)?&(s_tds_calib_param.tds_in): &(s_tds_calib_param.tds_out);
	//	for(uint8_t i = 0; i< CALIB_POINT_MAX; i++)
	//	{
	//		char calibStr[20]= "";
	//		if(i == (CALIB_POINT_MAX - 1))
	//			sprintf(calibStr,"%d",calib_param->tds_value[i]);
	//		else
	//			sprintf(calibStr,"%d,",calib_param->tds_value[i]);
	//		strcat((char*)out,calibStr);
	//	}
	return OK;
}

PUBLIC ERR_E ADC_CalibTdsValueFromUart(uint16_t tdsvalue, TDS_E channel, uint8_t index)
{
	ERR_E f_ret = OK;
	//	bool ret = false;
	//	if(index > (CALIB_POINT_MAX -1))
	//	{
	//		return ERR;
	//	}
	//	else
	//	{
	//		if(channel  == TDS_IN_VALUE)
	//		{
	//			s_tds_calib_param.tds_in.tds_value[index] = tdsvalue;
	//			s_tds_calib_param.tds_in.adc_value[index] = (index == 0)?(s_tds_in.sma_tds_adc - ZERO_CALIB) :s_tds_in.sma_tds_adc;
	//		}
	//		else if(channel  == TDS_OUT_VALUE)
	//		{
	//			s_tds_calib_param.tds_out.tds_value[index] = tdsvalue;
	//			s_tds_calib_param.tds_out.adc_value[index] = (index == 0)?(s_tds_out.sma_tds_adc -ZERO_CALIB):s_tds_out.sma_tds_adc;
	//		}
	//		ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
	//		f_ret = (ret == true)?OK:ERR;
	return f_ret;
	//	}
}

PUBLIC ERR_E ADC_CalibTdsAutoZero()
{
	ERR_E f_ret = OK;
	//	bool ret = false;
	//	int16_t delta = (s_tds_out.sma_tds_adc -ZERO_CALIB) - s_tds_calib_param.tds_out.adc_value[0];
	//	for(uint8_t i = 0; i< CALIB_POINT_MAX ; i ++)
	//	{
	//		s_tds_calib_param.tds_out.adc_value[i] += delta;
	//	}
	//	ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, TDS_PARAM_BLOCK, sizeof(s_tds_calib_param));
	//	f_ret = (ret == true)?OK:ERR;
	return f_ret;
}

PUBLIC ERR_E ADC_WriteAdcFromUart(int *adcTable, TDS_E channel)
{
	ERR_E f_ret = OK;
	//	bool ret = false;
	//	if(channel  == TDS_OUT_VALUE)
	//	{
	//		for(uint8_t i=0; i < CALIB_POINT_MAX; i++)
	//		{
	//			s_tds_calib_param.tds_out.adc_value[i] = (int16_t)adcTable[i];
	//		}
	//	}
	//	flash_block_address_t block = TDS_PARAM_BLOCK;
	//	ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, block, sizeof(s_tds_calib_param));
	//	f_ret = (ret == true)?OK:ERR;
	return f_ret;
}

PUBLIC ERR_E ADC_WriteTdsFromUart(int *tdsTable, TDS_E channel)
{
	ERR_E f_ret = OK;
	//	bool ret = false;
	//	if(channel  == TDS_OUT_VALUE)
	//	{
	//		for(uint8_t i=0; i < CALIB_POINT_MAX; i++)
	//		{
	//			s_tds_calib_param.tds_out.tds_value[i] = (int16_t)tdsTable[i];
	//		}
	//	}
	//	flash_block_address_t block = TDS_PARAM_BLOCK;
	//	ret = flash_app_writeBlock((uint8_t *)&s_tds_calib_param, block, sizeof(s_tds_calib_param));
	//	f_ret = (ret == true)?OK:ERR;
	return f_ret;
}

PUBLIC bool ADC_SetDisableUpdateTds(bool disable)
{
	if (s_disableUpdateTds == disable)
		return false;
	else
	{
		s_disableUpdateTds = disable;
		return true;
	}
}

PUBLIC void ADC_process()
{
	//	if((g_sysTime >= 60000) && (g_sysTime < 65000) && (s_tds_out.tds_display == 0))
	//	{
	//		s_tds_out.tds_display = ADC_GetTdsValue(TDS_OUT_VALUE);
	//	}
	if (g_adc_flag)
	{
		ADC_UpdateTds(0);
	}
}

void ADC_displayTask()
{
	static uint32_t timeStart = 0;
	//    sprintf(bufTds,"adc1:%d tdsout: %d, adc2:%d tds3000: %d, adc3: %d tdsin: %d, AT:%d AC:%d\n",s_tds_out.sma_tds_adc, s_tds_out.tds_display,s_tds_bu.sma_tds_adc, s_tds_bu.tds_display,s_tds_in.sma_tds_adc, s_tds_in.tds_display, CHECK_CO_AP_THAP,CHECK_CO_AP_CAO);
	//    UART_sendFrame((uint8_t*)bufTds, strlen(bufTds));
	if (elapsedTime(g_sysTime, timeStart) > 5000)
	{
		ADC_UpdateTdsDisplay();
		timeStart = g_sysTime;
	}
}
