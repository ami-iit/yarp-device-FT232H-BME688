/*
 * SPDX-FileCopyrightText: 2023 Istituto Italiano di Tecnologia (IIT)
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef YARP_DEVICETEMPLATE_H
#define YARP_DEVICETEMPLATE_H

#include "BME688.h"
#include "FT232H_i2c.h"
#include <thread>
#include <yarp/dev/DeviceDriver.h>
#include <yarp/sig/Vector.h>

class FT232H_BME688_Driver : public yarp::dev::DeviceDriver {
public:
  // DeviceDriver
  bool open(yarp::os::Searchable &config) override;
  bool close() override;

  void run();

private:
  BME688 *bme688;
  FT232H_I2C *ft232h_i2c;

  // for publishing information
  yarp::os::BufferedPort<yarp::sig::Vector> sensorDataPort;
  std::thread sensorUpdateThread;
  bool runSensorUpdateThread;
};

#endif
