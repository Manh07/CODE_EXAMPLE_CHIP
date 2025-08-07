/** @file angle_sensor.c
 *
 * @brief Get data from angle sensor LSM6DS3TRC
 */

#include "angle_sensor.h"

#if STUB_LOAD_IMAGE

#else
#define LSM6DS3TRC 1
#endif

static bool b_have_sensor = true;
static pthread_mutex_t g_sensor_mutex;
static uint8_t g_sensor_fs = -1;
static int16_t g_offsetX = -280, g_offsetY = 16850, g_offsetZ = 400;
ErrorCodeNum g_sensor_error;

/* Checks if the sensor is connected by reading the WHO_AM_I register */
static bool sensor_id()
{
    uint8_t id = wiringPiI2CReadReg8(g_sensor_fs, REG_WHO_AM_I);
    if (id != 0x6A)
    {
        printf("Can't find sensor LSM6DS3TRC!\n");
        return false;
    }
    return true;
}

/* Reads accelerometer data from the sensor */
static bool sensor_read_accel(int16_t *ax, int16_t *ay, int16_t *az)
{
    if (!sensor_id)
    {
        return false;
    }

    uint8_t xl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTX_L_XL);
    uint8_t xh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTX_H_XL);
    uint8_t yl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTY_L_XL);
    uint8_t yh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTY_H_XL);
    uint8_t zl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTZ_L_XL);
    uint8_t zh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTZ_H_XL);

    *ax = (int16_t)((xh << 8) | xl);
    *ay = (int16_t)((yh << 8) | yl);
    *az = (int16_t)((zh << 8) | zl);

    return true;
}

/* Reads gyroscope data from the sensor */
static bool sensor_read_gyro(int16_t *gx, int16_t *gy, int16_t *gz)
{
    if (!sensor_id)
    {
        return false;
    }

    uint8_t xl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTX_L_G);
    uint8_t xh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTX_H_G);
    uint8_t yl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTY_L_G);
    uint8_t yh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTY_H_G);
    uint8_t zl = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTZ_L_G);
    uint8_t zh = wiringPiI2CReadReg8(g_sensor_fs, REG_OUTZ_H_G);

    *gx = (int16_t)((xh << 8) | xl);
    *gy = (int16_t)((yh << 8) | yl);
    *gz = (int16_t)((zh << 8) | zl);

    return true;
}

/*------------------------ External API --------------------------------------*/

/* Initializes the sensor and configures the accelerometer and gyroscope */
bool sensor_init(int16_t *offset)
{
#ifndef LSM6DS3TRC
    return true;
#endif
    g_sensor_error = SENSOR_OK;
    g_sensor_fs = wiringPiI2CSetup(LSM6DS3TRC_ADDR);
    if ((g_sensor_fs == -1) || (!sensor_id()))
    {
        printf("Can not init angle sensor, fs = %d! \n", g_sensor_fs);
        b_have_sensor = false;
        g_sensor_error = SENSOR_ERR_1;
        return b_have_sensor;
    }
    else
    {
        printf("Init sensor success!\n");
    }

    // Configure accelerometer (±2g, 104Hz)
    wiringPiI2CWriteReg8(g_sensor_fs, REG_CTRL1_XL, 0x40);

    // Configure gyroscope (250dps, 104Hz)
    wiringPiI2CWriteReg8(g_sensor_fs, REG_CTRL2_G, 0x40);

    g_offsetX = offset[0];
    g_offsetY = offset[1];
    g_offsetZ = offset[2];

    pthread_mutex_init(&g_sensor_mutex, NULL);

    return b_have_sensor;
}

/* Calibrates the sensor by setting new offset values */
void sensor_calib(int16_t offsetX, int16_t offsetY, int16_t offsetZ)
{
#ifndef LSM6DS3TRC
    return;
#endif
    if (b_have_sensor == false)
    {
        return;
    }
    g_offsetX = offsetX;
    g_offsetY = offsetY;
    g_offsetZ = offsetZ;
    printf("[CalibXYZ]=====>> %s, %d g_offsetX %d, g_offsetY %d, g_offsetZ %d\n", __func__, __LINE__, g_offsetX, g_offsetY, g_offsetZ);
}

