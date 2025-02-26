/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */


#include "FT232H_BME688_Driver.h"
#include "FT232H_i2c.h"

#include <yarp/os/Log.h>
#include <yarp/os/LogComponent.h>
#include <yarp/os/LogStream.h>

using namespace yarp::os;

bool FT232H_BME688_Driver::open(yarp::os::Searchable &config)
{
    try
    {
        ft232h_i2c = new FT232H_I2C(0);
    }
    catch (std::runtime_error& e)
    {
        yError() << "Runtime error: " << e.what();
        ft232h_i2c = nullptr;
        return false;
    }

    try
    {
        bme688 = new BME688(ft232h_i2c);
    }
    catch(std::runtime_error& e)
    {
        yError() << "Runtime error: " << e.what();
        delete ft232h_i2c;
        ft232h_i2c = nullptr;
        bme688 = nullptr;

        return false;
    }

    if(!sensorDataPort.open("/FT232H_BME688/data"))
    {
        yError() << "Failed to open port /FT232H_BME688_Driver";
        close();
        return false;
    }

    bme688->setupForReading();
    bme688->readTempCalibrationData();
    bme688->readPressureCalibrationData();
    bme688->readHumidityCalibrationData();
    runSensorUpdateThread = true;
    sensorUpdateThread = std::thread(&FT232H_BME688_Driver::run, this);
    yInfo() << "FT232H_BME688 Device is now running";

    return true;
}

bool FT232H_BME688_Driver::close()
{
    runSensorUpdateThread = false;
    sensorUpdateThread.join();

    sensorDataPort.close();

    delete bme688;
    delete ft232h_i2c;
    bme688 = nullptr;
    ft232h_i2c = nullptr;

    return true;
}

void FT232H_BME688_Driver::run()
{

    while(runSensorUpdateThread)
    {
        yarp::sig::Vector sensorOutput(3);
        bme688->readTemperature();
        bme688->readPressure();
        bme688->readHumidity();

        sensorOutput.clear();
        sensorOutput.push_back(bme688->getTemperature());
        sensorOutput.push_back(bme688->getPressure());
        sensorOutput.push_back(bme688->getHumidity());

        sensorDataPort.prepare() = sensorOutput;
        sensorDataPort.write();

    }
}
