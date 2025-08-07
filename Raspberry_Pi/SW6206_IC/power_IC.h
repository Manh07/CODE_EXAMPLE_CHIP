/** @file power_IC.h
 *
 * @brief Get data from power IC SW6206
 */

#ifndef POWER_IC_H
#define POWER_IC_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <wiringPi.h>
#include <wiringPiI2C.h>

// Address I2C of IC
#define SW6206_I2C_ADDR 0x3C
// Address data of IC
#define SW6206_REG_ADC_CONFIG 0x12
#define SW6206_REG_ADC_DATA_HIGH 0x13
#define SW6206_REG_ADC_DATA_LOW 0x14
#define SW6206_REG_BATTERY_CAPACITY_LOW 0x73
#define SW6206_REG_BATTERY_CAPACITY_HIGH 0x74
// Type of ADC config
#define ADC_VBAT 0
#define ADC_VOUT 1
#define ADC_TEMP_IC 2
#define ADC_TEMP_NTC 3
#define ADC_CURRENT_CHARGE 4
#define ADC_CURRENT_DISCHARGE 5

// Init IC SW6206
int power_ic_init();

// Read dato from reg of SW6206
int power_ic_read_reg(uint8_t reg);

// Write data to reg of SW6206
int power_ic_write_reg(uint8_t reg, uint8_t value);

// Read Vbat
float power_ic_read_vbat();

// Read Vout
float power_ic_read_vout();

// Read current
float power_ic_read_current();

// Read temperature
float power_ic_read_temp();

// Read battery
float power_ic_read_battery();

#endif

/**************************** End of file *************************************/