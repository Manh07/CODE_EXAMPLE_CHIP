/******************************************************************************
 *
 * Embedded software team.
 * (c) Copyright 2018.
 * ALL RIGHTS RESERVED.
 *
 ***************************************************************************/
/*!
 *
 * @file        main.c
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

#include "r_smc_entry.h"
#include "config.h"
#include "gpio.h"
#include "adc.h"
#include "flash_app.h"
#include "timeCheck.h"
#include "filter_time.h"
#include "UIControl.h" //
#include "pumpControl.h"
#include "timer.h"
#include "errorCheck.h" //
#include "uart.h"
#include "filterExpire.h"
#include "uartHandle.h"
#include "updateParam.h"
#include "wtd.h"
#include "heatControl.h" //
#include "getWater.h"	 //
#include "flowSensor.h"	 //
#include "thermo.h"
#include "getWater.h"
#include "flowSensor.h"
#include "uartVoice.h"
/******************************************************************************
 * External objects
 ******************************************************************************/
extern volatile uint8_t g_run200usFlag;
extern volatile uint8_t g_run1msFlag;
volatile bool g_beforemain = true;
/******************************************************************************
 * Global variables
 ******************************************************************************/

uint16_t g_adc_result;

/******************************************************************************
 * Constants and macros
 ******************************************************************************/

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define TIME_TO_DISPLAY_TDS (2000) // 2 secs
/******************************************************************************
 * Local types
 ******************************************************************************/

/******************************************************************************
 * Local function prototypes
 ******************************************************************************/

/******************************************************************************
 * Local variables
 ******************************************************************************/

uint16_t value;
uint32_t time_out = 0;
uint32_t i = 0;
uint8_t s_timeOut100ms;
uint8_t s_timeOut10ms;
uint16_t s_timeOut1s = 0;
uint16_t tds_Value = 0;
// LOCAL BOOLEAN s_is_timeout = TRUE;

/******************************************************************************
 * Local functions
 ******************************************************************************/
void run200usTask();
void run1msTask();
void run10msTask();
void run100msTask();
void run_DisplayTds();
void run1sTask();
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
void main(void);
extern bool g_ioTestEn;
void main(void)
{
 	GPIO_Init();
	R_Config_CMT0_Start();
	R_Config_CMT1_Start();
	getWater_initPin();
	UART_Init();
	uartHandle_Init();
	flash_app_init();
	ADC_Init();
	TIMER_Init();
	pumpControl_onVanXaInMs(5000);
	/* Main loop */
	g_beforemain = false;
	uartVoice_sendResetToMcu();
	uartHandle_sendResetEsp();
	flowSensor_init();
	while (1)
	{
		//////////////////////
		if (g_run200usFlag == 1)
		{
			run200usTask();
			g_run200usFlag = 0;
		}
		if (g_run1msFlag == 1)
		{
			run1msTask();
			g_run1msFlag = 0;
		}
		if (s_timeOut10ms >= 10)
		{
			run10msTask();
			s_timeOut10ms = 0;
		}
		if (s_timeOut100ms >= 100)
		{
			run100msTask();
			s_timeOut100ms = 0;
		}
		if (s_timeOut1s >= 2000)
		{
			run1sTask();
			s_timeOut1s = 0;
		}
		flowSensor_updateFlowRate();
		// ADC_process();
		// ADC_displayTask();
		TIMER_CheckTimerEvent();
	}
}

void run200usTask()
{
}
void run1msTask()
{
	// UART_Process();
	// uartHandle_tick();
	if (s_timeOut1s < 1000)
	{
		s_timeOut1s++;
	}

	if (s_timeOut100ms < 100)
	{
		s_timeOut100ms++;
	}
	if (s_timeOut10ms < 10)
	{
		s_timeOut10ms++;
	}
}

void run10msTask()
{
	wtd_feed();
	// updateParam_tick();
	//    userGpio_controlBlock();
	//    thermo_readThermoValue();
	// getWater_checVdt5GetPh();
	gpio_xaSucPowerOnAuto();
}

void run100msTask()
{
	// pumpControl_process();
	ErrorCheck_process();
	// filterExpire_process();
	// filter_time_process();
	userGpio_checkAutoFunction();
	// userAdc_calculateTemperatureLanh();
	// userAdc_calculateTemperatureNong();
	UIControl_process();
	// pumpControl_checkFirstTime();
	// getWater_checkUpdateValueToEsp();
}

void run1sTask()
{
	tds_Value = ADC_GetTdsValueDisplay(TDS_OUT_VALUE);
	userGpio_testPinOption();
}
void abort(void)
{
	while (1)
	{
		;
	}
}
