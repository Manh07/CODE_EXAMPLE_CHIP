/**
 ******************************************************************************
 * @file    heatControl.h
 * @author  Makipos Co.,LTD.
 * @version 1.0
 * @date    Jan 8, 2021
 * @brief   
 * @history
 ******************************************************************************/


#ifndef APPS_HEATCONTROL_HEATCONTROL_H_
#define APPS_HEATCONTROL_HEATCONTROL_H_

/* Includes ------------------------------------------------------------------*/
#include "stdint.h"
#include "stdbool.h"
#include "voiceCmd.h"
/* Exported types ------------------------------------------------------------*/
typedef enum
{
	TEM_IDX_SETTING_MIN = 0,
	TEM_IDX_SETTING_RO = 0,
	TEM_IDX_SETTING_45,
	TEM_IDX_SETTING_75,
	TEM_IDX_SETTING_99,
	TEM_IDX_SETTING_MAX
} temSetting_t;

typedef enum
{
	AMOUNT_IDX_SETTING_MIN = 0,
	AMOUNT_IDX_SETTING_180 = 0,
	AMOUNT_IDX_SETTING_300,
	AMOUNT_IDX_SETTING_900,
	AMOUNT_IDX_SETTING_MAX
} amountWaterSetting_t;

enum
{
	BYTE_CONTROL_WATER_OFF = 0x00,
	BYTE_CONTROL_WATER_OUT = 0x01,
	BYTE_CONTROL_WATER_OUTING = 0x80
};

typedef enum
{
	HEATCONTROL_STATE_NONE = 0,
	HEATCONTROL_STATE_CONTROL,
	HEATCONTROL_STATE_INQUIRE,
	HEATCONTROL_STATE_MAX
} heatControl_state_t;;

typedef enum
{
	HEAT_ERR_E1 = 0,
	HEAT_ERR_E2,
	HEAT_ERR_E9,
	HEAT_ERR_E5,
	HEAT_ERR_H1,
	HEAT_ERR_H2,
	HEAT_ERR_H4,
	HEAT_ERR_H3,
	HEAT_ERR_H6,
	HEAT_ERR_H5,
	HEAT_ERR_E7,
	HEAT_ERR_Eb,
	HEAT_ERR_Eu,
	HEAT_ERR_MAX
} heatControl_error_t;
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define CMD_RSP_A5 0x00
#define CMD_RSP_E5_B1 0xB1
#define CMD_RSP_AA_00_1 0xBB
#define CMD_RSP_AA_00_2 0xCC

#define TEM_SETTING_RO 25
#define AMOUNT_SETTING_RO 0xFF00
/* Exported functions ------------------------------------------------------- */
uint8_t heatControl_getTemSetting();
uint16_t heatControl_getAmountSetting();
uint16_t heatControl_getCurrentAmountWater();
void heatControl_settingTemNext();
void heatControl_settingAmountNext();
void heatControl_requestWaterOut();
void heatControl_decodeResponeInquire(uint8_t* frame, uint16_t len);
void heatControl_task();
bool heatControl_waterIsOuting();
void heatControl_requestWaterRO();
bool heatControl_isROState();
void heatControl_getCode7SegFromErr(uint8_t* code1Idx, uint8_t* code2Idx, uint8_t errorShow);
bool heatControl_haveError();
heatControl_error_t heatControl_getErrorNeedShow();
void heatControl_requestWaterSettingDefault();
void heatControl_requestWaterSettingVoiceCommand(voiceCmd_waterType_t type);
void heatControl_requestWaterSettingVoiceCommandOnline(uint16_t amount, uint8_t tem);
uint8_t heatControl_getSettingTemDefaultIdx();
uint8_t heatControl_getSettingAmountDefaultIdx();
void heatControl_setSettingDefaultIdx(uint8_t idxTem, uint8_t idxAmount);
#endif /* APPS_HEATCONTROL_HEATCONTROL_H_ */
