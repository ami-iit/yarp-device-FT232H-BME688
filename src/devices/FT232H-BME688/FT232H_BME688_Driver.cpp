/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "FT232H_BME688_Driver.h"

#include <yarp/os/Log.h>
#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>
#include <yarp/os/Network.h>

using namespace yarp::os;
using namespace yarp::dev;

bool FT232H_BME688_Driver::open(yarp::os::Searchable &config)
{
    try
    {
        ft232h_i2c = new FT232H_I2C(0);
    }
    catch (std::runtime_error& e)
    {
        yError() << "Runtime error: " << e.what();
        return false;
    }

    try
    {
        bme688 = new BME688(ft232h_i2c);
    }
    catch(std::runtime_error& e)
    {
        yError() << "Runtime error: " << e.what();
        return false;
    }

    bme688->setupForReading();
    bme688->readTempCalibrationData();
    bme688->readPressureCalibrationData();
    bme688->readHumidityCalibrationData();
    return true;
}

bool FT232H_BME688_Driver::close()
{
    delete bme688;
    delete ft232h_i2c;
    bme688 = nullptr;
    ft232h_i2c = nullptr;

    return true;
}

void FT232H_BME688_Driver::run()
{
    Network yarp;
    BufferedPort<Bottle> port;
    port.open("/FT232H_BME688_Driver");
    Bottle& sensorOutput = port.prepare();

    while(true)
    {
        bme688->readTemperature();
        bme688->readPressure();
        bme688->readHumidity();

        sensorOutput.clear();
        sensorOutput.addFloat64(bme688->getTemperature());
        sensorOutput.addFloat64(bme688->getPressure());
        sensorOutput.addFloat64(bme688->getHumidity());
        port.write();

        Time::delay(0.1);
    }
}
