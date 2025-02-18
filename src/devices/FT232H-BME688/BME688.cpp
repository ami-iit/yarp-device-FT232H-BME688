#include "BME688.h"
#include <ftd2xx.h>

BME688::BME688(FT232H_I2C* i2cCommsInterfance)
{
    i2c = i2cCommsInterfance;
    // reset the device
    i2c->writeByte(BME688_I2C_ADDR, 0xE0, 0xB6);
    UCHAR data;
    i2c->readByte(BME688_I2C_ADDR, 0xF0, &data);
    if (data != 0x01)
    {
        std::string errorMsg = "BME688 not found, expected a chip id value of 1, ";
        errorMsg += "but got " + std::to_string(data);
        throw std::runtime_error(errorMsg);
    }
}

void BME688::setupForReading()
{
    // set oversampling for temperature and pressue
    i2c->writeByte(BME688_I2C_ADDR, BME688_CTRL_MEAS_REG_ADDR, 0x28);

    // set oversampling for humidity
    i2c->writeByte(BME688_I2C_ADDR, BME688_CTRL_HUM_REG_ADDR, 0x03);

    // set the filter
    i2c->writeByte(BME688_I2C_ADDR, BME688_CONFIG_REG_ADDR, 0x08);

    // set run gas and nb_conv
    i2c->writeByte(BME688_I2C_ADDR, BME688_CTRL_GAS_1_REG_ADDR, 0x12);

    // set parallel mode
    i2c->writeByte(BME688_I2C_ADDR, BME688_CTRL_MEAS_REG_ADDR, 0x2A);
}

void BME688::readTempCalibrationData()
{
    UCHAR data;
    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_CALIB_PAR_T1_MSB_REG_ADDR, &data);
    UCHAR partT1msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_CALIB_PAR_T1_LSB_REG_ADDR, &data);
    UCHAR partT1lsb = data;

    tempCalibData.parT1 = (uint16_t)partT1msb << 8 | (uint16_t)partT1lsb;

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_CALIB_PAR_T2_MSB_REG_ADDR, &data);
    UCHAR partT2msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_CALIB_PAR_T2_LSB_REG_ADDR, &data);
    UCHAR partT2lsb = data;

    tempCalibData.parT2 = (int16_t)partT2msb << 8 | (int16_t)partT2lsb;

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_CALIB_PAR_T3_REG_ADDR, &data);
    tempCalibData.parT3 = (int8_t)data;
}

void BME688::readTemperature()
{
    CHAR data;
    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_MSB0_REG_ADDR, &data);
    tempuratureBytes[0] = data; 

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_LSB0_REG_ADDR, &data);
    tempuratureBytes[1] = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_TEMP_XLSB0_REG_ADDR, &data);
    tempuratureBytes[2] = data;
}

// the equations used in this function are found in page 23 of the documentation
float BME688::getTemperature()
{
    uint32_t temp_adc = (uint32_t)tempuratureBytes[0] << 12 | (uint32_t)tempuratureBytes[1] << 4 | (uint32_t)tempuratureBytes[2] >> 4;
    
    int64_t var1 = (temp_adc / 8) - (tempCalibData.parT1 * 2);

    int64_t var2 = (var1 * tempCalibData.parT2) / 2048;
    int64_t var3 = ((var1 / 2) * (var1 / 2)) / 4096;
    var3 = (var3 * tempCalibData.parT3 * 16) / 16384;
    t_fine = (var2 + var3);

    int16_t temp = (t_fine * 5 + 128) / 256;

    return (float)temp / 100.0;

}

void BME688::readPressureCalibrationData()
{
    UCHAR data;

    // for the first pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P1_MSB_REG_ADDR, &data);
    UCHAR partP1msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P1_LSB_REG_ADDR, &data);
    UCHAR partP1lsb = data;

    pressCalibData.parP1 = (uint16_t)partP1msb << 8 | (uint16_t)partP1lsb;

    // for the second pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P2_MSB_REG_ADDR, &data);
    UCHAR partP2msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P2_LSB_REG_ADDR, &data);
    UCHAR partP2lsb = data;

    pressCalibData.parP2 = (int16_t)partP2msb << 8 | (int16_t)partP2lsb;

    // for the third pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P3_REG_ADDR, &data);
    pressCalibData.parP3 = (uint8_t)data;

    // for the fourth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P4_MSB_REG_ADDR, &data);
    UCHAR partP4msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P4_LSB_REG_ADDR, &data);
    UCHAR partP4lsb = data;

    pressCalibData.parP4 = (int16_t)partP4msb << 8 | (int16_t)partP4lsb;

    // for the fifth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P5_MSB_REG_ADDR, &data);
    UCHAR partP5msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P5_LSB_REG_ADDR, &data);
    UCHAR partP5lsb = data;

    pressCalibData.parP5 = (int16_t)partP5msb << 8 | (int16_t)partP5lsb;

    // for the sixth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P6_REG_ADDR, &data);
    pressCalibData.parP6 = (int8_t)data;

    // for the seventh pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P7_REG_ADDR, &data);
    pressCalibData.parP7 = (int8_t)data;

    // for the eighth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P8_MSB_REG_ADDR, &data);
    UCHAR partP8msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P8_LSB_REG_ADDR, &data);
    UCHAR partP8lsb = data;

    pressCalibData.parP8 = (int16_t)partP8msb << 8 | (int16_t)partP8lsb;

    // for the ninth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P9_MSB_REG_ADDR, &data);
    UCHAR partP9msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P9_LSB_REG_ADDR, &data);
    UCHAR partP9lsb = data;

    pressCalibData.parP9 = (int16_t)partP9msb << 8 | (int16_t)partP9lsb;

    // for the tenth pressure calibration parameter
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_CALIB_PAR_P10_REG_ADDR, &data);
    pressCalibData.parP10 = (uint8_t)data;
}

