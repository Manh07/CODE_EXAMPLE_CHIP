/** @file angle_sensor.h
 *
 * @brief Get data from angle sensor LSM6DS3TRC
 */

#ifndef ANGLE_SENSOR_H
#define ANGLE_SENSOR_H

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

#define LSM6DS3TRC_ADDR 0x6A // I2C address of the sensor

#define REG_WHO_AM_I 0x0F // Device identification register

#define REG_CTRL1_XL 0x10 // Accelerometer configuration register
#define REG_CTRL2_G 0x11  // Gyroscope configuration register

// Gyroscope output registers
#define REG_OUTX_L_G 0x22 // X-axis data (LSB)
#define REG_OUTX_H_G 0x23 // X-axis data (MSB)
#define REG_OUTY_L_G 0x24 // Y-axis data (LSB)
#define REG_OUTY_H_G 0x25 // Y-axis data (MSB)
#define REG_OUTZ_L_G 0x26 // Z-axis data (LSB)
#define REG_OUTZ_H_G 0x27 // Z-axis data (MSB)

// Accelerometer output registers
#define REG_OUTX_L_XL 0x28 // X-axis data (LSB)
#define REG_OUTX_H_XL 0x29 // X-axis data (MSB)
#define REG_OUTY_L_XL 0x2A // Y-axis data (LSB)
#define REG_OUTY_H_XL 0x2B // Y-axis data (MSB)
#define REG_OUTZ_L_XL 0x2C // Z-axis data (LSB)
#define REG_OUTZ_H_XL 0x2D // Z-axis data (MSB)

typedef enum
{
    SENSOR_OK,    // Sensor is working correctly
    SENSOR_ERR_1, // Sensor initialization failed
    SENSOR_ERR_2, // Sensor reading error when get offset
    SENSOR_ERR_3  // Sensor reading error when get angle
} ErrorCodeNum;

/* Initialize the sensor */
bool sensor_init(int16_t *offset);

/* Get roll and pitch angles */
bool sensor_get_angle(float *roll, float *pitch);

/* Get sensor offsets */
void sensor_get_offset(int16_t *offsetX, int16_t *offsetY, int16_t *offsetZ);

/* Calibrate sensor offsets */
void sensor_calib(int16_t offsetX, int16_t offsetY, int16_t offsetZ);

/* Deinitialize the sensor */
void sensor_deinit();

/* Test module auto*/
ErrorCodeNum sensor_test_module();

#endif

/**************************** End of file *************************************/