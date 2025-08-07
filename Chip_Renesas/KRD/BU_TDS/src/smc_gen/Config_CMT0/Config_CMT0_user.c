/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2019 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_CMT0_user.c
* Component Version: 2.2.0
* Device(s)        : R5F51303AxFM
* Description      : This file implements device driver for Config_CMT0.
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "Config_CMT0.h"
/* Start user code for include. Do not edit comment generated here */
#include "Config_S12AD0.h"
#include "timer.h"
#include "getWater.h"
#include "gpio.h"
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
volatile uint8_t g_pwm_timer;
extern volatile uint8_t g_adc_flag;
volatile uint8_t g_pwm_value = 0;
volatile uint8_t cnt = 0;
extern volatile uint8_t lastLogicPwm;
extern bool g_ioTestEn;
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_CMT0_Create_UserInit
* Description  : This function adds user code after initializing the CMT0 channel
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

void R_Config_CMT0_Create_UserInit(void)
{
    /* Start user code for user init. Do not edit comment generated here */
    /* End user code. Do not edit comment generated here */
}

/***********************************************************************************************************************
* Function Name: r_Config_CMT0_cmi0_interrupt
* Description  : This function is CMI0 interrupt service routine
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/

#if FAST_INTERRUPT_VECTOR == VECT_CMT0_CMI0
#pragma interrupt r_Config_CMT0_cmi0_interrupt(vect=VECT(CMT0,CMI0),fint)
#else
#pragma interrupt r_Config_CMT0_cmi0_interrupt(vect=VECT(CMT0,CMI0))
#endif
static void r_Config_CMT0_cmi0_interrupt(void)
{
    /* Start user code for r_Config_CMT0_cmi0_interrupt. Do not edit comment generated here */
//	LED5 ^= 1u;
//	PWM ^= 1u;
//	g_pwm_timer = 1;
    // start adc for tds out
//	g_adc_flag = 0;


	if(0U == g_adc_flag)
	{
		uint8_t check = (cnt++) % 4;
		switch (check) {
			case 0:
				PWM_TDS_1 = 1;
				PWM_TDS_2 = 1;
				if(g_ioTestEn || getWater_isAlkalineOuting())
				{
                    TDS_Pin_Output_to_ADC();
					PWM_TDS_3 = 1;
				}
				else
				{
					TDS_Pin_ADC_to_Output();
					PWM_TDS_3 = 0;
				}
				break;
			case 1:
				if(0U == g_adc_flag)
				{
					if(lastLogicPwm == 1)
						break;
					g_pwm_value = 1;
					R_Config_S12AD0_Start();
				}
				break;
			case 2:
				PWM_TDS_1 = 0;
				PWM_TDS_2 = 0;
				PWM_TDS_3 = 0;
				g_pwm_value = 0;
				break;
			case 3:
				 if(0U == g_adc_flag)
				{
					if(lastLogicPwm == 0)
						break;
					g_pwm_value = 0;
					R_Config_S12AD0_Start();
				}
				break;
			default:
				break;
		}
	}
	TIMER_SystemTickEvent();
    /* End user code. Do not edit comment generated here */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
