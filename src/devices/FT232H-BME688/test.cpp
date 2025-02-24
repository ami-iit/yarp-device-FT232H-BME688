#include <unistd.h>
#include "BME688.h"
#include "FT232H_i2c.h"
#include <iostream>

int main(void)
{
    FT232H_I2C* ft232h_i2c;
    try
    {
        ft232h_i2c = new FT232H_I2C(0);
    }
    catch (std::runtime_error& e)
    {
        std::cout << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    BME688* bme688;
    try
    {
        bme688 = new BME688(ft232h_i2c);
    }
    catch(std::runtime_error& e)
    {
        std::cout << "Runtime error: " << e.what() << std::endl;
        return 1;
    }

    bme688->setupForReading();
    bme688->readTempCalibrationData();
    bme688->readPressureCalibrationData();
    bme688->readHumidityCalibrationData();

    std::cout << "Printing out 100 readings of temperature, pressure, and humidity" << std::endl;
    std::cout << std::endl;
    for(int i = 0; i < 100; i++)
    {
        bme688->readTemperature();
        bme688->readPressure();
        bme688->readHumidity();
        bme688->getTemperature();
        std::cout << "Temperature: " << bme688->getTemperature() << std::endl;
        std::cout << "Pressure: " << bme688->getPressure() << std::endl;
        std::cout << "Humidity: " << bme688->getHumidity() << std::endl;
        std::cout << std::endl;

        usleep(100 * 1000);

    }
    delete bme688;
    delete ft232h_i2c;
    bme688 = nullptr;
    ft232h_i2c = nullptr;


    return 0;
}

