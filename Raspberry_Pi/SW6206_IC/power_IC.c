/** @file power_IC.c
 *
 * @brief Get data from power IC SW6206
 */

#include "power_IC.h"

static int sw6206_fd = -1;

int power_ic_read_reg(uint8_t reg)
{
    if (sw6206_fd == -1)
    {
        printf("SW6206 not initialized!\n");
        return -1;
    }
    return wiringPiI2CReadReg8(sw6206_fd, reg);
}

int power_ic_write_reg(uint8_t reg, uint8_t value)
{
    if (sw6206_fd == -1)
    {
        printf("SW6206 not initialized!\n");
        return -1;
    }
    return wiringPiI2CWriteReg8(sw6206_fd, reg, value);
}

static int power_ic_read_adc(uint8_t adc_type)
{
    power_ic_write_reg(SW6206_REG_ADC_CONFIG, adc_type);
    int high = power_ic_read_reg(SW6206_REG_ADC_DATA_HIGH);
    int low = power_ic_read_reg(SW6206_REG_ADC_DATA_LOW) & 0x0F;
    return (high << 4) | low;
}

/*------------------------ External API --------------------------------------*/
int power_ic_init()
{
    if (wiringPiSetup() == -1)
    {
        printf("Failed to initialize wiringPi!\n");
        return -1;
    }

    sw6206_fd = wiringPiI2CSetup(SW6206_I2C_ADDR);
    if (sw6206_fd == -1)
    {
        printf("Failed to initialize I2C device SW6206!\n");
        return -1;
    }
    return 0;
}

float power_ic_read_vbat()
{
    return power_ic_read_adc(ADC_VBAT) * 1.2; // 1.2mV mỗi đơn vị ADC
}

float power_ic_read_vout()
{
    return power_ic_read_adc(ADC_VOUT) * 4.0; // 4mV mỗi đơn vị ADC
}

float power_ic_read_current()
{
    return (power_ic_read_adc(ADC_CURRENT_DISCHARGE) * 25.0) / 11.0; // 25/11 mA mỗi đơn vị ADC
}

float power_ic_read_temp()
{
    int raw_temp = power_ic_read_adc(ADC_TEMP_IC);
    return (raw_temp - 1839) / 6.82; // Chuyển đổi ADC sang độ C
}

float power_ic_read_battery()
{
    int low = power_ic_read_reg(SW6206_REG_BATTERY_CAPACITY_LOW);
    int high = power_ic_read_reg(SW6206_REG_BATTERY_CAPACITY_HIGH) & 0x0F;
    int capacity = (high << 8) | low;
    return capacity * 0.1695; // Chuyển đổi sang % pin
}

/**************************** End of file *************************************/
