#ifndef BME688_H
#define BME688_H

#include "BME688_reg_addrs.h"
#include "FT232H_i2c.h"
#include <inttypes.h>
#include <unistd.h>

#define BME688_I2C_ADDR 0x77

struct TemperatureCalibData {
  uint16_t parT1;
  int16_t parT2;
  int8_t parT3;
};

struct PressureCalibData {
  uint16_t parP1;
  int16_t parP2;
  int8_t parP3;
  int16_t parP4;
  int16_t parP5;
  int8_t parP6;
  int8_t parP7;
  int16_t parP8;
  int16_t parP9;
  uint8_t parP10;
};

struct HumidityCalibData {
  uint16_t parH1;
  uint16_t parH2;
  int8_t parH3;
  int8_t parH4;
  int8_t parH5;
  uint8_t parH6;
  int8_t parH7;
};

class BME688 {
public:
  BME688(FT232H_I2C *i2cCommsInterface);
  void setupForReading();
  void readTempCalibrationData();
  void readTemperature();
  float getTemperature();
  void readPressureCalibrationData();
  void readPressure();
  float getPressure();
  void readHumidityCalibrationData();
  void readHumidity();
  float getHumidity();

private:
  CHAR tempuratureBytes[3];
  CHAR pressureBytes[3];
  CHAR humidityBytes[2];
  double t_fine;

  struct TemperatureCalibData tempCalibData;
  struct PressureCalibData pressCalibData;
  struct HumidityCalibData humCalibData;
  float temperature;
  float pressure;
  float humidity;
  FT232H_I2C *i2c;
};

#endif