/* Computes roll and pitch angles from accelerometer data */
bool sensor_get_angle(float *roll, float *pitch)
{
#ifndef LSM6DS3TRC
    return true;
#else
    if (!b_have_sensor)
    {
        *roll = 0;
        *pitch = 0;
        return false;
    }
    pthread_mutex_lock(&g_sensor_mutex);

    int16_t ax, ay, az;
    float ave_ax = 0, ave_ay = 0, ave_az = 0;
    uint8_t cnt = 0;
    uint8_t cnt_max = 50;
    uint64_t time_start = millis();

    while (1)
    {
        bool ret = sensor_read_accel(&ax, &ay, &az);
        if (ret)
        {
            // printf("%.2f  %.2f  %.2f \n", ave_ax, ave_ay, ave_az);
            // printf("%d  %d  %d \n", ax, ay, az);
            ave_ax += ax;
            ave_ay += ay;
            ave_az += az;
            cnt++;
        }
        if (cnt == cnt_max)
        {
            break;
        }
        if ((millis() - time_start) >= 1e6)
        {
            g_sensor_error = SENSOR_ERR_3;
            break;
        }
    }
    if (cnt == cnt_max)
    {
        // ave_ax = g_offsetX + ave_ax / cnt;
        // ave_ay = g_offsetY + ave_ay / cnt;
        // ave_az = g_offsetZ + ave_az / cnt;
        // // Convert acceleration data to angles (degrees)
        // *roll = atan2(ave_ay, sqrt(ave_ax * ave_ax + ave_az * ave_az)) * (180.0 / M_PI);   // Roll angle
        // *pitch = atan2(-ave_ax, sqrt(ave_ay * ave_ay + ave_az * ave_az)) * (180.0 / M_PI); // Pitch angle

        ave_ax = (ave_ax / cnt);
        ave_ay = (ave_ay / cnt);
        ave_az = (ave_az / cnt);
        printf("%d  %d  %d \n", g_offsetX, g_offsetY, g_offsetZ);
        float dg = sqrt(ave_ax * ave_ax + ave_ay * ave_ay + ave_az * ave_az);
        float dg0 = sqrt(g_offsetX * g_offsetX + g_offsetY * g_offsetY + g_offsetZ * g_offsetZ);
        // Convert acceleration data to angles (degrees)
        *roll = (asin(g_offsetZ / dg0) - asin(ave_az / dg)) * 180.0 / M_PI;
        *pitch = (asin(g_offsetY / dg0) - asin(ave_ay / dg)) * 180.0 / M_PI;
    }
    else
    {
        *roll = 0;
        *pitch = 0;
    }

    pthread_mutex_unlock(&g_sensor_mutex);
    return true;
#endif
}

/* Get sensor offsets */
void sensor_get_offset(int16_t *offsetX, int16_t *offsetY, int16_t *offsetZ)
{
#ifndef LSM6DS3TRC
    return;
#else
    if (!b_have_sensor)
    {
        *offsetX = 0;
        *offsetY = 0;
        *offsetZ = 0;
        return;
    }
    pthread_mutex_lock(&g_sensor_mutex);

    int16_t ax, ay, az;
    float ave_ax = 0, ave_ay = 0, ave_az = 0;
    uint16_t cnt = 0;
    uint16_t cnt_max = 500;
    uint64_t time_start = millis();

    while (1)
    {
        bool ret = sensor_read_accel(&ax, &ay, &az);
        if (ret)
        {
            // printf("%.2f  %.2f  %.2f \n", ave_ax, ave_ay, ave_az);
            // printf("%d  %d  %d \n", ax, ay, az);
            ave_ax += ax;
            ave_ay += ay;
            ave_az += az;
            cnt++;
        }
        if (cnt == cnt_max)
        {
            break;
        }
        if ((millis() - time_start) >= 1e6)
        {
            g_sensor_error = SENSOR_ERR_2;
            break;
        }
    }
    if (cnt == cnt_max)
    {
        *offsetX = ave_ax / cnt;
        *offsetY = ave_ay / cnt;
        *offsetZ = ave_az / cnt;
        printf("offsetX = %d, offsetY = %d, offsetZ = %d\r\n", *offsetX, *offsetY, *offsetZ);
    }
    else
    {
        *offsetX = 0;
        *offsetY = 0;
        *offsetZ = 0;
    }

    pthread_mutex_unlock(&g_sensor_mutex);
#endif
}

/* Deinitializes the sensor and releases resources */
void sensor_deinit()
{
#ifndef LSM6DS3TRC
    return;
#endif
    if (g_sensor_fs != -1)
    {
        close(g_sensor_fs);
        g_sensor_fs = -1;
    }
    pthread_mutex_destroy(&g_sensor_mutex);
}

/* Test module auto*/
ErrorCodeNum sensor_test_module()
{
    int16_t offset[3] = {0, 0, 0};
    if (!sensor_init(offset))
    {
        return g_sensor_error;
    }

    int16_t offsetX = 0, offsetY = 0, offsetZ = 0;
    sensor_get_offset(&offsetX, &offsetY, &offsetZ);
    if (g_sensor_error == SENSOR_ERR_2)
    {
        return g_sensor_error;
    }

    g_offsetX = offsetX;
    g_offsetY = offsetY;
    g_offsetZ = offsetZ;
    float roll_test, pitch_test, roll_test_old, pitch_test_old;
    for (int i = 0; i < 5; i++)
    {
        sensor_get_angle(&roll_test, &pitch_test);
        if (abs(roll_test - roll_test_old) >= 1.0 || abs(pitch_test - pitch_test_old) >= 1.0 || g_sensor_error == SENSOR_ERR_3)
        {
            g_sensor_error = SENSOR_ERR_3;
            return g_sensor_error;
        }
        roll_test_old = roll_test;
        pitch_test_old = pitch_test;
        sleep(1);
    }

    return g_sensor_error;
}

/**************************** End of file *************************************/
