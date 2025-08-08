/******************************************************************************
 *
 * M1 Communication Inc.
 * (c) Copyright 2016 M1 Communication, Inc.
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************/
/**
 *
 * @file         Config.h
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

#ifndef CONFIG_H_
#define CONFIG_H_

/******************************************************************************
 * Includes
 ******************************************************************************/

#include "platform.h"
#include "string.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdbool.h"
/******************************************************************************
 * Constants
 ******************************************************************************/

/******************************************************************************
 * Macros
 ******************************************************************************/

#define PUBLIC
#define LOCAL static

#define uint8 uint8_t
#define uint16 uint16_t
#define uint32 uint32_t
#define TRUE 1
#define FALSE 0
#define PNULL 0
// #define DEBUG_ENABLE
#define MAX_TRACE_LEN (120)
#define ARR_SIZE(_a) (sizeof((_a)) / sizeof((_a[0])))
enum
{
	HARD_VER_NUM_NOMAL = 1,
	HARD_VER_NUM_5V = 2,
	HARD_VER_NUM_30 = 3
};

// define ver hardware
// HARD_VER_NOMAL
// HARD_VER_5V
// HARD_VER_30, dung chung cho ca AIOTEC43 va cac model k co wifi
#define HARD_VER_30
// auto define hard ver num
#if defined(HARD_VER_30)
#define HARD_VER_NUM 19 // bu-renesas
#elif defined(HARD_VER_5V)
#define HARD_VER_NUM HARD_VER_NUM_5V
#elif defined(HARD_VER_NOMAL)
#define HARD_VER_NUM HARD_VER_NUM_NOMAL
#else
#error "error define something"
#endif

#define VERSION_SOFTWARE 14
#define FILTER_NUM_MAX 9
// define model **********************************************************************
// MODEL_LEAF , MODEL_888_TN , MODEL_888_TC , MODEL_IRO3_K92
// MODEL_OPTIMUS_I1, MODEL_OPTIMUS_I2, MODEL_IQ_IRO2 , MODEL_TOPBOX, MODEL_AIOTEC_4_3, MODEL_D68, MODEL_D88
#define MODEL_AIOTEC_4_3

// số lượng lõi
#define FILTER_NUM 9
// ***********************************************************************************

// TỰ ĐỘNG DEFINE THEO MODEL MÁY
// nhà sản xuất tương ứng với model máy
// KAROFI : MODEL_OPTIMUS_I1, MODEL_OPTIMUS_I2,MODEL_IQ_IRO2 , MODEL_TOPBOX, MODEL_AIOTEC_4_3
// KAROHOME : MODEL_LEAF , MODEL_888_TN , MODEL_888_TC , MODEL_IRO3_K92
#if defined(MODEL_OPTIMUS_I1) || defined(MODEL_OPTIMUS_I2) || defined(MODEL_IQ_IRO2) || defined(MODEL_TOPBOX) || defined(MODEL_AIOTEC_4_3)
#define KAROFI
#endif
#if defined(MODEL_LEAF) || defined(MODEL_888_TN) || defined(MODEL_888_TC) || defined(MODEL_IRO3_K92)
#define KAROHOME
#endif
// define có bật tds out hay không tương ứng với từng model máy
#if defined(MODEL_LEAF) || defined(MODEL_IRO3_K92) || defined(MODEL_OPTIMUS_I2) || defined(MODEL_IQ_IRO2) || defined(MODEL_TOPBOX) || defined(MODEL_AIOTEC_4_3)
#define ENABLE_TDS_OUT
#endif

// Tắt tính năng dò nước với các model 888-TN, 888-TC, optimus I1
#if defined(MODEL_888_TN) || defined(MODEL_888_TC) || defined(MODEL_OPTIMUS_I1)
#define DISABLE_H2O_DET
#endif

#if defined(MODEL_TOPBOX)
#define ENABLE_FLOAT
#endif

// ***********************************************************************************
// thời gian chờ trước khi hiện tds với mạch Karofi. Với mạch Korihome không có ảnh hưởng.
#define WAIT_TIME_UPDATE_TDS 30 // s

#define TEST_SPEED 1 // for testing purpose. 1 day is like 1 year or more.

