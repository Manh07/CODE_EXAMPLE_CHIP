/*
 * thermo.h
 *
 *  Created on: Jul 31, 2021
 *      Author: TranQuangThai
 */

#ifndef APPS_THERMOMODULE_THERMO_H_
#define APPS_THERMOMODULE_THERMO_H_
#include "stdint.h"
#include "stdbool.h"

void thermo_readThermoValue();
uint8_t thermo_getTemHot();
uint8_t thermo_getTemCold();
bool thermo_getTankHotIsEnable();
bool thermo_getTankColdIsEnable();
//bool thermo_isAvailable();
void thermo_controlTank(bool isHotEnable, bool isColdEnable);
void thermo_controlEnableTankHot(bool isEnable);
void thermo_controlEnableTankCold(bool isEnable);
//void thermo_checkPulse();
//void thermo_initPinTempPulseRead();
//void thermo_checkInterruptPulse();
#endif /* APPS_THERMOMODULE_THERMO_H_ */