void BME688::readPressure()
{
    UCHAR data;
    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_MSB0_REG_ADDR, &data);
    pressureBytes[0] = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_LSB0_REG_ADDR, &data);
    pressureBytes[1] = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_PRESS_XLSB0_REG_ADDR, &data);
    pressureBytes[2] = data;
}

float BME688::getPressure()
{
    uint32_t press_adc = ((int32_t)pressureBytes[0]) << 12 | (int32_t)pressureBytes[1] << 4 | (int32_t)pressureBytes[2] >> 4;

    int32_t var1 = (t_fine / 2) - 64000;

    int32_t var2 = ((var1 / 4) * (var1 / 4)) / 2048;
    var2 = (var2 * pressCalibData.parP6) / 4;
    var2 = var2 + (var1 * pressCalibData.parP5 * 2);
    var2 = (var2 / 4) + (pressCalibData.parP4 * 65536);
    var1 = ((((var1 / 4) * (var1 / 4)) / 8192) * (pressCalibData.parP3 * 32) / 8) + (
        (pressCalibData.parP2 * var1) / 2
    );

    var1 = var1 / 262144;
    var1 = ((32768 + var1) * pressCalibData.parP1) / 32768;
    int32_t calc_pres = 1048576 - press_adc;
    calc_pres = (calc_pres - (var2 / 4096)) * 3125;
    calc_pres = (calc_pres / var1) * 2;
    var1 = (pressCalibData.parP9 * (((calc_pres / 8) * (calc_pres / 8)) / 8192)) / 4096;
    var2 = ((calc_pres / 4) * pressCalibData.parP8) / 8192;
    int32_t var3 = (((calc_pres / 256) * (calc_pres / 256) * (calc_pres / 256)) * pressCalibData.parP10) / 131072;
    calc_pres += (var1 + var2 + var3 + (pressCalibData.parP7 * 128)) / 16;

    return (float)calc_pres / 100.0;

}

void BME688::readHumidityCalibrationData()
{
    UCHAR data;
    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H1_MSB_REG_ADDR, &data);
    UCHAR partH1msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H1_LSB_REG_ADDR, &data);
    UCHAR partH1lsb = data;

    humCalibData.parH1 = (uint16_t)partH1msb << 4 | ((uint16_t)partH1lsb & 0x0F);

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H2_MSB_REG_ADDR, &data);
    UCHAR partH2msb = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H2_LSB_REG_ADDR, &data);
    UCHAR partH2lsb = data;

    humCalibData.parH2 = (uint16_t)partH2msb << 4 | (uint16_t)partH2lsb >> 4;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H3_REG_ADDR, &data);
    humCalibData.parH3 = (int8_t)data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H4_REG_ADDR, &data);
    humCalibData.parH4 = (int8_t)data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H5_REG_ADDR, &data);
    humCalibData.parH5 = (int8_t)data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H6_REG_ADDR, &data);
    humCalibData.parH6 = (uint8_t)data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_CALIB_PAR_H7_REG_ADDR, &data);
    humCalibData.parH7 = (int8_t)data;
}

void BME688::readHumidity()
{
    UCHAR data;
    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_MSB0_REG_ADDR, &data);
    humidityBytes[0] = data;

    i2c->readByte(BME688_I2C_ADDR, BME688_HUM_LSB0_REG_ADDR, &data);
    humidityBytes[1] = data;
}

float BME688::getHumidity()
{
    uint16_t hum_adc = (int16_t)humidityBytes[0] << 8 | (int16_t)humidityBytes[1];
    int32_t scaledTemp = (((int32_t)t_fine * 5) + 128) >> 8;

    int32_t var1 = (int32_t)(hum_adc - ((int32_t)((int32_t)humCalibData.parH1 * 16))) -
           (((scaledTemp * (int32_t)humCalibData.parH3) / ((int32_t)100)) >> 1);
    int32_t var2 =
        ((int32_t)humCalibData.parH2 *
         (((scaledTemp * (int32_t)humCalibData.parH4) / ((int32_t)100)) +
          (((scaledTemp * ((scaledTemp * (int32_t)humCalibData.parH5) / ((int32_t)100))) >> 6) / ((int32_t)100)) +
          (int32_t)(1 << 14))) >> 10;
    int32_t var3 = var1 * var2;
    int32_t var4 = (int32_t)humCalibData.parH6 << 7;
    var4 = ((var4) + ((scaledTemp * (int32_t)humCalibData.parH7) / ((int32_t)100))) >> 4;
    int32_t var5 = ((var3 >> 14) * (var3 >> 14)) >> 10;
    int32_t var6 = (var4 * var5) >> 1;
    int32_t calc_hum = (((var3 + var6) >> 10) * ((int32_t)1000)) >> 12;
    if (calc_hum > 100000) /* Cap at 100%rH */
    {
        calc_hum = 100000;
    }
    else if (calc_hum < 0)
    {
        calc_hum = 0;
    }

    /*lint -restore */
    return (float)calc_hum / 1000.0;

}
