/*
 * flowSensor.h
 *
 *  Created on: Mar 13, 2021
 *      Author: TranQuangThai
 */

#ifndef APPS_GPIO_FLOWSENSOR_H_
#define APPS_GPIO_FLOWSENSOR_H_
#include "stdint.h"
#include "stdbool.h"

#define PIN_LL  GPIO_PORT_E_PIN_2

float flowSensor_getFlowRateCurrent_ll1();
void flowSensor_checkPulseInInterrupt();
void flowSensor_checkFlowRunning();
uint16_t flowSensor_getFlowCurrent();
void flowSensor_init();
void flowSensor_updateFlowRate();
bool flowSensor_checkRemain_ll1();
void flowSensor_resetFlow();
bool flowSensor_isRunning();
#endif /* APPS_GPIO_FLOWSENSOR_H_ */