enum
{
	MODEL_NUM_LEAF_9 = 1,
	MODEL_NUM_LEAF_5 = 2,
	MODEL_NUM_LEAF_6 = 3,
	MODEL_NUM_888_TN_9 = 4,
	MODEL_NUM_888_TN_6 = 5,
	MODEL_NUM_888_TC_9 = 6,
	MODEL_NUM_888_TC_7 = 7,
	MODEL_NUM_888_TC_8 = 8,
	MODEL_NUM_IRO3_K92 = 9,
	MODEL_NUM_OPTIMUS_I1 = 10,
	MODEL_NUM_OPTIMUS_I2 = 11,
	MODEL_NUM_TOPBOX = 12,
	MODEL_NUM_IQ_IRO2 = 13,
	MODEL_NUM_AIOTEC_4_3 = 14,
	MODEL_NUM_D68 = 15,
	MODEL_NUM_D88 = 16,
	MODEL_NUM_VOICE = 20,
	MODEL_NUM_PWR_VOICE = 31,
	MODEL_NUM_PWR_BU = 32,
	MODEL_NUM_PWR_S85_LCD = 33,
	MODEL_NUM_PWR_S88_LCD = 34,
	MODEL_NUM_PWR_S88_LCD_PRM = 88
};
// MODEL_LEAF , MODEL_888_TN , MODEL_888_TC , MODEL_IRO3_K92
// MODEL_OPTIMUS_I1, MODEL_OPTIMUS_I2 , MODEL_TOPBOX

#if defined(MODEL_LEAF) && (FILTER_NUM == 9)
#define MODEL_NUM MODEL_NUM_LEAF_9
//
#elif defined(MODEL_LEAF) && (FILTER_NUM == 5)
#define MODEL_NUM MODEL_NUM_LEAF_5
//
#elif defined(MODEL_LEAF) && (FILTER_NUM == 6)
#define MODEL_NUM MODEL_NUM_LEAF_6
//
#elif defined(MODEL_888_TN) && (FILTER_NUM == 9)
#define MODEL_NUM MODEL_NUM_888_TN_9
//
#elif defined(MODEL_888_TN) && (FILTER_NUM == 6)
#define MODEL_NUM MODEL_NUM_888_TN_6
//
#elif defined(MODEL_888_TC) && (FILTER_NUM == 9)
#define MODEL_NUM MODEL_NUM_888_TC_9
//
#elif defined(MODEL_888_TC) && (FILTER_NUM == 7)
#define MODEL_NUM MODEL_NUM_888_TC_7
//
#elif defined(MODEL_888_TC) && (FILTER_NUM == 8)
#define MODEL_NUM MODEL_NUM_888_TC_8
//
#elif defined(MODEL_IRO3_K92)
#define MODEL_NUM MODEL_NUM_IRO3_K92
//
#elif defined(MODEL_OPTIMUS_I1)
#define MODEL_NUM MODEL_NUM_OPTIMUS_I1
//
#elif defined(MODEL_OPTIMUS_I2)
#define MODEL_NUM MODEL_NUM_OPTIMUS_I2
//
#elif defined(MODEL_TOPBOX)
#define MODEL_NUM MODEL_NUM_TOPBOX
//
#elif defined(MODEL_IQ_IRO2)
#define MODEL_NUM MODEL_NUM_IQ_IRO2
//
#elif defined(MODEL_AIOTEC_4_3)
#define MODEL_NUM MODEL_NUM_PWR_S88_LCD_PRM
#endif

#ifndef MODEL_AIOTEC_4_3
#define MODEL_HAVE_WIFI 0
#else
#if (MODEL_NUM == MODEL_NUM_PWR_VOICE || MODEL_NUM == MODEL_NUM_PWR_BU || MODEL_NUM == MODEL_NUM_PWR_S85_LCD || MODEL_NUM == MODEL_NUM_PWR_S88_LCD || MODEL_NUM == MODEL_NUM_PWR_S88_LCD_PRM) // vi test jig k co noi voi esp
#define MODEL_HAVE_WIFI 0
#else
#define MODEL_HAVE_WIFI 1
#endif
#endif
/******************************************************************************
 * Types
 ******************************************************************************/

typedef uint8_t BOOLEAN;

typedef void *DPARAM; /*!< param data pointer type */

typedef enum
{
	OK = 0,
	ERR = 1,
	ERR_PARAM = 2,
	ERR_TIMEOUT = 3,
	ERR_UNKNOWN = 4,
	NOT_SUPPORT = 5,
	INPUT_FALSE = 6,

} ERR_E;

// #define VER_5V
/**
 * @brief Use brief, otherwise the index won't have a brief explanation.
 *    PE43712ds_attenuation
 * Detailed explanation.
 */

/******************************************************************************
 * Global variables
 ******************************************************************************/
extern volatile uint32_t g_sysTime;
extern volatile uint32_t g_sysTimeS;

/******************************************************************************
 * Global functions
 ******************************************************************************/

/******************************************************************************
 * Inline functions
 ******************************************************************************/

#endif
