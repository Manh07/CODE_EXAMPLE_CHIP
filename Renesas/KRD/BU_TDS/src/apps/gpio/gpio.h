
/******************************************************************************
*
* Embedded software team.
* (c) Copyright 2018.
* ALL RIGHTS RESERVED.
*
***************************************************************************/
/**
 *
 * @file         gpio.h
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

#ifndef GPIO_H_
#define GPIO_H_


/******************************************************************************
* Includes
******************************************************************************/

#include <config.h>
#include "r_gpio_rx_if.h"

/******************************************************************************
* Constants
******************************************************************************/



/******************************************************************************
* Macros 
******************************************************************************/
// Pin define

#define PULSE_PIN 	GPIO_PORT_H_PIN_3
#define PULSE_2_PIN GPIO_PORT_1_PIN_4
#define PULSE_3_PIN GPIO_PORT_1_PIN_7
#define MCU_WDI_PIN GPIO_PORT_E_PIN_3

#define PIN_AP_THAP 	GPIO_PORT_5_PIN_4
#define PIN_AP_CAO		GPIO_PORT_C_PIN_5

#define PIN_VDT_2		GPIO_PORT_B_PIN_3
#define PIN_VDT_3		GPIO_PORT_B_PIN_5
#define PIN_VDT_4		GPIO_PORT_B_PIN_6
#define PIN_VDT_5		GPIO_PORT_B_PIN_7
#define PIN_VDT_6		GPIO_PORT_C_PIN_2
#define PIN_VDT_7		GPIO_PORT_C_PIN_3
#define PIN_VDT_8		GPIO_PORT_5_PIN_5
#define PIN_VDT_9		GPIO_PORT_H_PIN_0

#define PIN_BOM_DL		GPIO_PORT_H_PIN_2
#define PIN_BOM_HUT		GPIO_PORT_H_PIN_1

#define PIN_VAN_XA         GPIO_PORT_C_PIN_6	//tam thoi chua dung
#define PIN_DK_BOM      PIN_VDT_8

#define PIN_H2CAP    PIN_VDT_7

#define PIN_WATER_RO    PIN_VDT_2
//#define PIN_WATER_HOT    GPIO_PORT_A_PIN_4
#define PIN_WATER_COLD    PIN_VDT_4
#define PIN_WATER_PH    PIN_VDT_6
// action define
#define CHECK_CO_AP_THAP 	(R_GPIO_PinRead(PIN_AP_THAP)== GPIO_LEVEL_LOW)
#define CHECK_CO_AP_CAO 	(R_GPIO_PinRead(PIN_AP_CAO)== GPIO_LEVEL_LOW)

#define PIN_DIEN_PHAN_SWAP         GPIO_PORT_A_PIN_0

#define TURN_ON_PUMP 		//R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_HIGH)
#define TURN_OFF_PUMP 		//R_GPIO_PinWrite(PIN_DK_BOM,GPIO_LEVEL_LOW)

#define TURN_ON_VAN_XA 		R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_HIGH)
#define TURN_OFF_VAN_XA		R_GPIO_PinWrite(PIN_VAN_XA,GPIO_LEVEL_LOW)

#define TURN_ON_H2CAP    //R_GPIO_PinWrite(PIN_H2CAP,GPIO_LEVEL_HIGH)
#define TURN_OFF_H2CAP  //R_GPIO_PinWrite(PIN_H2CAP,GPIO_LEVEL_LOW)

#define TURN_ON_SWAP  R_GPIO_PinWrite(PIN_DIEN_PHAN_SWAP,GPIO_LEVEL_HIGH)
#define TURN_OFF_SWAP  R_GPIO_PinWrite(PIN_DIEN_PHAN_SWAP,GPIO_LEVEL_LOW)

#define TURN_ON_VDT_2  R_GPIO_PinWrite(PIN_VDT_2,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_2  R_GPIO_PinWrite(PIN_VDT_2,GPIO_LEVEL_LOW)
#define TURN_ON_VDT_3  R_GPIO_PinWrite(PIN_VDT_3,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_3  R_GPIO_PinWrite(PIN_VDT_3,GPIO_LEVEL_LOW)
#define TURN_ON_VDT_4  R_GPIO_PinWrite(PIN_VDT_4,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_4  R_GPIO_PinWrite(PIN_VDT_4,GPIO_LEVEL_LOW)


#define TURN_ON_VDT_7  R_GPIO_PinWrite(PIN_VDT_7,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_7  R_GPIO_PinWrite(PIN_VDT_7,GPIO_LEVEL_LOW)

#define TURN_ON_VDT_6  R_GPIO_PinWrite(PIN_VDT_6,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_6  R_GPIO_PinWrite(PIN_VDT_6,GPIO_LEVEL_LOW)

#define TURN_ON_VDT_5  R_GPIO_PinWrite(PIN_VDT_5,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_5  R_GPIO_PinWrite(PIN_VDT_5,GPIO_LEVEL_LOW)
#define TURN_ON_VDT_8  R_GPIO_PinWrite(PIN_VDT_8,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_8  R_GPIO_PinWrite(PIN_VDT_8,GPIO_LEVEL_LOW)
#define TURN_ON_VDT_9  R_GPIO_PinWrite(PIN_VDT_9,GPIO_LEVEL_HIGH)
#define TURN_OFF_VDT_9  R_GPIO_PinWrite(PIN_VDT_9,GPIO_LEVEL_LOW)

#define TURN_ON_BOM_DL  R_GPIO_PinWrite(PIN_BOM_DL,GPIO_LEVEL_HIGH)
#define TURN_OFF_BOM_DL  R_GPIO_PinWrite(PIN_BOM_DL,GPIO_LEVEL_LOW)
#define TURN_ON_BOM_HUT  R_GPIO_PinWrite(PIN_BOM_HUT,GPIO_LEVEL_HIGH)
#define TURN_OFF_BOM_HUT  R_GPIO_PinWrite(PIN_BOM_HUT,GPIO_LEVEL_LOW)

#define IO_IS_ON(iopin)  (R_GPIO_PinRead(iopin) != 0)
/******************************************************************************
* Types
******************************************************************************/


/******************************************************************************
* Global variables
******************************************************************************/
   

/******************************************************************************
* Global functions
******************************************************************************/
PUBLIC void GPIO_Init(void);
void userGpio_testPinOption();
void userGpio_controlBlock();
void userGpio_isHaveSucRuaLoiA();
void userGpio_isHaveGetWater(bool isOn) ;
void userGpio_initPwmDienPhan();
void userGpio_turnPwm(uint8_t percent);
void gpio_resetAllXaSuc();
bool gpio_isXaSuc();
uint8_t gpio_xaSucType();
void userGpio_checkH2CAP();
void gpio_setNeedXaSucWhenResetFilterC();
void userGpio_checkAutoFunction();
void gpio_setNeedXaSucInterval();
void gpio_setNeedXaSucFollowFlowSensor();
void gpio_setResumePauseXaSucAfterGetWater(bool isResume, bool isXaBefore);
void pumpControl_checkFirstTime();
void TDS_Pin_ADC_to_Output();
void TDS_Pin_Output_to_ADC();
void gpio_xaSucPowerOnAuto();
/******************************************************************************
* Inline functions
******************************************************************************/



#endif 


